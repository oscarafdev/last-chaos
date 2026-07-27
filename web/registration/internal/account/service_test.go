package account

import (
	"context"
	"testing"
)

type recordingRepository struct {
	username     string
	passwordHash string
	err          error
}

func (repository *recordingRepository) Create(
	_ context.Context,
	username string,
	passwordHash string,
) error {
	repository.username = username
	repository.passwordHash = passwordHash
	return repository.err
}

func TestRegisterUsesLegacyCompatibleHash(t *testing.T) {
	repository := &recordingRepository{}
	service := NewService(
		repository,
		"phoohie1yaihooyaequae7PuiWoeNgahjieth3ru3yeeghaepahb7aeYaipe2we6zii6mai6uweig8siasheinoungeoyeiLohShi2xoh2xi8ooxee9ahpiehahc9Phe",
	)

	if err := service.Register(
		context.Background(),
		"NOOB",
		"noob-password",
	); err != nil {
		t.Fatal(err)
	}

	if repository.username != "noob" {
		t.Fatalf("expected normalized username, got %q", repository.username)
	}
	const expectedHash = "D78F415C92E72EE1BD081E331A2628D98AE6DA38E78619E9FFF4979930856339"
	if repository.passwordHash != expectedHash {
		t.Fatalf(
			"expected hash %s, got %s",
			expectedHash,
			repository.passwordHash,
		)
	}
}

func TestRegisterRejectsInvalidValues(t *testing.T) {
	service := NewService(&recordingRepository{}, "salt")
	testCases := []struct {
		name     string
		username string
		password string
	}{
		{name: "short username", username: "abc", password: "password-1"},
		{name: "invalid username", username: "user name", password: "password-1"},
		{name: "short password", username: "validuser", password: "short"},
		{name: "same password", username: "sameuser", password: "sameuser"},
	}

	for _, testCase := range testCases {
		t.Run(testCase.name, func(t *testing.T) {
			if err := service.Register(
				context.Background(),
				testCase.username,
				testCase.password,
			); err == nil {
				t.Fatal("expected validation error")
			}
		})
	}
}
