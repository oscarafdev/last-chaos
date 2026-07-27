package web

import (
	"net/http/httptest"
	"net/url"
	"strings"
	"testing"
)

func TestEvaluateCSRFSignals(t *testing.T) {
	const token = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFG"
	publicOrigin, err := url.Parse("https://lc.somositconfig.com")
	if err != nil {
		t.Fatal(err)
	}

	tests := []struct {
		name          string
		cookieToken   string
		formToken     string
		origin        string
		fetchSite     string
		expectedValid bool
	}{
		{
			name:          "all browser signals",
			cookieToken:   token,
			formToken:     token,
			origin:        publicOrigin.String(),
			fetchSite:     "same-origin",
			expectedValid: true,
		},
		{
			name:          "privacy extension strips cookie",
			formToken:     token,
			origin:        publicOrigin.String(),
			fetchSite:     "same-origin",
			expectedValid: true,
		},
		{
			name:          "privacy extension strips origin",
			cookieToken:   token,
			formToken:     token,
			fetchSite:     "same-origin",
			expectedValid: true,
		},
		{
			name:          "older browser lacks fetch metadata",
			cookieToken:   token,
			formToken:     token,
			origin:        publicOrigin.String(),
			expectedValid: true,
		},
		{
			name:          "cross-site request",
			cookieToken:   token,
			formToken:     token,
			origin:        "https://attacker.example",
			fetchSite:     "cross-site",
			expectedValid: false,
		},
		{
			name:          "forged form without cookie",
			formToken:     token,
			origin:        "https://attacker.example",
			fetchSite:     "cross-site",
			expectedValid: false,
		},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			request := httptest.NewRequest(
				"POST",
				publicOrigin.String()+"/",
				strings.NewReader("csrf_token="+url.QueryEscape(test.formToken)),
			)
			request.Header.Set(
				"Content-Type",
				"application/x-www-form-urlencoded",
			)
			request.Header.Set("Origin", test.origin)
			request.Header.Set("Sec-Fetch-Site", test.fetchSite)
			if err := request.ParseForm(); err != nil {
				t.Fatal(err)
			}

			signals := evaluateCSRFSignals(
				request,
				test.cookieToken,
				publicOrigin,
			)
			if signals.valid() != test.expectedValid {
				t.Fatalf(
					"valid() = %v, expected %v; signals: %+v",
					signals.valid(),
					test.expectedValid,
					signals,
				)
			}
		})
	}
}
