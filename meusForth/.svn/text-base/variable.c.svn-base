/**@file variable.c
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
#include "variable.h"
#include "xvm.h"
#include "xvm_inner.h"
#include "compiler.h"
#include "code.h"
#include "files.h"
#include "voc.h"
#include "debug.h"
#include "directives.h"

#ifdef ICC_WIN
    #define strtof (float) strtod
#endif

#if HAVE_FP_NUMBERS
    #include "float.h"
#endif

TVarDefinition var_def[MAX_VARIABLES];

int8_t g_var_name[MAX_CHARS_VAR_NAME + 1];
int8_t g_var_str[MAX_CHARS_VAR_STR + 1];

static int32_t var_cnt = 0;

/**
 *
 *
 * @param void
 * @return void
 */
void reset_var(void)
{
    var_cnt = 0;
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return
 */
int32_t is_var(int8_t const* str_start,
               int8_t const* str_end)
{
    int32_t len = str_end - str_start;

    assert(str_start);
    assert(str_end);

    if(len > MAX_CHARS_VAR_NAME)
        return -1;

    strncpy((char*) g_var_name,
            (char const*) str_start,
            len);

    g_var_name[len] = '\0';

    return find_var_def(g_var_name);
}

/**
 *
 *
 * @param void
 * @return
 */
bool add_var_def(void)
{
    bool status  = false;

    if(var_cnt < MAX_VARIABLES)
    {
        strncpy((char*)(var_def[var_cnt].name),
                (char const*) g_var_name,
                MAX_CHARS_VAR_NAME);

        var_def[var_cnt].addr = XVM_U32_MAX_RAM + var_cnt;

        var_cnt++;

        status = true;
    }
    else
    {
        printf("ERROR: too many variable definitions.\n");
        compiler_error();
    }

    return status;
}

/**
 *
 *
 * @param void
 * @return
 */
bool add_var_val(int8_t*    str,
                 int8_t*    end
#if HAVE_FP_NUMBERS
                ,
                 bool       var_type)
#else
                                )
#endif
{
    int32_t     addr        = 0;
    bool        status      = false;
    int8_t*     end_ptr     = str;
    int32_t     val         = 0;

#if HAVE_FP_NUMBERS
    float       val_fp      = 0;
#endif

    size_t      cnt         = end - str;
    int8_t*     this_str    = (int8_t*) alloca(cnt + 1);

    memcpy(this_str, str, cnt);

    this_str[cnt] = '\0';

    end_ptr = this_str;

    if((addr = find_var_def(g_var_name)) != -1)
    {
        errno = ~(ERANGE);

#if HAVE_FP_NUMBERS
        if(var_type == VAR_VAL_TYPE_FP)
        {
            val_fp = strtof((char*) this_str, (char**) &end_ptr);

            if(errno != ERANGE && val_fp <= FLT_MAX && val_fp >= FLT_MIN && end_ptr != this_str)
            {
                memcpy(&g_xvm_ram[addr], &val_fp, sizeof(float));

                set_var_type(g_var_name, VAR_VAL_TYPE_FP);

                status = true;
            }
            else
            {
                printf("ERROR: invalid floating point value defined for variable %s.\n", (char*) g_var_name);
                compiler_error();
                status = false;
            }
        }
        else // var_type == VAR_VAL_TYPE_SC
#endif
        {
            val = strtol((char*)  this_str, (char**) &end_ptr, 0);

            if(errno != ERANGE && val <= INT_MAX && val >= INT_MIN && end_ptr != this_str)
            {
                g_xvm_ram[addr] = val;

                set_var_type(g_var_name, VAR_VAL_TYPE_SC);

                status = true;
            }
            else
            {
                printf("ERROR: invalid fixed point value defined for variable %s.\n", (char*) g_var_name);
                compiler_error();
                status = false;
            }
        }
    }
    else
    {
        printf("ERROR: can't assign value; no variable name defined.\n");
        compiler_error();
        status = false;
    }

    return status;
}

/**
 *
 *
 * @param name
 * @return
 */
int32_t find_var_def(int8_t* name)
{
    int32_t i;

    assert(name);

    for(i = 0; i < var_cnt; i++)
    {
        if(strcmp((char*) name, (char*) (var_def[i].name)) == 0)
        {
            return (var_def[i].addr);
        }
    }

    return -1;
}

/**
 *
 *
 * @param name
 * @return
 */
int32_t set_var_type(int8_t* name,
                     bool   type)
{
    int32_t i;

    assert(name);

    for(i = 0; i < var_cnt; i++)
    {
        if(strcmp((char*) name, (char*) (var_def[i].name)) == 0)
        {
            var_def[i].type = type;

            return true;
        }
    }

    return -1;
}

/**
 *
 *
 * @param name
 * @return
 */
void remove_def_var(int32_t index)
{
    int32_t i;

    // scale the index value
    index = index - XVM_U32_MAX_RAM;

    // shift everything over by one starting at the index of the word being removed
    for(i = index; i < var_cnt; i++)
    {
        // moves the values
        g_xvm_vars[i] = g_xvm_ram[i + 1];

        // move the info structures
        var_def[i] = var_def[i + 1];
    }

    // decrement the count
    var_cnt -= 1;

    // this shouldn't happen...
    if(var_cnt < 0)
        var_cnt = 0;
}

/**
 *
 *
 * @param name
 * @return
 */
void show_vars(void)
{
    int32_t i;

    printf("\n---- Variables ----\n");

    if(var_cnt)
    {
        for(i = 0; i < var_cnt; i++)
        {
            if(!(i % 5))
                printf("\n");

            printf("%s ", (char*) var_def[i].name);
        }

        printf("\n\n");
    }
    else
        printf("\nNo variables defined...\n\n");

}

// end of file
