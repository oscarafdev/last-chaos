package web

import (
	"crypto/rand"
	"crypto/subtle"
	"embed"
	"encoding/base64"
	"errors"
	"html/template"
	"log"
	"net"
	"net/http"
	"net/url"
	"strings"
	"sync"
	"time"

	"lastchaos/registration/internal/account"
)

//go:embed templates/*.html
var templateFiles embed.FS

type Handler struct {
	service       *account.Service
	template      *template.Template
	publicOrigin  *url.URL
	secureCookies bool
	limiter       *rateLimiter
}

type pageData struct {
	CSRFToken string
	Error     string
	Success   bool
	Username  string
}

func NewHandler(
	service *account.Service,
	publicOrigin string,
	secureCookies bool,
) (*Handler, error) {
	parsedOrigin, err := url.Parse(publicOrigin)
	if err != nil {
		return nil, err
	}
	parsedTemplate, err := template.ParseFS(templateFiles, "templates/*.html")
	if err != nil {
		return nil, err
	}

	return &Handler{
		service:       service,
		template:      parsedTemplate,
		publicOrigin:  parsedOrigin,
		secureCookies: secureCookies,
		limiter:       newRateLimiter(5, 10*time.Minute),
	}, nil
}

func (handler *Handler) Routes() http.Handler {
	mux := http.NewServeMux()
	mux.HandleFunc("GET /health", handler.health)
	mux.HandleFunc("GET /", handler.form)
	mux.HandleFunc("POST /", handler.submit)
	return securityHeaders(mux)
}

func (handler *Handler) health(writer http.ResponseWriter, _ *http.Request) {
	writer.Header().Set("Content-Type", "text/plain; charset=utf-8")
	writer.WriteHeader(http.StatusOK)
	_, _ = writer.Write([]byte("ok"))
}

func (handler *Handler) form(
	writer http.ResponseWriter,
	request *http.Request,
) {
	token, err := handler.ensureCSRFToken(writer, request)
	if err != nil {
		http.Error(writer, "No se pudo iniciar el formulario.", http.StatusInternalServerError)
		return
	}
	handler.render(writer, pageData{CSRFToken: token})
}

func (handler *Handler) submit(
	writer http.ResponseWriter,
	request *http.Request,
) {
	request.Body = http.MaxBytesReader(writer, request.Body, 16<<10)
	if err := request.ParseForm(); err != nil {
		http.Error(writer, "Solicitud inválida.", http.StatusBadRequest)
		return
	}

	token, err := handler.ensureCSRFToken(writer, request)
	if err != nil || !handler.validRequest(request, token) {
		http.Error(writer, "Solicitud inválida.", http.StatusForbidden)
		return
	}
	if request.FormValue("website") != "" {
		http.Error(writer, "Solicitud inválida.", http.StatusBadRequest)
		return
	}
	if !handler.limiter.Allow(clientIP(request)) {
		http.Error(
			writer,
			"Demasiados intentos. Espera unos minutos.",
			http.StatusTooManyRequests,
		)
		return
	}

	username := strings.ToLower(strings.TrimSpace(request.FormValue("username")))
	password := request.FormValue("password")
	if password != request.FormValue("password_confirmation") {
		handler.render(writer, pageData{
			CSRFToken: token,
			Error:     "Las contraseñas no coinciden.",
			Username:  username,
		})
		return
	}

	err = handler.service.Register(request.Context(), username, password)
	switch {
	case err == nil:
		handler.render(writer, pageData{
			CSRFToken: token,
			Success:   true,
		})
	case errors.Is(err, account.ErrUsernameTaken):
		handler.render(writer, pageData{
			CSRFToken: token,
			Error:     "Ese usuario ya está registrado.",
			Username:  username,
		})
	default:
		log.Printf("registration failed: %v", err)
		handler.render(writer, pageData{
			CSRFToken: token,
			Error:     "No se pudo crear la cuenta. Revisa los datos e inténtalo nuevamente.",
			Username:  username,
		})
	}
}

func (handler *Handler) validRequest(
	request *http.Request,
	cookieToken string,
) bool {
	formToken := request.FormValue("csrf_token")
	if len(formToken) != len(cookieToken) ||
		subtle.ConstantTimeCompare([]byte(formToken), []byte(cookieToken)) != 1 {
		return false
	}

	origin := request.Header.Get("Origin")
	if origin == "" {
		return false
	}
	parsedOrigin, err := url.Parse(origin)
	if err != nil {
		return false
	}
	return strings.EqualFold(parsedOrigin.Scheme, handler.publicOrigin.Scheme) &&
		strings.EqualFold(parsedOrigin.Host, handler.publicOrigin.Host)
}

func (handler *Handler) ensureCSRFToken(
	writer http.ResponseWriter,
	request *http.Request,
) (string, error) {
	if cookie, err := request.Cookie("lc_csrf"); err == nil &&
		len(cookie.Value) >= 32 {
		return cookie.Value, nil
	}

	randomBytes := make([]byte, 32)
	if _, err := rand.Read(randomBytes); err != nil {
		return "", err
	}
	token := base64.RawURLEncoding.EncodeToString(randomBytes)
	http.SetCookie(writer, &http.Cookie{
		Name:     "lc_csrf",
		Value:    token,
		Path:     "/",
		MaxAge:   3600,
		HttpOnly: true,
		Secure:   handler.secureCookies,
		SameSite: http.SameSiteStrictMode,
	})
	return token, nil
}

func (handler *Handler) render(writer http.ResponseWriter, data pageData) {
	writer.Header().Set("Content-Type", "text/html; charset=utf-8")
	if err := handler.template.ExecuteTemplate(
		writer,
		"register.html",
		data,
	); err != nil {
		log.Printf("template render failed: %v", err)
	}
}

func securityHeaders(next http.Handler) http.Handler {
	return http.HandlerFunc(func(writer http.ResponseWriter, request *http.Request) {
		writer.Header().Set("Content-Security-Policy", "default-src 'self'; style-src 'unsafe-inline'; form-action 'self'; frame-ancestors 'none'")
		writer.Header().Set("Referrer-Policy", "no-referrer")
		writer.Header().Set("X-Content-Type-Options", "nosniff")
		writer.Header().Set("X-Frame-Options", "DENY")
		next.ServeHTTP(writer, request)
	})
}

func clientIP(request *http.Request) string {
	if realIP := request.Header.Get("X-Real-IP"); net.ParseIP(realIP) != nil {
		return realIP
	}
	host, _, err := net.SplitHostPort(request.RemoteAddr)
	if err == nil {
		return host
	}
	return request.RemoteAddr
}

type rateLimiter struct {
	mutex  sync.Mutex
	limit  int
	window time.Duration
	hits   map[string][]time.Time
}

func newRateLimiter(limit int, window time.Duration) *rateLimiter {
	return &rateLimiter{
		limit:  limit,
		window: window,
		hits:   make(map[string][]time.Time),
	}
}

func (limiter *rateLimiter) Allow(key string) bool {
	limiter.mutex.Lock()
	defer limiter.mutex.Unlock()

	now := time.Now()
	cutoff := now.Add(-limiter.window)
	recent := limiter.hits[key][:0]
	for _, hit := range limiter.hits[key] {
		if hit.After(cutoff) {
			recent = append(recent, hit)
		}
	}
	if len(recent) >= limiter.limit {
		limiter.hits[key] = recent
		return false
	}
	limiter.hits[key] = append(recent, now)
	return true
}
