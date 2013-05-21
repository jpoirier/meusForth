/**@file compiler.c
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
#include "parser.h"
#include "xvm.h"
#include "xvm_inner.h"
#include "code.h"
#include "compiler.h"
#include "files.h"
#include "voc.h"
#include "token_info.h"
#include "directives.h"
#include "debug.h"
#include "macro.h"
#include "variable.h"
#include "ffi.h"

#ifdef ICC_WIN
    #define strtof (float) strtod
#endif

#if HAVE_FP_NUMBERS
    #include "float.h"
#endif

static void init_comp_stack(void);

int32_t             g_current_word_def_addr  = -1;

//------------------------------------------------
int32_t             g_comp_mode         = COMP_CONSOLE;
uint8_t*            g_comp_cp           = 0;            // points to the current array
bool                g_can_execute       = true;
int32_t             g_comp_wradr        = 0;            // current write address within array
static uint32_t     m_max_comp_wradr    = 0;

//------------------------------------------------
I32Stack            g_mode_stack        = 0;
int32_t             g_err_cnt           = 0;

//------------------------------------------------
// LOOP stack items
I32Stack            m_loop_astack       = 0;
PI8Stack            m_loop_nstack       = 0;
PI8Stack            m_loop_fstack       = 0;
I32Stack            m_loop_lstack       = 0;

//------------------------------------------------
// IF-ELSE-THEN stack items
I32Stack            m_if_astack         = 0;
PI8Stack            m_if_nstack         = 0;
PI8Stack            m_if_fstack         = 0;
I32Stack            m_if_lstack         = 0;

//------------------------------------------------
// Holds the current stack state
static I32Stack     m_astack            = 0;
static PI8Stack     m_nstack            = 0;
static PI8Stack     m_fstack            = 0;
static I32Stack     m_lstack            = 0;

//------------------------------------------------
static int32_t      m_curr_stack        = IF_STACK;

/**
 * Caches the current mode then sets the system the requested mode.
 *
 * @param mode the requested mode
 * @return void
 */
void set_comp_mode(int32_t mode)
{
/*
    current mode        desired mode
    --------------------------------
    target              target
    target              console

    console             console
    console             target

*/
    // Check the current compiler mode and save the write address
    // to the appropriate variable based on the mode.
    switch(g_comp_mode)
    {
        case COMP_TARGET:
            g_xvm_code_wradr        = g_comp_wradr;
            break;
        default: // COMP_CONSOLE
            g_console_code_wradr    = g_comp_wradr;
            break;
    }

    // Set the following items based on the mode.
    switch(mode)
    {
        case COMP_TARGET:
            m_max_comp_wradr    = XVM_U8_MAX_CODE;
            g_comp_wradr        = g_xvm_code_wradr;
            g_comp_cp           = g_xvm_code;
            break;
        default: // COMP_CONSOLE
            m_max_comp_wradr    = MAX_CONSOLE_ADDR;
            g_comp_wradr        = g_console_code_wradr;
            g_comp_cp           = g_console_code;
            break;
    }

    // Now switch to the desired mode.
    g_comp_mode = mode;
}

/**
 * Adds a byte to the code stream.
 *
 * @param val the byte to added to the code stream
 * @return void
 */
void add_byte(uint8_t val)
{
    if(g_comp_wradr < m_max_comp_wradr)
    {
        *(g_comp_cp + g_comp_wradr) = val;

        g_comp_wradr += 1;
    }
    else
    {
        switch(g_comp_mode)
        {
            case COMP_TARGET:
                printf("ERROR: target code address limit reached.\n");
                compiler_error();
                break;
            default:
                printf("ERROR: console code address limit reached.\n");
                compiler_error();
                break;
        }
    }
}

/**
 * Prints out the user specified error string, and in the case of a
 * file the file specific information
 *
 * @param err the error string to print
 * @return void
 */
void compiler_error(void)
{
    TFileInfo* file_info = 0;

    file_info = (TFileInfo*) m_file_stack->peek(m_file_stack);

    if(file_info->fp)
        printf("file %s, line %d, position %d\n",
               (char const*) file_info->pfe,
               file_info->line,
               file_info->pos);

    g_err_cnt++;
}

/**
 *
 *
 * @param stack the requested stack
 * @return void
 */
void init_comp_stack(void)
{
    g_mode_stack->reset(g_mode_stack);
    mode_push(IF_STACK);

    m_curr_stack    = IF_STACK;
    m_astack        = m_if_astack;
    m_lstack        = m_if_lstack;
    m_nstack        = m_if_nstack;
    m_fstack        = m_if_fstack;
}

