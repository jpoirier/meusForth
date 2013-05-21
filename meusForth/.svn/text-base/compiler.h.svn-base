/**@file compiler.h
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
#ifndef COMPILER_H
#define COMPILER_H

#include "stack.h"

enum {
    // Used by the compiler to change between console and target mode
    COMP_CONSOLE            = 0,
    COMP_TARGET             = 1,
    // Used to set the compiler's stack mode
    NO_CURR_STACK           = -1,
    IF_STACK                = 0,
    LOOP_STACK              = 1,
    //
    MAX_MODE_STACK          = 500,
    MAX_CSP                 = 10000,
    MAX_CHARS_FILE_NAME     = 512,
    MAX_CHARS_MACRO_NAME    = 32,
    MAX_CHARS_MACRO_STR     = 32,
    MAX_CHARS_VAR_NAME      = 32,
    MAX_CHARS_VAR_STR       = 32,
    MAX_CHARS_TOKEN_NAME    = 33,
    MAX_CHARS_TOKEN_INFO    = 64,
    VOC_NAME_MAX_CHARS      = 32
};


#ifdef __cplusplus
extern "C"{
#endif

    extern I32Stack g_mode_stack;
    extern I32Stack m_loop_astack;
    extern PI8Stack m_loop_nstack;
    extern PI8Stack m_loop_fstack;
    extern I32Stack m_loop_lstack;
    extern I32Stack m_if_astack;
    extern PI8Stack m_if_nstack;
    extern PI8Stack m_if_fstack;
    extern I32Stack m_if_lstack;

    extern int32_t g_current_word_def_addr;
    extern int32_t g_comp_mode;
    extern bool g_can_execute;
    extern int32_t g_comp_wradr;
    extern uint8_t* g_comp_cp;
    extern int32_t g_err_cnt;

    extern int32_t mode_pop(void);
    extern void mode_push(int32_t mode);
    extern void set_comp_mode(int32_t mode);
    extern void add_byte(uint8_t val);
    extern void select_comp_stack(int32_t stack);
    extern void copy_con_to_target(void);
    extern int32_t addr_pop(void);
    extern int8_t* was_file_name(void);
    extern int32_t was_line(void);
    extern void add_target_header(void);
    extern int32_t compile_integer(int32_t val);
    extern int32_t compile_integer_str(int8_t* str, int8_t* end);
#if HAVE_FP_NUMBERS
    extern int32_t compile_float(uint32_t val);
    extern int32_t compile_fp_str(int8_t* str, int8_t* end);
#endif
    extern void compile_lgoto(uint32_t addr);
    extern void compile_call(uint32_t addr);
    extern void compile_lcall(int32_t index);
#if HAVE_FFI
    extern void compile_fficall(int32_t index);
#endif
    extern uint8_t get_next_byte(void);
    extern void compiler_error(void);
    extern void addr_push(int32_t data);
    extern void name_push(int8_t* name);
    extern int8_t* name_pop(void);
    extern int32_t find_token(int8_t* tok_name, int32_t cnt);
    extern int8_t* token_name(int32_t tok);
    extern int8_t* token_comment(int32_t tok);
    extern int32_t token_len(int32_t tok);
    extern void compiler_reset(void);
    extern void compiler_init(void);
    extern int32_t compile_word(int8_t const* start, int8_t const* end);
    extern void compilation_end(void);

#ifdef __cplusplus
}
#endif

#endif // COMPILER_H

// end of file

