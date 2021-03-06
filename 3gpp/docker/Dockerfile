#
# ***** BEGIN LICENSE BLOCK *****
# 
# Copyright (C) 2020 Olof Hagsand and Rubicon Communications, LLC
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
RUN apk add --update git make build-base gcc flex bison fcgi-dev curl-dev

# Create a directory to hold source-code, dependencies etc
RUN mkdir /clixon
RUN mkdir /clixon/build
WORKDIR /clixon

# Clone cligen
RUN git clone https://github.com/olofhagsand/cligen.git

# Build cligen
WORKDIR /clixon/cligen
RUN ./configure --enable-debug --prefix=/clixon/build
RUN make
RUN make install

# For nginx and RESTCONF: Need to add www user manually
RUN adduser -D -H www-data
# nginx adds group www-data
RUN apk add --update nginx

# Clone clixon
WORKDIR /clixon
RUN git clone https://github.com/clicon/clixon.git

# Configure, build and install clixon
WORKDIR /clixon/clixon
RUN ./configure --enable-debug --prefix=/clixon/build --with-cligen=/clixon/build --with-wwwuser=www-data --enable-debug
RUN make
RUN make install
RUN make install-include

# Create paths locally (copied to /usr/local in stage 2)
# Paths are according to 3gpp.xml config file.
# To make this adaptive, an installation script inside the container can be made, but now
# the hardcoded paths are used.
RUN mkdir /clixon/build/var
RUN mkdir /clixon/build/var/3gpp
RUN mkdir /clixon/build/lib/3gpp
RUN mkdir /clixon/build/lib/3gpp/clispec

# Copy src file from local: config file and standard clispec file
COPY src/3gpp.xml /clixon/build/etc/clixon.xml
COPY src/3gpp_cli.cli /clixon/build/lib/3gpp/clispec/

# Copy yang files from local. Paths are according to openconfig.xml config file.
RUN mkdir /clixon/build/share/clixon/3gpp
COPY yang /clixon/build/share/clixon/3gpp

# Copy startscript for starting backend and restconf
COPY startsystem.sh /clixon/build/bin/

#
# Stage 2
#
FROM alpine
MAINTAINER Olof Hagsand <olof@hagsand.se>

# For clixon and cligen
RUN apk add --update flex bison fcgi-dev 

# need to add www user manually
RUN adduser -D -H www-data
# nginx adds group www-data
RUN apk add --update nginx

# Expose nginx port for restconf
EXPOSE 80

# Create clicon user and group
RUN adduser -D -H clicon
RUN adduser nginx clicon
RUN adduser www-data clicon

COPY --from=0 /clixon/build/ /usr/local/
COPY --from=0 /www-data /www-data

# Manually created
RUN chown www-data /www-data
RUN chgrp www-data /www-data

# For debug
RUN apk add --update gdb

# Start the backend and restconf deamons
CMD /usr/local/bin/startsystem.sh