/**
 * Caches the current stack then switches to the requested stack.
 *
 * @param stack the requested stack
 * @return void
 */
void select_comp_stack(int32_t stack)
{
//    if(m_curr_stack != stack)
//    {
//        DPRINTF("m_curr_stack != stack\n");
//        mode_push(m_curr_stack);
//    }

    mode_push(m_curr_stack);

    if(stack == IF_STACK)
    {
        DPRINTF("stack == IF_STACK\n");

        m_astack  = m_if_astack;
        m_lstack  = m_if_lstack;
        m_nstack  = m_if_nstack;
        m_fstack  = m_if_fstack;
    }
    else // LOOP_STACK
    {
        DPRINTF("stack == LOOP_STACK\n");

        m_astack  = m_loop_astack;
        m_lstack  = m_loop_lstack;
        m_nstack  = m_loop_nstack;
        m_fstack  = m_loop_fstack;
    }

    m_curr_stack = stack;
}

/**
 * Pushes a mode onto its stack.
 *
 * @param mode the mode to be pushed on to the stack
 * @return succes or failure
 */
void mode_push(int32_t mode)
{
    g_mode_stack->push(g_mode_stack, mode);
}

/**
 * Pops a mode from its stack.
 *
 * @param void
 * @return the mode or failure (-1)
 */
int32_t mode_pop(void)
{
    return g_mode_stack->pop(g_mode_stack);
}

/**
 * Pushes data onto its stack.
 *
 * @param data
 * @return void
 */
void addr_push(int32_t data)
{
    m_astack->push(m_astack, data);
}

/**
 * Pops the address from its stack.
 *
 * @param void
 * @return the address or -1 if stack is empty
 */
int32_t addr_pop(void)
{
    return (m_astack->pop(m_astack));
}

/**
 *
 *
 * @param name
 * @return void
 */
void name_push(int8_t* name)
{
// XXX: 4OCT08 this code isn't really doing anything but wasn't commented out
//    TFileInfo*  file_info   = 0;

//    file_info = (TFileInfo*) m_file_stack->peek(m_file_stack);

//printf("pushing name: %s\n", name);
//printf("file        : %s\n", file_info->pfe);
//printf("line        : %d\n", file_info->line);

    m_nstack->push(m_nstack, name);

// XXX: bug?? since we probably don't want to push on
//      to the stack what's already there
//    m_fstack->push(m_fstack, file_info->pfe);
//    m_lstack->push(m_lstack, file_info->line);
}

/**
 *
 *
 * @param void
 * @return popped item from the name stack
 */
int8_t* name_pop(void)
{
    int8_t* str = m_nstack->pop(m_nstack);

    return str;
}

/**
 *
 *
 * @param void
 * @return popped item from the file stack
 */
int8_t* was_file_name(void)
{
    return (m_fstack->pop(m_fstack));
}

/**
 *
 *
 * @param void
 * @return popped item from the line stack
 */
int32_t was_line(void)
{
    return (m_lstack->pop(m_lstack));
}

/**
 *
 *
 * @param tok_name the token to search for
 * @param cnt the byte count of the token being searched for
 * @return
 */
int32_t find_token(int8_t* tok_name,
                   int32_t cnt)
{
    int32_t i = 0;

    for(i = 0; i < MAX_TOKEN_NO; i++)
    {
        if(g_xvm_token_tab[i].len == 0)
            break;

        if(!strncmp((char const*) tok_name, (char const*)  (g_xvm_token_tab[i].name), cnt))
            return i;
    }

    return -1;
}

/**
 *
 *
 * @param tok the structure index of the desired token
 * @return a token name or the flag MAX_TOKEN_NO indicating the token wasn't found
 */
int8_t* token_name(int32_t tok)
{
    if((tok < 0) || (tok > MAX_TOKEN_NO))
        tok = MAX_TOKEN_NO;

    return g_xvm_token_tab[tok].name;
}

/**
 *
 *
 * @param tok the structure index of the desired token
 * @return token information or the flag MAX_TOKEN_NO indicating the token wasn't found
 */
int8_t* token_comment(int32_t tok)
{
    if((tok < 0) || (tok > MAX_TOKEN_NO))
        tok = MAX_TOKEN_NO;

    return (int8_t*) (g_xvm_token_tab[tok].info);
}

/**
 *
 *
 * @param tok the structure index of the desired token
 * @return token length or the flag MAX_TOKEN_NO indicating the token wasn't found
 */
