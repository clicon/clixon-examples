/*
 *
  ***** BEGIN LICENSE BLOCK *****
 
  Copyright (C) 2021-2022 Olof Hagsand and Rubicon Communications, LLC(Netgate)

  This file is part of CLIXON.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  Alternatively, the contents of this file may be used under the terms of
  the GNU General Public License Version 3 or later (the "GPL"),
  in which case the provisions of the GPL are applicable instead
  of those above. If you wish to allow use of your version of this file only
  under the terms of the GPL, and not to allow others to
  use your version of this file under the terms of Apache License version 2, indicate
  your decision by deleting the provisions above and replace them with the 
  notice and other provisions required by the GPL. If you do not delete
  the provisions above, a recipient may use your version of this file under
  the terms of any one of the Apache License version 2 or the GPL.

  ***** END LICENSE BLOCK *****
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdbool.h>

/* clicon */
#include <cligen/cligen.h>

/* Clicon library functions. */
#include <clixon/clixon.h>

/* These include signatures for plugin and transaction callbacks. */
#include <clixon/clixon_backend.h> 

/*
 * This file reflects the state of hello world.  If it exists, hello
 * world is set.  If it does not exit, then hello world is not set.
 */
#define WORLD_FILE "/tmp/world"

/*
 * Allocate an integer for us to store the results of validation in.
 * That was we don't have to parse again in the commit.
 */
static int
hello_begin(clicon_handle h, transaction_data td) {
    int *data;
    int rv;

    printf("*****hello begin*****\n");
    data = malloc(sizeof(*data));
    if (!data) {
	clixon_err(OE_XML, 0, "Could not allocate memory");
	return -1;
    }
    *data = -1;
    rv = transaction_arg_set(td, data);
    if (rv) {
	free(data);
	return rv;
    }
    return 0;
}

static int
hello_end(clicon_handle h, transaction_data td) {
    int *data = transaction_arg(td);

    printf("*****hello end*****\n");
    free(data);
    return 0;
}

/*
 * Validate that the current XML object is of the form:
 *   <hello xmlns="urn:example:hello">
 *     <world/>
 *   </hello>
 */
static int
find_hello_world(cxobj *vec)
{
    bool world_found = false, ns_found = false;
    cxobj *c;
    int j;

    if (strcmp(xml_name(vec), "hello") != 0)
	return 0;
    /*
     * It would be nice to be able to use nscache_xxx() for this.
     * However, it's not always set for some reason.  The xmlns entry
     * is always in the children, though, so use that.
     */
    for (j = 0; (c = xml_child_i(vec, j)); j++) {
	if (strcmp(xml_name(c), "xmlns") == 0) {
	    char *ns;

	    if (ns_found) {
		clixon_err(OE_XML, 0, "Multiple xmlns in hello");
		return -1;
	    }
	    ns = xml_value(c);
	    if (!ns) {
		clixon_err(OE_XML, 0, "xmlns has no value");
		return -1;
	    }
	    if (strcmp(ns, "urn:example:hello") != 0)
		return 0;
	    ns_found = true;
	    continue;
	}
	if (strcmp(xml_name(c), "world") != 0) {
	    clixon_err(OE_XML, 0, "Non-\"world\" in hello vec: %s",
		       xml_name(c));
	    return -1;
	}
	if (world_found) {
	    clixon_err(OE_XML, 0, "Multiple \"world\" in hello vec");
	    return -1;
	}

	world_found = true;
    }

    if (ns_found && world_found)
	return 1;
    return 0;
}

static int
hello_validate(clicon_handle h, transaction_data td) {
    int *data = transaction_arg(td);
    cxobj **vec;
    size_t i, len;

    printf("*****hello validate*****\n");
    transaction_print(stdout, td);

    vec = transaction_dvec(td);
    len = transaction_dlen(td);
    for (i = 0; i < len; i++) {
	switch (find_hello_world(vec[i])) {
	case 0:
	    break;
	case 1:
	    *data = 0;
	    break;
	default:
	    return -1;
	}
    }

    vec = transaction_avec(td);
    len = transaction_alen(td);
    for (i = 0; i < len; i++) {
	switch (find_hello_world(vec[i])) {
	case 0:
	    break;
	case 1:
	    *data = 1;
	    break;
	default:
	    return -1;
	}
    }

    return 0;
}

static int
hello_commit(clicon_handle h, transaction_data td) {
    int *data = transaction_arg(td);
    FILE *f;
    int rv;

    printf("*****hello commit*****: %d\n", *data);
    switch (*data) {
    case 0:
	rv = remove(WORLD_FILE);
	if (rv < 0) {
	    clixon_err(OE_XML, 0, "Error deleting %s: %s",
		       WORLD_FILE, strerror(errno));
	    return -1;
	}
	break;

    case 1:
	f = fopen("/tmp/world", "w");
	if (!f) {
	    clixon_err(OE_XML, 0, "Error creating %s: %s",
		       WORLD_FILE, strerror(errno));
	    return -1;
	}
	fclose(f);

    default:
	break;
    }
    return 0;
}

static int
hello_statedata(clixon_handle h, cvec *nsc, char *xpath, cxobj *xtop)
{
    int     retval = -1;
    cxobj **xvec = NULL;
    FILE *f = fopen(WORLD_FILE, "r");

    printf("*****hello statedata*****\n");

    if (f) {
	fclose(f);
	if (clixon_xml_parse_string("<hello xmlns=\"urn:example:hello\">"
				    "<world/>"
				    "</hello>", YB_NONE, NULL, &xtop, NULL) < 0)
	    goto done;
    } else {
	if (clixon_xml_parse_string("", YB_NONE, NULL, &xtop, NULL) < 0)
	    goto done;
    }
    retval = 0;
 done:
    if (xvec)
        free(xvec);
    return retval;
}

static clixon_plugin_api api = {
    .ca_name = "hello backend",
    .ca_init = clixon_plugin_init,
    .ca_statedata = hello_statedata,
    .ca_trans_begin = hello_begin,
    .ca_trans_end = hello_end,
    .ca_trans_commit = hello_commit,
    .ca_trans_validate = hello_validate,
};

clixon_plugin_api *
clixon_plugin_init(clicon_handle h) {
    printf("*****hello init*****\n");
    return &api;
}
