package config

import (
	"fmt"
	"os"
	"strings"
)

type Config struct {
	ListenAddress string
	DatabaseDSN   string
	PasswordSalt  string
	PublicOrigin  string
	SecureCookies bool
}

func FromEnvironment() (Config, error) {
	cfg := Config{
		ListenAddress: valueOrDefault("LISTEN_ADDRESS", "0.0.0.0:8080"),
		DatabaseDSN:   os.Getenv("DB_DSN"),
		PasswordSalt:  os.Getenv("PASSWORD_SALT"),
		PublicOrigin:  strings.TrimRight(os.Getenv("PUBLIC_ORIGIN"), "/"),
		SecureCookies: strings.EqualFold(
			valueOrDefault("SECURE_COOKIES", "true"),
			"true",
		),
	}

	if cfg.DatabaseDSN == "" {
		return Config{}, fmt.Errorf("DB_DSN is required")
	}
	if cfg.PasswordSalt == "" {
		return Config{}, fmt.Errorf("PASSWORD_SALT is required")
	}
	if cfg.PublicOrigin == "" {
		return Config{}, fmt.Errorf("PUBLIC_ORIGIN is required")
	}

	return cfg, nil
}

func valueOrDefault(name, fallback string) string {
	if value := os.Getenv(name); value != "" {
		return value
	}
	return fallback
}