int32_t token_len(int32_t tok)
{
    if((tok < 0) || (tok > MAX_TOKEN_NO))
        tok = MAX_TOKEN_NO;

    return g_xvm_token_tab[tok].len;
}

/**
 *
 *
 * @param void
 * @return void
 */
void add_target_header(void)
{
    set_comp_mode(COMP_TARGET);

    add_byte(TOKEN_LGOTO);
    add_byte(TOKEN_NOOP);
    add_byte(TOKEN_NOOP);

    set_comp_mode(COMP_CONSOLE);
}

/**
 *
 *
 * @param void
 * @return void
 */
void compiler_reset(void)
{
    close_all_files();

    set_comp_mode(COMP_CONSOLE);

    g_console_code_rdadr    = 0;
    g_console_code_wradr    = 0;
    g_comp_wradr            = 0;


    set_comp_mode(COMP_CONSOLE);

    g_err_cnt               = 0;

    init_comp_stack();
}

/**
 *
 *
 * @param void
 * @return void
 */
void compiler_init(void)
{

    m_file_stack    = CONSTRUCT_VA(PVOIDStack, construct, MAX_FILE_STACK);
    g_mode_stack    = CONSTRUCT_VA(I32Stack, construct, MAX_MODE_STACK);

    m_loop_astack   = CONSTRUCT_VA(I32Stack, construct, MAX_CSP);
    m_loop_nstack   = CONSTRUCT_VA(PI8Stack, construct, MAX_CSP);
    m_loop_fstack   = CONSTRUCT_VA(PI8Stack, construct, MAX_CSP);
    m_loop_lstack   = CONSTRUCT_VA(I32Stack, construct, MAX_CSP);

    m_if_astack     = CONSTRUCT_VA(I32Stack, construct, MAX_CSP);
    m_if_nstack     = CONSTRUCT_VA(PI8Stack, construct, MAX_CSP);
    m_if_fstack     = CONSTRUCT_VA(PI8Stack, construct, MAX_CSP);
    m_if_lstack     = CONSTRUCT_VA(I32Stack, construct, MAX_CSP);

    g_num_base              = 10;
    g_comp_wradr            = 0;

    g_xvm_code_wradr        = 0;
    g_xvm_pc                = 0;
    g_console_code_rdadr    = 0;
    g_console_code_wradr    = 0;

    set_comp_mode(COMP_CONSOLE);

#if HAVE_FP_NUMBERS
    #ifdef CONSOLE_MODE
        memset(shadow_g_xvm_stack, 0, sizeof(uint32_t) * XVM_U32_MAX_STACK);
    #endif
#endif

    compiler_reset();
    xvm_reset();
    reset_vocs();
    reset_macro();
    reset_parser();
    add_target_header();
}

/**
 *
 *
 * @param val
 * @return the compiled byte count

    --t;
    while(++t < l)
    {
        num = num * radix + (*t - '0');
    }

 */
int32_t compile_integer(int32_t val)
{
    uint32_t    tmp             = 0;
    int32_t     bytes_compiled  = 0;

    if(val == -1)
    {
        // 1-byte opcode
        add_byte((uint8_t) TOKEN_LIT_MINUS1);

        bytes_compiled = 1;
    }
    else if(val >= 0 && val <= 15)
    {
        // 1-byte opcode
        // We want the token representation value. The "literal" tokens
        // start at TOKEN_LIT0 and increment by a value of 1.
        add_byte((uint8_t) (val + TOKEN_LIT0));

        bytes_compiled = 1;
    }
    else if(val >= 16 && val <= 255)
    {
        // 2-bytes: 1 byte for the opcode and 1 byte for the data
        add_byte((uint8_t) TOKEN_LIT);
        add_byte((uint8_t) val);

        bytes_compiled = 2;
    }
    else
    {
        // 5-bytes: 1 byte for the opcode and 4 bytes for the data
        add_byte((uint8_t) TOKEN_LLIT);

        tmp = val;

        // 32 bit value: 4, 8-bit, bytes
        add_byte((uint8_t) ((tmp >> 24) & 0xFF));
        add_byte((uint8_t) ((tmp >> 16) & 0xFF));
        add_byte((uint8_t) ((tmp >>  8) & 0xFF));
        add_byte((uint8_t) (tmp & 0xFF));

        bytes_compiled = 5;
    }

    return bytes_compiled;
}

