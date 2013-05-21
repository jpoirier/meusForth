/**@file macro.c
 *
 * @author Original author: Joseph D Poirier
 * @date   Creation date  : 16 August 2008
 *
 *
 * @version X.X.X
 *
 * VERSION    DATE/AUTHOR              COMMENT
 *
 *
 *
 * Copyright 2008 Joseph D Poirier. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied.
 *
 * @cmd<<%PRJ% file=%PFE%>>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#include "types.h"
#include "config.h"
#include "macro.h"
#include "xvm.h"
#include "xvm_inner.h"
#include "compiler.h"
#include "code.h"
#include "files.h"
#include "voc.h"
#include "debug.h"
#include "directives.h"


TMacroDefinition mac_def[MAX_MACRO];

int8_t g_macro_name[MAX_CHARS_MACRO_NAME + 1];
int8_t g_macro_str[MAX_CHARS_MACRO_STR + 1];

static int32_t mac_cnt = 0;

/**
 *
 *
 * @param void
 * @return void
 */
void reset_macro(void)
{
    mac_cnt = 0;
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return
 */
int32_t is_macro(int8_t const* str_start,
                 int8_t const* str_end)
{
    int32_t len = str_end - str_start;

    assert(str_start);
    assert(str_end);

    if(len > MAX_CHARS_MACRO_NAME)
    {
        return -1;
    }

    strncpy((char*) g_macro_name,
            (char const*) str_start,
            len);

    g_macro_name[len] = '\0';

    return find_macro_def(g_macro_name);
}

/**
 *
 *
 * @param void
 * @return
 */
bool add_macro_def(bool val_type)
{
    bool status  = false;

    if(mac_cnt < MAX_MACRO)
    {
        strncpy((char*)(mac_def[mac_cnt].str),
                (char const*) g_macro_str,
                MAX_CHARS_MACRO_STR);

        strncpy((char*)(mac_def[mac_cnt].name),
                (char const*) g_macro_name,
                MAX_CHARS_MACRO_NAME);

        mac_def[mac_cnt].type = val_type;

        mac_cnt++;

        status = true;
    }
    else
    {
        printf("ERROR: too many macro definitions.\n");
        compiler_error();
    }

    return status;
}

/**
 *
 *
 * @param name
 * @return
 */
int32_t find_macro_def(int8_t* name)
{
    int32_t i;

    assert(name);

    for(i = 0; i < mac_cnt; i++)
    {
        if(strcmp((char*) name, (char*) (mac_def[i].name)) == 0)
            return i;
    }

    return -1;
}

// end of file
