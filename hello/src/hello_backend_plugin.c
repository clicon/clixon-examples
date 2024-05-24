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
 * If it exists, it contains the "to" value.
 */
#define WORLD_FILE "/tmp/world"

struct hello_data {
    char to[10];
    enum { HELLO_NOP, HELLO_ADD, HELLO_DEL } op;
};

/*
 * Allocate an integer for us to store the results of validation in.
 * That was we don't have to parse again in the commit.
 */
static int
hello_begin(clicon_handle h, transaction_data td) {
    struct hello_data *data;
    int rv;

    printf("*****hello begin*****\n");
    data = malloc(sizeof(*data));
    if (!data) {
	clixon_err(OE_XML, 0, "Could not allocate memory");
	return -1;
    }
    data->op = HELLO_NOP;
    memset(data->to, 0, sizeof(data->to));
    rv = transaction_arg_set(td, data);
    if (rv) {
	free(data);
	return rv;
    }
    return 0;
}

static int
hello_end(clicon_handle h, transaction_data td) {
    struct hello_data *data = transaction_arg(td);

    printf("*****hello end*****\n");
    free(data);
    return 0;
}

static const char *valid_tos[] = { "city", "state", "country", "world", NULL };

/*
 * Validate that the current XML object is of the form:
 *   <hello xmlns="urn:example:hello">
 *     <to>world</to>
 *   </hello>
 * Returns the contents of "to" in str.
 *
 * Return value is 0 for not found, 1 for found, and -1 for error.
 */
static int
find_hello_to(cxobj *vec, const char **str)
{
    bool to_found = false, ns_found = false;
    cxobj *c, *c2;
    int j, k;
    char *s;

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
	if (strcmp(xml_name(c), "to") != 0) {
	    clixon_err(OE_XML, 0, "Non-\"to\" in hello vec: %s",
		       xml_name(c));
	    return -1;
	}
	if (to_found) {
	    clixon_err(OE_XML, 0, "Multiple \"to\" in hello vec");
	    return -1;
	}

	c2 = xml_child_i(c, 0);
	if (!c2) {
	    clixon_err(OE_XML, 0, "The \"to\" element doesn't have data");
	    return -1;
	}

	if (strcmp(xml_name(c2), "body") != 0) {
	    clixon_err(OE_XML, 0, "The \"to\" element doesn't have a body");
	    return -1;
	}
	s = xml_value(c2);
	if (!s) {
	    clixon_err(OE_XML, 0, "The \"to\" element doesn't have a value");
	    return -1;
	}

	for (k = 0; valid_tos[k]; k++) {
	    if (strcmp(valid_tos[k], s) == 0)
		break;
	}
	if (!valid_tos[k]) {
	    clixon_err(OE_XML, 0, "Invalid \"to\" element: %s", s);
	    return -1;
	}

	*str = valid_tos[k];
	to_found = true;
    }

    if (ns_found && to_found)
	return 1;
    return 0;
}

static int
hello_validate(clicon_handle h, transaction_data td) {
    struct hello_data *data = transaction_arg(td);
    cxobj **vec;
    size_t i, len;
    int rv;

    printf("*****hello validate*****\n");
    transaction_print(stdout, td);

    vec = transaction_dvec(td);
    len = transaction_dlen(td);
    for (i = 0; i < len; i++) {
	const char *place;

	rv = find_hello_to(vec[i], &place);
	if (rv == -1)
	    return -1;
	if (!rv)
	    continue;
	strncpy(data->to, place, sizeof(data->to) - 1);
	data->op = HELLO_DEL;
    }

    vec = transaction_avec(td);
    len = transaction_alen(td);
    for (i = 0; i < len; i++) {
	const char *place;

	rv = find_hello_to(vec[i], &place);
	if (rv == -1)
	    return -1;
	if (!rv)
	    continue;
	strncpy(data->to, place, sizeof(data->to) - 1);
	data->op = HELLO_ADD;
    }

    /*
     * We do not look at the old data (_scvec), we just look at the
     * changed data.  The only thing that can change is what is in
     * "to", so we check the parent of "to" to verify it.
     */
    len = transaction_clen(td);
    vec = transaction_tcvec(td);
    for (i = 0; i < len; i++) {
	cxobj *p = xml_parent(vec[i]);
	const char *place;

	if (!p)
	    continue;

	rv = find_hello_to(p, &place);
	if (rv == -1)
	    return -1;
	if (!rv)
	    continue;
	strncpy(data->to, place, sizeof(data->to) - 1);
	data->op = HELLO_ADD;
    }

    return 0;
}

static int
hello_commit(clicon_handle h, transaction_data td) {
    struct hello_data *data = transaction_arg(td);
    FILE *f;
    int rv;

    printf("*****hello commit*****: %d\n", data->op);
    switch (data->op) {
    case HELLO_DEL:
	rv = remove(WORLD_FILE);
	if (rv < 0) {
	    clixon_err(OE_XML, 0, "Error deleting %s: %s",
		       WORLD_FILE, strerror(errno));
	    return -1;
	}
	break;

    case HELLO_ADD:
	f = fopen("/tmp/world", "w");
	if (!f) {
	    clixon_err(OE_XML, 0, "Error creating %s: %s",
		       WORLD_FILE, strerror(errno));
	    return -1;
	}
	fprintf(f, "%s", data->to);
	fclose(f);

    case HELLO_NOP:
	break;
    }
    return 0;
}

static int
hello_statedata(clixon_handle h, cvec *nsc, char *xpath, cxobj *xtop)
{
    int rv = -1;
    cxobj **xvec = NULL;
    FILE *f = fopen(WORLD_FILE, "r");
    char to[10], xmlstr[200];
    int k;

    printf("*****hello statedata*****: %p\n", f);

    if (f) {
	memset(to, 0, sizeof(to));
	k = fread(to, 1, 9, f);
	fclose(f);

	if (!k) {
	    clixon_err(OE_XML, 0, "Empty %s contents", WORLD_FILE);
	    goto done;
	}
	for (k = 0; valid_tos[k]; k++) {
	    if (strcmp(valid_tos[k], to) == 0)
		break;
	}
	if (!valid_tos[k]) {
	    clixon_err(OE_XML, 0, "Invalid %s contents: %s", WORLD_FILE, to);
	    goto done;
	}

	snprintf(xmlstr, sizeof(xmlstr),
		 "<hello xmlns=\"urn:example:hello\"><to>%s</to></hello>", to);
	if (clixon_xml_parse_string(xmlstr, YB_NONE, NULL, &xtop, NULL) < 0)
	    goto done;
    } else {
	if (clixon_xml_parse_string("", YB_NONE, NULL, &xtop, NULL) < 0)
	    goto done;
    }
    rv = 0;
 done:
    if (xvec)
        free(xvec);
    return rv;
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
