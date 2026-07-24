#!/usr/bin/env bash
set -Eeuo pipefail

readonly LOGIN_PORT_HEX="0FA1"

# Inspect the kernel socket table instead of opening a protocol-less TCP
# connection. Netcat makes LoginServer report a false EOF on every check.
is_login_server_listening() {
  local slot local_address remote_address state remainder

  while read -r slot local_address remote_address state remainder; do
    if [[ "${local_address##*:}" == "${LOGIN_PORT_HEX}" && "${state}" == "0A" ]]; then
      return 0
    fi
  done < /proc/net/tcp

  return 1
}

is_login_server_listening
