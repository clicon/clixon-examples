#
# ***** BEGIN LICENSE BLOCK *****
# 
# Copyright (C) 2020-2021, 2023 Olof Hagsand and Rubicon Communications, LLC
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

# To rebuild the `configure' script from this, execute the command
# 	autoconf
# in the directory containing this script.
#

AC_INIT([clixon-example],[0.1])

# Default CFLAGS and INSTALLFLAGS unless set by environment
: ${CFLAGS="-O2 -Wall"}
: ${INSTALLFLAGS="-s"}

# where autotool scripts are: install-sh, config.sub, config.guess
AC_CONFIG_AUX_DIR(aux)

# Check CLIgen
if test "$prefix" = "NONE"; then
	CLIGEN_PREFIX="$ac_default_prefix"
	CLIXON_PREFIX="$ac_default_prefix"
else
	CLIGEN_PREFIX="$prefix"
	CLIXON_PREFIX="$prefix"
fi

AC_CHECK_LIB(m, main)

# AC_SUBST(var) makes @var@ appear in makefiles.
AC_SUBST(CLIXON_PREFIX)
AC_SUBST(CLIGEN_PREFIX)

AC_CANONICAL_TARGET

AC_SUBST(CC)
AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(INCLUDES)
AC_SUBST(CPPFLAGS)
AC_PROG_INSTALL
AC_SUBST(INSTALL)
AC_SUBST(INSTALL_DATA)
AC_SUBST(INSTALL_PROGRAM)
AC_SUBST(INSTALLFLAGS)
AC_SUBST(LIBS)
AC_SUBST(SH_SUFFIX)
# Used in expansion in, e.g., hello/src/hello.xml.in
AC_SUBST(SYSCONFDIR)
AC_SUBST(DATADIR)
AC_SUBST(LIBDIR)
AC_SUBST(LIBEXECDIR)
AC_SUBST(LOCALSTATEDIR)
AC_SUBST(SBINDIR)

#
AC_PROG_CC()
AC_PROG_CPP

CPPFLAGS="-DHAVE_CONFIG_H ${CPPFLAGS}"

AC_MSG_RESULT(compiler is $CC)	
# Debug flag
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug],[Build with debug symbols, default: no]),[
	  if test "$enableval" = no; then
	      ac_enable_debug=no
	  else	      
	      ac_enable_debug=yes
          fi
        ],
	[ ac_enable_debug=no])

AC_MSG_RESULT(debug is $ac_enable_debug)	
if test "$ac_enable_debug" = "yes"; then
   CFLAGS="-g -Wall"
   INSTALLFLAGS=""	
fi

AC_MSG_RESULT(CPPFLAGS is $CPPFLAGS)	
AC_MSG_RESULT(CFLAGS is $CFLAGS)
AC_MSG_RESULT(INSTALLFLAGS is $INSTALLFLAGS)	

if test "$prefix" = "NONE"; then
     prefix=${ac_default_prefix}
fi

# This is for cligen
AC_ARG_WITH(cligen,  [  --with-cligen=dir       Use CLIGEN here ] )
if test "${with_cligen}"; then
  echo "Using CLIgen here: ${with_cligen}"
  CPPFLAGS="-I${with_cligen}/include ${CPPFLAGS}"
  LDFLAGS="-L${with_cligen}/lib ${LDFLAGS}"
  test -d "$with_cligen" && CLIGEN_PREFIX="$with_cligen"
fi

# This is for clixon
AC_ARG_WITH(clixon,  [  --with-clixon=dir       Use Clixon here ] )
if test "${with_clixon}"; then
  echo "Using Clixon here: ${with_clixon}"
  CPPFLAGS="-I${with_clixon}/include ${CPPFLAGS}"
  LDFLAGS="-L${with_clixon}/lib ${LDFLAGS}"
  test -d "$with_clixon" && CLIXON_PREFIX="$with_clixon"
fi

# who we run as
CLICON_USER="clicon"
AC_ARG_WITH([clicon-user], [AS_HELP_STRING([--with-clicon-user=user], [Run as this user in configuration files])], [
  CLICON_USER="$withval"
])
AC_SUBST(CLICON_USER)
if test -n "${CLICON_USER}"; then
    echo "Using CLICON_USER here: ${CLICON_USER}"
fi

CLICON_GROUP="clicon"
AC_ARG_WITH([clicon-group], [AS_HELP_STRING([--with-clicon-group=group], [Run as this group in configuration files])], [
  CLICON_GROUP="$withval"
])
AC_SUBST(CLICON_GROUP)
if test -n "${CLICON_GROUP}"; then
    echo "Using CLICON_GROUP here: ${CLICON_GROUP}"
fi

AC_CHECK_HEADERS(cligen/cligen.h,, AC_MSG_ERROR(cligen missing. Try: git clone https://github.com/olofhagsand/cligen.git))

AC_CHECK_LIB(cligen, cligen_init,, AC_MSG_ERROR([CLIgen missing. Try: git clone https://github.com/olofhagsand/cligen.git]))

AC_CHECK_HEADERS(clixon/clixon.h,, AC_MSG_ERROR(Clixon missing. Try: git clone https://github.com/clicon/clixon.git), [[#include <signal.h>
#include <cligen/cligen.h>]])
AC_CHECK_LIB(clixon, clicon_log_init,, AC_MSG_ERROR([Clixon missing. Try: git clone https://github.com/clicon/clixon.git]),)

# This is where local yangs are installed
AC_ARG_WITH(yang-installdir,
            [  --with-yang-installdir=DIR  Install yang files here (default: ${prefix}/share/clixon) ],
	    [YANG_INSTALLDIR="$withval"],
    	    [YANG_INSTALLDIR="${prefix}/share/clixon"]		
	    )
AC_SUBST(YANG_INSTALLDIR)
AC_MSG_RESULT(Clixon yang files are installed in ${YANG_INSTALLDIR})	

# Dummy to disable native language support (nls) to remove warnings in buildroot
AC_ARG_ENABLE(nls)

# Expand for easy replacement in hello/src/hello.xml.in
# Special case is $libdir, which is composed from $exec_prefix
test "x$prefix" = xNONE && prefix=$ac_default_prefix
SYSCONFDIR=`eval echo $sysconfdir`
DATADIR=`eval echo $datadir`
LIBDIR=`eval echo $libdir`
LIBEXECDIR=`eval echo $libexecdir`
LOCALSTATEDIR=`eval echo $localstatedir`
SBINDIR=`eval echo $sbindir`

AC_CONFIG_FILES([Makefile
	  hello/Makefile
  	  hello/src/Makefile
  	  hello/src/hello.xml
    	  hello/docker/Makefile
  	  wifi/Makefile
    	  wifi/src/Makefile
    	  wifi/src/wifi.xml
    	  wifi/docker/Makefile
  	  openconfig/Makefile
  	  openconfig/src/Makefile
  	  openconfig/src/openconfig.xml
    	  openconfig/docker/Makefile
	  3gpp/Makefile
  	  3gpp/src/Makefile
  	  3gpp/src/3gpp.xml
	  3gpp/yang/Makefile
    	  3gpp/docker/Makefile
])
AC_OUTPUT
