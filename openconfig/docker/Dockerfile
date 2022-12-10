#
# ***** BEGIN LICENSE BLOCK *****
# 
# Copyright (C) 2009-2022 Olof Hagsand
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

FROM alpine
MAINTAINER Olof Hagsand <olof@hagsand.se>

# For clixon and cligen
RUN apk add --update git make build-base gcc flex bison curl-dev

# nghttp2 dependencies
RUN apk add --update nghttp2

# Create a directory to hold source-code, dependencies etc
RUN mkdir /clixon

RUN mkdir /clixon/build
WORKDIR /clixon

# Clone cligen
RUN git clone https://github.com/clicon/cligen.git

# Build cligen
WORKDIR /clixon/cligen
RUN ./configure --prefix=/clixon/build
RUN make
RUN make install

# Need to add www user manually
RUN adduser -D -H -G www-data www-data

# Clone clixon
WORKDIR /clixon
RUN git clone https://github.com/clicon/clixon.git
WORKDIR /clixon/clixon

# Configure, build and install clixon
RUN ./configure --prefix=/clixon/build --with-cligen=/clixon/build --with-restconf=native --enable-nghttp2

RUN make
RUN make install
RUN make install-include

# Create paths locally (copied to /usr/local in stage 2)
# Paths are according to openconfig.xml config file.
# To make this adaptive, an installation script inside the container can be made, but now
# the hardcoded paths are used.
RUN mkdir /clixon/build/var
RUN mkdir /clixon/build/var/openconfig
RUN mkdir /clixon/build/lib/openconfig
RUN mkdir /clixon/build/lib/openconfig/clispec

# Copy src file from local: config file and standard clispec file
COPY src/openconfig.xml /clixon/build/etc/clixon.xml
COPY src/openconfig_cli.cli /clixon/build/lib/openconfig/clispec/
COPY src/startup_db /clixon/build/var/openconfig/startup_db

# Copy yang files from local. Paths are according to openconfig.xml config file.
RUN mkdir /clixon/build/share/openconfig
WORKDIR /clixon/build/share/openconfig
RUN git clone https://github.com/openconfig/public

# Copy startscript for starting backend and restconf
COPY startsystem.sh /clixon/build/bin/

#
# Stage 2
# The second step skips the development environment and builds a runtime system
FROM alpine
MAINTAINER Olof Hagsand <olof@hagsand.se>

# For clixon and cligen
RUN apk add --update flex bison

# need to add www user manually
RUN adduser -D -H -G www-data www-data

RUN apk add --update openssl 

# nghttp2 dependencies
RUN apk add --update nghttp2

# Expose http port for restconf
EXPOSE 80/tcp
# EXPOSE 443/tcp

# Create clicon user and group
RUN adduser -D -H clicon

COPY --from=0 /clixon/build/ /usr/local/

# Start the backend and restconf deamons
CMD /usr/local/bin/startsystem.sh
