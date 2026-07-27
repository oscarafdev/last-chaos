package account

import (
	"context"
	"database/sql"
	"errors"

	"github.com/go-sql-driver/mysql"
)

type MySQLRepository struct {
	database *sql.DB
}

func NewMySQLRepository(database *sql.DB) *MySQLRepository {
	return &MySQLRepository{database: database}
}

func (repository *MySQLRepository) Create(
	ctx context.Context,
	username string,
	passwordHash string,
) error {
	const query = `
		INSERT INTO bg_user (
			user_id,
			passwd,
			active_time,
			create_date,
			password_old
		)
		VALUES (?, ?, NOW(), NOW(), '')`

	_, err := repository.database.ExecContext(
		ctx,
		query,
		username,
		passwordHash,
	)
	if err == nil {
		return nil
	}

	var databaseError *mysql.MySQLError
	if errors.As(err, &databaseError) && databaseError.Number == 1062 {
		return ErrUsernameTaken
	}
	return err
}
