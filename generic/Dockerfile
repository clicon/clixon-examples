#
# ***** BEGIN LICENSE BLOCK *****
#
# Copyright (C) 2009-2026 Olof Hagsand
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
# Generic clixon container - no application-specific config or yang baked in.
# Provide your clixon config and yang models as volume mounts at runtime:
#
#   Config file : mount at /usr/local/etc/clixon.xml
#                 (or set CLIXON_CONFIG env var to the path inside the container)
#   Yang models : mount at any path; reference it via CLICON_YANG_DIR in your config
#                 Suggested mount point: /usr/local/share/yang/
#
# The clixon built-in yang modules are installed at /usr/local/share/clixon/
# and are always available.
#

FROM alpine AS build

# Build dependencies
RUN apk add --update git make build-base gcc flex bison nghttp2 curl-dev

# For netsnmp
RUN apk add --update net-snmp net-snmp-dev

# For grpc/gnmi
RUN apk add --update protobuf-dev protobuf-c-dev

# For groupadd/groupdel
RUN apk add --update shadow

# Create a directory to hold source-code, dependencies etc
RUN mkdir /clixon
RUN mkdir /clixon/build
WORKDIR /clixon

# Clone and build cligen
RUN git clone https://github.com/clicon/cligen.git
WORKDIR /clixon/cligen
RUN ./configure --prefix=/usr/local
RUN make
RUN make DESTDIR=/clixon/build install

# Need to add www user manually before building clixon
RUN adduser -D -H -G www-data www-data

# Clone and build clixon
WORKDIR /clixon
RUN git clone https://github.com/clicon/clixon.git
WORKDIR /clixon/clixon
RUN ./configure --prefix=/usr/local --with-cligen=/clixon/build/usr/local --with-restconf=native --enable-nghttp2 --with-yang-installdir=/usr/local/share/yang/clixon --enable-grpc --with-mib-generated-yang-dir=/usr/local/share/mib-yangs/
RUN make

RUN make DESTDIR=/clixon/build install
RUN make DESTDIR=/clixon/build install-include

# Create the suggested yang model mount point and a writable datastore dir
RUN mkdir -p /clixon/build/usr/local/share/yang/main
RUN mkdir -p /clixon/build/usr/local/share/yang/lib
RUN mkdir -p /clixon/build/usr/local/var/clixon
RUN mkdir -p /clixon/build/usr/local/etc/clixon/generic.d

# Download standard YANG models from YangModels/yang (sparse checkout of standard/ only)
RUN apk add --no-cache git
RUN git clone --depth 1 --filter=blob:none --sparse \
        https://github.com/YangModels/yang.git /clixon/yangmodels
WORKDIR /clixon/yangmodels
RUN git sparse-checkout set standard
RUN cp -r standard /clixon/build/usr/local/share/yang

# Copy config file
WORKDIR /clixon
COPY generic.xml .
RUN install generic.xml /clixon/build/usr/local/etc/clixon/
RUN ln -s clixon/generic.xml clixon/generic.xml

# Copy clispec file
WORKDIR /clixon
COPY generic.cli .
COPY pipe.cli .
RUN mkdir /clixon/build/usr/local/lib/clispec
RUN install generic.cli /clixon/build/usr/local/lib/clispec/
RUN install pipe.cli /clixon/build/usr/local/lib/clispec/

# Copy the startup script
WORKDIR /clixon
COPY startsystem.sh .
RUN install startsystem.sh /clixon/build/usr/local/bin/

#
# Stage 2 - runtime only
#
FROM alpine

RUN apk add --update flex bison openssl nghttp2 git

# For grpc/gnmi
RUN apk add --update protobuf-c

# Users required by clixon
RUN adduser -D -H -G www-data www-data
RUN adduser -D -H clicon

# SSH daemon and noc user for NETCONF access
RUN apk add --no-cache openssh
RUN adduser -h /home/noc -s /bin/sh -D noc
RUN adduser noc clicon
RUN echo -n 'noc:noc' | chpasswd

EXPOSE 80/tcp
EXPOSE 22/tcp
EXPOSE 830/tcp
EXPOSE 9339/tcp

# Copy the built binaries and libraries from stage 1
COPY --from=build /clixon/build/ /

# The clixon config is expected at /usr/local/etc/clixon.xml (default) or at
# the path given by the CLIXON_CONFIG environment variable.
# The CLICON_YANG_DIR paths in your config determine where yang files are read from.

CMD ["/usr/local/bin/startsystem.sh"]