#if HAVE_FP_NUMBERS
    /**
     *
     *
     * @param val
     * @return the compiled byte count
     */
    int32_t compile_float(uint32_t val)
    {
        uint32_t    tmp             = 0;
        int32_t     bytes_compiled  = 5;

        // 5-bytes: 1 byte for the opcode and 4 bytes for the data
        add_byte((uint8_t) TOKEN_LLITFP);

        tmp = val;

        // 32 bit value: 4, 8-bit, bytes
        add_byte((uint8_t) ((tmp >> 24) & 0xFF));
        add_byte((uint8_t) ((tmp >> 16) & 0xFF));
        add_byte((uint8_t) ((tmp >>  8) & 0xFF));
        add_byte((uint8_t) (tmp & 0xFF));

        return bytes_compiled;
    }

    /**
     *
     *
     * @param val
     * @return the compiled byte count

        --t;
        while(++t < l)
        {
            num = num * radix + (*t - '0');
        }

     */
    int32_t compile_fp_str(int8_t* str,
                           int8_t* end)
    {
        int32_t     bytes_compiled  = 0;
        int8_t*     end_ptr         = 0;
        float       val             = 0;
        uint32_t    tmp             = 0;
        size_t      cnt             = end - str;
        int8_t*     this_str        = (int8_t*) alloca(cnt + 1);

        memcpy(this_str, str, cnt);
        this_str[cnt] = '\0';

        end_ptr = this_str;

        errno = ~(ERANGE);

        val = strtof((char*) this_str, (char**) &end_ptr);

    //printf("float: %f\n", val);

        if(errno != ERANGE && val <= FLT_MAX && val >= FLT_MIN && end_ptr != this_str)
        {
            // 5-bytes: 1 byte for the opcode and 4 bytes for the data
            add_byte((uint8_t) TOKEN_LLITFP);

            memcpy(&tmp, &val, sizeof(float));

            // 32 bit vale = 4, 8-bit, bytes
            add_byte((uint8_t) ((tmp >> 24) & 0xFF));
            add_byte((uint8_t) ((tmp >> 16) & 0xFF));
            add_byte((uint8_t) ((tmp >>  8) & 0xFF));
            add_byte((uint8_t) (tmp & 0xFF));

            bytes_compiled = 5;
        }

        return bytes_compiled;
    }
#endif // HAVE_FP_NUMBERS

/**
 *
 *
 * @param val
 * @return the compiled byte count

    --t;
    while(++t < l)
    {
        num = num * radix + (*t - '0');
    }

 */
int32_t compile_integer_str(int8_t* str,
                            int8_t* end)
{
    int8_t*     end_ptr         = str;
    int32_t     val             = 0;
    int32_t     bytes_compiled  = 0;
    size_t      cnt             = end - str;
    int8_t*     this_str        = (int8_t*) alloca(cnt + 1);

    memcpy(this_str, str, cnt);
    this_str[cnt] = '\0';

    end_ptr = this_str;

    errno = ~(ERANGE);

    val = strtol((char*)  this_str, (char**) &end_ptr, 0);

//    printf("val: %d\n", val);
//    if(errno == ERANGE) printf(" value == ERANGE");
//    if(val > INT_MAX) printf(" value > INT_MAX");
//    if(val < INT_MIN) printf(" value < INT_MIN");
//    if(end_ptr == str) printf(" end_ptr == str");

    if(errno != ERANGE && val <= INT_MAX && val >= INT_MIN && end_ptr != this_str)
    {
        bytes_compiled = compile_integer(val);
    }

    DPRINTF_VA("bytes_compiled: %d\n", bytes_compiled);

    return bytes_compiled;
}

/**
 *
 *
 * @param addr
 * @return void
 */
void compile_call(uint32_t addr)
{
    // 5-bytes: 1 byte for the opcode and 4 bytes for the data
    add_byte((uint8_t) TOKEN_LLIT);

    // and now the address
    add_byte((uint8_t) ((addr >> 24) & 0xFF));
    add_byte((uint8_t) ((addr >> 16) & 0xFF));
    add_byte((uint8_t) ((addr >>  8) & 0xFF));
    add_byte((uint8_t) (addr & 0xFF));

    // make the call
    add_byte(TOKEN_CALL);
}

/**
 *
 *
 * @param addr
 * @return void
 */
void compile_lgoto(uint32_t addr)
{
    // 3-bytes: 1 byte for the opcode and 2 bytes for the data
    add_byte(TOKEN_LGOTO);
    add_byte((uint8_t) (addr >> 8));    // upper byte
    add_byte((uint8_t) (addr & 0xFF));  // lower byte
}

