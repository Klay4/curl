/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2022, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include "curlcheck.h"

#include "curl/mprintf.h"

static CURLcode unit_setup(void) {return CURLE_OK;}
static void unit_stop(void) {}

UNITTEST_START

int rc;
char buf[3] = {'b', 'u', 'g'};
const char *str = "bug";
int width = 3;
char output[24];

/*#define curl_msnprintf snprintf */

/* without a trailing zero */
rc = curl_msnprintf(output, 4, "%.*s", width, buf);
fail_unless(rc == 3, "return code should be 3");
fail_unless(!strcmp(output, "bug"), "wrong output");

/* with a trailing zero */
rc = curl_msnprintf(output, 4, "%.*s", width, str);
fail_unless(rc == 3, "return code should be 3");
fail_unless(!strcmp(output, "bug"), "wrong output");

width = 2;
/* one byte less */
rc = curl_msnprintf(output, 4, "%.*s", width, buf);
fail_unless(rc == 2, "return code should be 2");
fail_unless(!strcmp(output, "bu"), "wrong output");

/* string with larger precision */
rc = curl_msnprintf(output, 8, "%.8s", str);
fail_unless(rc == 3, "return code should be 3");
fail_unless(!strcmp(output, "bug"), "wrong output");

/* longer string with precision */
rc = curl_msnprintf(output, 8, "%.3s", "0123456789");
fail_unless(rc == 3, "return code should be 3");
fail_unless(!strcmp(output, "012"), "wrong output");

/* negative width */
rc = curl_msnprintf(output, 8, "%-8s", str);
fail_unless(rc == 7, "return code should be 7");
fail_unless(!strcmp(output, "bug    "), "wrong output");

/* larger width that string length */
rc = curl_msnprintf(output, 8, "%8s", str);
fail_unless(rc == 7, "return code should be 7");
fail_unless(!strcmp(output, "     bu"), "wrong output");

/* output a number in a limited output */
rc = curl_msnprintf(output, 4, "%d", 10240);
fail_unless(rc == 3, "return code should be 3");
fail_unless(!strcmp(output, "102"), "wrong output");

/* padded strings */
rc = curl_msnprintf(output, 16, "%8s%8s", str, str);
fail_unless(rc == 15, "return code should be 15");
fail_unless(!strcmp(output, "     bug     bu"), "wrong output");

/* padded numbers */
rc = curl_msnprintf(output, 16, "%8d%8d", 1234, 5678);
fail_unless(rc == 15, "return code should be 15");
fail_unless(!strcmp(output, "    1234    567"), "wrong output");

UNITTEST_STOP
