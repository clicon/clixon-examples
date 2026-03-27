#!/bin/sh

# ***** BEGIN LICENSE BLOCK *****
#
# Copyright (C) 2017-2024 Olof Hagsand
# Copyright (C) 2020-2022 Olof Hagsand and Rubicon Communications, LLC
#
# This file is part of CLIXON
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Alternatively, the contents of this file may be used under the terms of
# the GNU General Public License Version 3 or later (the "GPL"),
# in which case the provisions of the GPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of the GPL, and not to allow others to
# use your version of this file under the terms of Apache License version 2,
# indicate your decision by deleting the provisions above and replace them with
# the notice and other provisions required by the GPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the Apache License version 2 or the GPL.
#
# ***** END LICENSE BLOCK *****
#
# Generic clixon container startup script.
# Runs inside the container at start time.
# Starts the clixon backend and then runs sshd in the foreground.
#
# Environment variables:
#   CLIXON_CONFIG  Path to the clixon config XML file
#                  Default: /usr/local/etc/clixon.xml
#   DBG            Debug level passed to backend/restconf (default: 0)

set -u

>&2 echo "$0"

DBG=${DBG:-0}
CLIXON_CONFIG=${CLIXON_CONFIG:-/usr/local/etc/clixon.xml}

# Workaround for: sudo: setrlimit(RLIMIT_CORE): Operation not permitted
echo "Set disable_coredump false" > /etc/sudo.conf

if [ ! -f "${CLIXON_CONFIG}" ]; then
    >&2 echo "ERROR: clixon config file not found: ${CLIXON_CONFIG}"
    >&2 echo "Mount your config file at ${CLIXON_CONFIG} or set CLIXON_CONFIG"
    exit 1
fi

chown noc:noc /home/noc

# If an authorized_keys file is mounted, fix ownership and permissions so sshd accepts it
if [ -f /home/noc/.ssh/authorized_keys ]; then
    chown -R noc:noc /home/noc/.ssh
    chmod 700 /home/noc/.ssh
    chmod 600 /home/noc/.ssh/authorized_keys
fi

# Start clixon backend (logs go to docker logs via -l e)
>&2 echo "Starting clixon_backend with config: ${CLIXON_CONFIG}"
/usr/local/sbin/clixon_backend -D "${DBG}" -f "${CLIXON_CONFIG}" -l e

# Generate sshd_config with the correct clixon_netconf path and config file.
# This runs on both port 22 (interactive) and port 830 (NETCONF).
cat > /etc/ssh/sshd_config <<EOF
AuthorizedKeysFile .ssh/authorized_keys
AllowTcpForwarding no
GatewayPorts no
X11Forwarding no
PasswordAuthentication yes
PubkeyAuthentication yes
Subsystem netconf /usr/local/bin/clixon_netconf -f ${CLIXON_CONFIG}
Port 22
Port 830
Match LocalPort 830
  ForceCommand /usr/local/bin/clixon_netconf -f ${CLIXON_CONFIG}
EOF

ssh-keygen -A
exec /usr/sbin/sshd -D -e "$@"