/**
 *
 *
 * @param index
 * @return void
 */
void compile_lcall(int32_t index)
{
    uint32_t addr   = 0;

    // 3-bytes: 1 byte for the opcode and 2 bytes for the data
    add_byte(TOKEN_LCALL);

    addr = get_def_addr(index);

    add_byte((uint8_t) (addr >> 8));    // upper byte
    add_byte((uint8_t) (addr & 0xFF));  // lower byte
}

#if HAVE_FFI
    /**
     *
     *
     * @param index
     * @return void
     */
    void compile_fficall(int32_t index)
    {
        // 3-bytes: 1 byte for the opcode and 2 bytes for the data
        add_byte(TOKEN_FFICALL);

        add_byte((uint8_t) (index >> 8));    // upper byte
        add_byte((uint8_t) (index & 0xFF));  // lower byte
    }
#endif
/**
 *
 *
 * @param start
 * @param end
 * @return
 */
int32_t compile_word(int8_t const* start,
                     int8_t const* end)
{
    int32_t index   = 0;
    size_t  cnt     = end - start;
    int8_t* word    = 0;

    if(FFI_KEY_SZ > cnt)
        word = (int8_t*) calloc(FFI_KEY_SZ, sizeof(int8_t));
    else
        word = (int8_t*) calloc(cnt, sizeof(int8_t));

    // Check order:
    //  - macro
    //  - variable
    //  - word
    //  - ffi
    index = is_macro(start, end);

    //---- is it a macro?
    if(index >= 0)
    {
        index = compile_integer_str((int8_t*) mac_def[index].str,
                                    (int8_t*) mac_def[index].str + (MAX_CHARS_MACRO_STR - 1));
    }
    else
    {
        index = is_var(start, end);

        //---- is it a variable?
        if(index >= 0)
        {
            assert(index < (XVM_U32_MAX_RAM + XVM_U32_MAX_VARS));

            // put the address on the stack
            compile_integer(index);
        }
        else
        {
            strncpy((char*) word, (char const*) start, cnt);
            word[cnt] = '\0';

            // compile a user-defined word, returns -1 if word not found
            index = find_def_word(word);

            //---- is it a word definition?
            if(index >= 0)
            {
                compile_lcall(index);
            }
            else
            {
#if HAVE_FFI
// XXX: add a check to make sure the index isn't larger than the max function cnt
                index = ffi_find_func(word);

                //---- is it a FFI call?
                if(index >= 0)
                {
                    compile_fficall(index);
                }
                else
#endif // HAVE_FFI
                {
                    printf("\t'%s'\n", (char*) word);
                    printf("ERROR: unknown token.\n");
                    compiler_error();
                }
            }
        }
    }

    free(word);

    return index;
}

/**
 * Copy the console buffer's compiled tokens to the target
 *
 * @param void
 * @return void
 */
void copy_con_to_target(void)
{
    int32_t     i                   = 0;
    uint32_t    last_target_wradr   = 0;

    set_comp_mode(COMP_CONSOLE);

    // This is the ned of the byte stream and we want to be
    // kicked out of the interpreter loop so we inject an invalid opcode.
//printf(" adding: TOKEN_ILLEGAL\n");
    add_byte((uint8_t) TOKEN_ILLEGAL);
    set_comp_mode(COMP_TARGET);

    last_target_wradr = g_xvm_code_wradr;

    for(i = 0; i < g_console_code_wradr; i++)
        add_byte(g_console_code[i]);

    // g_xvm_code[0] is _LGOTO token
    g_xvm_code[1] = (uint8_t) (last_target_wradr >> 8);
    g_xvm_code[2] = (uint8_t) (last_target_wradr & 0xFF);

    set_comp_mode(COMP_CONSOLE);

    g_xvm_code_wradr = last_target_wradr;
}

/**
 *
 *
 * @param void
 * @return void
 */
void compilation_end(void)
{
    int8_t* file_name   = 0;
    int8_t* tmp         = 0;
    int32_t line_num    = 0;

    while(true)
    {
        tmp = name_pop();

        if(tmp)
        {
            printf("ERROR, there were unmatched statements:\n");
            printf("       '%s', ", (char*) tmp);

            file_name   = was_file_name();
            line_num    = was_line();

            free(tmp);

            printf("file: %s, line: %d \n", (char*) file_name, line_num);
        }
        else
            break;
    }

    copy_con_to_target();
    reset_parser();
}

// end of file

