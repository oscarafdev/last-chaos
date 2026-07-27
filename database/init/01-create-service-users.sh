#!/usr/bin/env bash
set -Eeuo pipefail

readonly CRUD_PRIVILEGES="SELECT, INSERT, UPDATE, DELETE, EXECUTE, CREATE TEMPORARY TABLES"

require_secret() {
  local variable_name="$1"
  if [[ -z "${!variable_name:-}" ]]; then
    echo "Missing required database secret: ${variable_name}" >&2
    exit 1
  fi
}

escape_sql_string() {
  printf '%s' "$1" | sed "s/'/''/g"
}

create_service_user() {
  local user="$1"
  local password="$2"
  shift 2

  local escaped_password
  escaped_password="$(escape_sql_string "${password}")"

  mariadb \
    --protocol=socket \
    -uroot \
    -p"${MARIADB_ROOT_PASSWORD}" \
    --execute="CREATE USER IF NOT EXISTS '${user}'@'%' IDENTIFIED BY '${escaped_password}'; ALTER USER '${user}'@'%' IDENTIFIED BY '${escaped_password}';"

  local database_name
  for database_name in "$@"; do
    mariadb \
      --protocol=socket \
      -uroot \
      -p"${MARIADB_ROOT_PASSWORD}" \
      --execute="GRANT ${CRUD_PRIVILEGES} ON \`${database_name}\`.* TO '${user}'@'%';"
  done
}

for variable_name in \
  LC_DB_LOGIN_PASSWORD \
  LC_DB_CONNECTOR_PASSWORD \
  LC_DB_GAME_PASSWORD \
  LC_DB_HELPER_PASSWORD \
  LC_DB_SUBHELPER_PASSWORD \
  LC_DB_CASH_PASSWORD \
  LC_DB_REGISTRATION_PASSWORD
do
  require_secret "${variable_name}"
done

mariadb \
  --protocol=socket \
  -uroot \
  -p"${MARIADB_ROOT_PASSWORD}" \
  --execute="DROP USER IF EXISTS 'root'@'%';"

create_service_user \
  lc_login "${LC_DB_LOGIN_PASSWORD}" \
  2018_nov_db_auth 2018_nov_data
create_service_user \
  lc_connector "${LC_DB_CONNECTOR_PASSWORD}" \
  2018_nov_db_auth
create_service_user \
  lc_game "${LC_DB_GAME_PASSWORD}" \
  2018_nov_db 2018_nov_data 2018_nov_post
create_service_user \
  lc_helper "${LC_DB_HELPER_PASSWORD}" \
  2018_nov_db
create_service_user \
  lc_subhelper "${LC_DB_SUBHELPER_PASSWORD}" \
  2018_nov_db 2018_nov_data 2018_nov_post
create_service_user \
  lc_cash "${LC_DB_CASH_PASSWORD}" \
  2018_nov_data 2018_nov_db_auth

escaped_registration_password="$(
  escape_sql_string "${LC_DB_REGISTRATION_PASSWORD}"
)"
mariadb \
  --protocol=socket \
  -uroot \
  -p"${MARIADB_ROOT_PASSWORD}" \
  --execute="CREATE USER IF NOT EXISTS 'lc_registration'@'%' IDENTIFIED BY '${escaped_registration_password}'; ALTER USER 'lc_registration'@'%' IDENTIFIED BY '${escaped_registration_password}'; GRANT SELECT, INSERT ON \`2018_nov_db_auth\`.\`bg_user\` TO 'lc_registration'@'%';"
