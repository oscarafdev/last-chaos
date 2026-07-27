package account

import (
	"context"
	"crypto/sha256"
	"encoding/hex"
	"errors"
	"fmt"
	"regexp"
	"strings"
)

var (
	ErrUsernameTaken = errors.New("username is already registered")
	usernamePattern  = regexp.MustCompile(`^[a-z0-9]{4,20}$`)
	passwordPattern  = regexp.MustCompile(`^[\x21-\x7E]{8,32}$`)
)

type Repository interface {
	Create(ctx context.Context, username, passwordHash string) error
}

type Service struct {
	repository Repository
	salt       string
}

func NewService(repository Repository, salt string) *Service {
	return &Service{repository: repository, salt: salt}
}

func (service *Service) Register(
	ctx context.Context,
	username string,
	password string,
) error {
	username = strings.ToLower(strings.TrimSpace(username))
	if !usernamePattern.MatchString(username) {
		return fmt.Errorf(
			"el usuario debe tener entre 4 y 20 letras minúsculas o números",
		)
	}
	if !passwordPattern.MatchString(password) {
		return fmt.Errorf(
			"la contraseña debe tener entre 8 y 32 caracteres ASCII sin espacios",
		)
	}
	if strings.EqualFold(username, password) {
		return fmt.Errorf("la contraseña no puede ser igual al usuario")
	}

	sum := sha256.Sum256([]byte(username + service.salt + password))
	return service.repository.Create(
		ctx,
		username,
		strings.ToUpper(hex.EncodeToString(sum[:])),
	)
}
