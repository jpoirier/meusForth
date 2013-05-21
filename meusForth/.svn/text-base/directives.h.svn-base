/**@file directives.h
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
#ifndef DIRECTUVES_H
#define DIRECTUVES_H

#define DIRECTIVES_CNT      (60)

typedef struct directive_info
{
    int8_t* name;
    int8_t* info;
} directive_info;

#ifdef __cplusplus
extern "C"{
#endif

    extern int32_t find_std_word(int8_t const* w_name);

    extern void d_help(int8_t* str);
    extern void d_helpall(void);

    extern void d_if(void);
    extern void d_else(void);
    extern void d_then(void);

    extern void d_remove_word(int8_t* start, int8_t* end);

    extern void d_begin(void);
    extern void d_until(void);

    extern void d_do(void);
    extern void d_loop(void);
    extern void d_i(void);
    extern void d_j(void);
    extern void d_leave(void);

    extern void add_worddef_cmt(int8_t* cmt_start, int8_t* cmt_end);
    extern void d_word_def(int8_t* str_start, int8_t* str_end);
    extern void d_word_def_end(void);
    extern bool d_include(int8_t* str_start, int8_t* str_end);
    extern bool d_bin_file_run(int8_t* str_start, int8_t* str_end);
#if HAVE_PRINTF
    extern int32_t d_str_def(int8_t* str_start, int8_t* str_end);
#endif
    extern void d_words(void);
    extern void d_variables(void);
    extern void d_vocs(void);
    extern bool d_newvoc(int8_t* str_start, int8_t* str_end);
    extern void d_also(void);
    extern void d_only(void);
    extern void d_order(void);
    extern void d_vocdef(void);

    extern void d_nodebug(void);
    extern void d_debug(void);
    extern void d_step(void);
    extern void d_nostep(void);
    extern void d_showstackon(void);
    extern void d_showstackoff(void);
    extern void d_run(void);
    extern void d_base(void);
    extern void d_hex(void);
    extern void d_decimal(void);

    extern void d_recurse(void);

    extern bool d_compa(int8_t* str_start, int8_t* str_end);
    extern bool d_compb(int8_t* str_start, int8_t* str_end);
    extern bool d_macro_name(int8_t* str_start, int8_t* str_end);
    extern bool d_macro_value(int8_t* str_start, int8_t* str_end, bool val_type);
    extern bool d_var_name(int8_t* str_start, int8_t* str_end);
    extern void d_reset(void);

#ifdef __cplusplus
}
#endif

#endif // DIRECTUVES_H

// wnd of file
