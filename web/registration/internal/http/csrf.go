package web

import (
	"crypto/subtle"
	"net/http"
	"net/url"
	"strings"
)

type csrfSignals struct {
	tokenMatches    bool
	originMatches   bool
	sameOriginFetch bool
}

func (signals csrfSignals) valid() bool {
	matches := 0
	if signals.tokenMatches {
		matches++
	}
	if signals.originMatches {
		matches++
	}
	if signals.sameOriginFetch {
		matches++
	}
	return matches >= 2
}

func evaluateCSRFSignals(
	request *http.Request,
	cookieToken string,
	publicOrigin *url.URL,
) csrfSignals {
	formToken := request.FormValue("csrf_token")
	tokenMatches := len(formToken) == len(cookieToken) &&
		len(formToken) >= 32 &&
		subtle.ConstantTimeCompare(
			[]byte(formToken),
			[]byte(cookieToken),
		) == 1

	originMatches := false
	if parsedOrigin, err := url.Parse(request.Header.Get("Origin")); err == nil {
		originMatches =
			strings.EqualFold(parsedOrigin.Scheme, publicOrigin.Scheme) &&
				strings.EqualFold(parsedOrigin.Host, publicOrigin.Host)
	}

	return csrfSignals{
		tokenMatches:  tokenMatches,
		originMatches: originMatches,
		sameOriginFetch: strings.EqualFold(
			strings.TrimSpace(request.Header.Get("Sec-Fetch-Site")),
			"same-origin",
		),
	}
}
