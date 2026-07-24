#!/usr/bin/env bash
set -Eeuo pipefail

readonly SERVER_ROOT="/opt/lastchaos"
readonly DB_HOST="${DB_HOST:-database}"
readonly DB_PASSWORD="${DB_PASSWORD:-lastchaos}"
readonly SERVER_BIND_IP="${SERVER_BIND_IP:-${SERVER_IP:-0.0.0.0}}"
readonly SERVER_PUBLIC_IP="${SERVER_PUBLIC_IP:-127.0.0.1}"
readonly ENABLE_CASH_SERVER="${ENABLE_CASH_SERVER:-true}"

escape_sed_replacement() {
  printf '%s' "$1" | sed 's/[&|\]/\\&/g'
}

configure_server() {
  local escaped_host escaped_password escaped_bind_ip escaped_public_ip
  escaped_host="$(escape_sed_replacement "${DB_HOST}")"
  escaped_password="$(escape_sed_replacement "${DB_PASSWORD}")"
  escaped_bind_ip="$(escape_sed_replacement "${SERVER_BIND_IP}")"
  escaped_public_ip="$(escape_sed_replacement "${SERVER_PUBLIC_IP}")"

  while IFS= read -r -d '' config; do
    sed -i \
      -e "s|IP=127\.0\.0\.1|IP=${escaped_host}|g" \
      -e "s|Password=Password|Password=${escaped_password}|g" \
      -e "s|^IP=192\.168\.0\.108$|IP=${escaped_bind_ip}|g" \
      -e "s|^EX_IP_1=192\.168\.0\.108$|EX_IP_1=${escaped_public_ip}|g" \
      "${config}"
  done < <(find "${SERVER_ROOT}" -name newStobm.bin -print0)
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
start_component GameServer GameServer_d

echo "Last Chaos 2018 services started."
wait -n "${pids[@]}"
exit_code=$?
echo "A server component stopped with exit code ${exit_code}."
exit "${exit_code}"
