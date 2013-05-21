/**@file macro.h
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
 */
#ifndef VARIABLE_H
#define VARIABLE_H

#include "config.h"
#include "parser.h"
#include "compiler.h"

// available variable count
#define MAX_VARIABLES               XVM_U32_MAX_VARS

// floating or fixed point type
#define VAR_VAL_TYPE_SC             0
#define VAR_VAL_TYPE_FP             1

typedef struct
{
    int8_t  name[MAX_CHARS_VAR_NAME + 1];   // variable name
    int32_t addr;                           // ram address offset
    bool    type;                           // type; floating or fixed
} TVarDefinition;




#ifdef __cplusplus
extern "C"{
#endif

    extern TVarDefinition var_def[MAX_VARIABLES];
    extern int8_t g_var_str[MAX_CHARS_VAR_STR + 1];
    extern int8_t g_var_name[MAX_CHARS_VAR_NAME + 1];

    extern int32_t is_var(int8_t const* str_start, int8_t const* str_end);
    extern void reset_var(void);
    extern bool add_var_def(void);
#if HAVE_FP_NUMBERS
    extern bool add_var_val(int8_t* str, int8_t* end, bool var_type);
#else
    extern bool add_var_val(int8_t* str, int8_t* end);
#endif
    extern int32_t find_var_def(int8_t* name);
    extern int32_t set_var_type(int8_t* name, bool type);
    extern void remove_def_var(int32_t index);
    extern void show_vars(void);

#ifdef __cplusplus
}
#endif


#endif // VARIABLE_H

// end of file
