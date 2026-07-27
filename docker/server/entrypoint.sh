#!/usr/bin/env bash
set -Eeuo pipefail

readonly SERVER_ROOT="/opt/lastchaos"
readonly DB_HOST="${DB_HOST:-database}"
readonly SERVER_BIND_IP="${SERVER_BIND_IP:-${SERVER_IP:-0.0.0.0}}"
readonly SERVER_PUBLIC_IP="${SERVER_PUBLIC_IP:?SERVER_PUBLIC_IP is required}"
readonly ENABLE_CASH_SERVER="${ENABLE_CASH_SERVER:-false}"

require_secret() {
  local variable_name="$1"
  if [[ -z "${!variable_name:-}" ]]; then
    echo "Missing required secret: ${variable_name}" >&2
    exit 1
  fi
}

configure_file() {
  local config="$1"
  local database_user="$2"
  local database_password="$3"
  local temporary
  temporary="$(mktemp)"

  awk \
    -v bind_ip="${SERVER_BIND_IP}" \
    -v db_host="${DB_HOST}" \
    -v db_user="${database_user}" \
    -v db_password="${database_password}" \
    -v public_ip="${SERVER_PUBLIC_IP}" '
      /^\[/ {
        section = $0
      }
      /^IP=/ {
        if (section == "[Server]") {
          print "IP=" bind_ip
        } else if (section ~ / DB\]$/) {
          print "IP=" db_host
        } else {
          print "IP=127.0.0.1"
        }
        next
      }
      /^EX_IP_[123]=/ {
        sub(/=.*/, "=" public_ip)
        print
        next
      }
      /^User=/ {
        print "User=" db_user
        next
      }
      /^Password=/ {
        print "Password=" db_password
        next
      }
      {
        print
      }
    ' "${config}" > "${temporary}"

  cat "${temporary}" > "${config}"
  rm -f "${temporary}"
}

configure_server() {
  require_secret LC_DB_LOGIN_PASSWORD
  require_secret LC_DB_CONNECTOR_PASSWORD
  require_secret LC_DB_GAME_PASSWORD
  require_secret LC_DB_HELPER_PASSWORD
  require_secret LC_DB_SUBHELPER_PASSWORD
  require_secret LC_DB_CASH_PASSWORD

  configure_file \
    "${SERVER_ROOT}/LoginServer/newStobm.bin" \
    lc_login "${LC_DB_LOGIN_PASSWORD}"
  configure_file \
    "${SERVER_ROOT}/Connector/newStobm.bin" \
    lc_connector "${LC_DB_CONNECTOR_PASSWORD}"
  configure_file \
    "${SERVER_ROOT}/GameServer/data/newStobm.bin" \
    lc_game "${LC_DB_GAME_PASSWORD}"
  configure_file \
    "${SERVER_ROOT}/Helper/newStobm.bin" \
    lc_helper "${LC_DB_HELPER_PASSWORD}"
  configure_file \
    "${SERVER_ROOT}/SubHelper/newStobm.bin" \
    lc_subhelper "${LC_DB_SUBHELPER_PASSWORD}"
  configure_file \
    "${SERVER_ROOT}/Messenger/newStobm.bin" \
    lc_subhelper "${LC_DB_SUBHELPER_PASSWORD}"
  configure_file \
    "${SERVER_ROOT}/CashServer/newStobm.bin" \
    lc_cash "${LC_DB_CASH_PASSWORD}"
}

wait_for_database() {
  echo "Waiting for database at ${DB_HOST}:3306..."
  until nc -z "${DB_HOST}" 3306; do
    sleep 2
  done
}

start_component() {
  local directory="$1"
  local executable="$2"

  echo "Starting ${directory}/${executable}"
  (
    cd "${SERVER_ROOT}/${directory}"
    exec "./${executable}"
  ) &
  pids+=("$!")
}

stop_components() {
  local pid
  echo "Stopping Last Chaos services..."
  for pid in "${pids[@]:-}"; do
    kill "${pid}" 2>/dev/null || true
  done
  wait || true
}

configure_server
wait_for_database

declare -a pids=()
trap stop_components SIGINT SIGTERM EXIT

start_component Messenger Messenger
start_component Helper Helper
start_component SubHelper SubHelper

if [[ "${ENABLE_CASH_SERVER,,}" == "true" ]]; then
  export LD_LIBRARY_PATH="${SERVER_ROOT}/CashServer:${LD_LIBRARY_PATH:-}"
  start_component CashServer CashServer
fi

start_component Connector Connector
start_component LoginServer LoginServer
start_component GameServer GameServer

echo "Last Chaos 2018 services started."
wait -n "${pids[@]}"
exit_code=$?
echo "A server component stopped with exit code ${exit_code}."
exit "${exit_code}"
