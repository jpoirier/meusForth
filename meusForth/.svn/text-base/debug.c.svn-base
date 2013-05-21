/**@file debug.c
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

#include "types.h"
#include "config.h"
#include "xvm.h"
#include "xvm_inner.h"
#include "parser.h"
#include "compiler.h"
#include "code.h"
#include "files.h"
#include "voc.h"
#include "macro.h"
#include "token_info.h"
#include "debug.h"

#if HAVE_FP_NUMBERS
    // Rather than having a separate stack for floating point numbers this stack
    // keeps the state, floating point or not floating point, of individual
    // items (cells) that reside on the data stack. Cells are sanity checked
    // while in console mode.
    // Since the cells of the data stack are scalar memcpy is used to cope with
    // storage issues that exist bewteen the two dis-similar data types.
    // Currently only floats are supported.
    int32_t     shadow_g_xvm_stack[XVM_U32_MAX_STACK];     // data stack
#endif

bool            g_show_stack    = true;
bool            g_debug         = false;
bool            g_step          = false;
static uint8_t  m_last_token    = 0;

//int8_t* dbg_in[32];
//static int32_t  cmd         = 0;

/**
 *
 *
 * @param void
 * @return void
 */
void show_stack(void)
{
    int32_t i       = 0;

#if HAVE_FP_NUMBERS
    float   fval    = 0;
#endif

    printf("( ");

    for(i = 0; i < g_xvm_sp; i++)
    {
#if HAVE_FP_NUMBERS
        if(shadow_g_xvm_stack[i] == THIS_IS_FP_NUM)
        {
            memcpy(&fval, &g_xvm_stack[i],sizeof(uint32_t));

            printf("%f ", fval);
        }
        else
        {
#endif
            if(g_num_base == 10)
                printf("%d ", (int32_t) (g_xvm_stack[i]));
            else
                printf("0x%0.4X ", (uint32_t) (g_xvm_stack[i]));
#if HAVE_FP_NUMBERS
        }
#endif
    }

    printf(")\n");
}

/**
 *
 *
 * @param tok
 * @return void
 */
void show_token(int8_t tok)
{
    printf("%s \n", (char*) token_name(tok));
}

/**
 *
 *
 * @param void
 * @return void
 */
void show_PC(void)
{
    printf("%s %X \n","PC =", g_xvm_pc);
}

/**
 *
 *
 * @param err
 * @return void
 */
void show_error(int8_t err)
{
    switch(err)
    {
        case XVM_OK:
//            printf("FORTH");
            break;
        case XVM_DSTACK_OVERFLOW:
            printf("- Data stack overflow at address %X, token %s\n",
                    g_xvm_pc - 1,
                    (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_RSTACK_OVERFLOW:
            printf("- Return stack overflow at address %X, token %s\n",
                    g_xvm_pc - 1,
                    (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_DSTACK_UNDERFLOW:
            printf("- Data stack underflow at address %X, token %s\n",
                    g_xvm_pc - 1,
                    (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_RSTACK_UNDERFLOW:
            printf("- Return stack underflow at address %X, token %s\n",
                    g_xvm_pc - 1,
                    (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_ILLEGAL_OPCODE:
            printf("- Illegal opcode %X at address %X\n",
                    m_last_token, g_xvm_pc - 1);
            break;
        case XVM_PC_OVERFLOW:
            printf("- Program counter overflow at address %X\n",
                    g_xvm_pc - 1);
            break;
        case XVM_ILLEGAL_FORTH_PROGRAM:
            printf("- It is not a forth program\n");
            break;
        case XVM_UNDEFINED_TOKEN:
            printf("- Undefined token at address %X, token %s\n",
                    g_xvm_pc - 1, (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_DIVISION_BY_0:
            printf("- Division by 0 at address %X, token %s\n",
                    g_xvm_pc - 1, (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_STOPPED:
//            printf("FORTH");
            break;
        case XVM_RAM_OVERFLOW:
            printf("- Ram overflow at address %X, token %s\n",
                    g_xvm_pc - 1, (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_RAM_UNDERFLOW:
            printf("- Ram underflow at address %X, token %s\n",
                    g_xvm_pc - 1, (char*) g_xvm_token_tab[m_last_token].name);
            break;
        case XVM_LEXICAL_ERROR:
            printf("- Lecical error at address %X, token %s\n",
                    g_xvm_pc - 1, (char*) g_xvm_token_tab[m_last_token].name);
            break;
        default:
            printf("- Unknown error at address %X, token %s\n",
                    g_xvm_pc - 1, (char*) g_xvm_token_tab[m_last_token].name);
            break;
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void show_target_dump(void)
{
    if(!g_debug)
        return;

    int32_t i       = 0;
    int32_t j       = 0;
    int32_t addr    = 0;

    printf("\nTarget memory dump:\n");

    if(!g_xvm_code_wradr)
    {
        printf("empty\n");

        return;
    }
// XXX: WTF with the magic numbers
    for(j = 0; j < 16; j++)
    {
        printf("%X: ",addr);

        for(i = 0; i < 16; i++)
        {
            printf("%X ", g_xvm_code[addr]);

            if(i == 7)
                printf(" ");

            addr++;

            if (addr > g_xvm_code_wradr + g_console_code_wradr)
            {
                printf("\n");

                return;
            }
        }
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void show_console_dump(void)
{
    if(!g_debug)
        return;

    int32_t i       = 0;
    int32_t j       = 0;
    int32_t addr    = 0;

    printf("\nConsole buffer dump:\n");

    if(!g_console_code_wradr)
    {
        printf("empty\n");

        return;
    }
// XXX: WTF with the magic numbers
    for(j = 0; j < 16; j++)
    {
        printf("%X: ", addr);

        for(i = 0; i < 16;i++)
        {
            printf("%X ", g_console_code[addr]);

            if(i == 7)
                printf(" ");

            addr++;

            if(addr > g_console_code_wradr)
            {
                printf("\n");

                return;
            }
        }
    }
}

/**
 *
 *
 * @param tok
 * @return void
 */
void debug_tok(uint8_t tok)
{
    m_last_token = tok;

    if(!g_debug)
        return;

// XXX: We need to account for the ILLEGAL_TOKEN that's used to define
//      an ending instruction boundary


    // The opcode token definitions start from index 0.
    // When in console mode the "copy_con_to_target" function adds
    // a byte with a value of "MAX_TOKEN_NO" which should knock us
    // out of the execution loop by throwing an error.
    if(tok >= MAX_TOKEN_NO)
    {
        return;
    }

    printf("PC: %d, Token: %s, ", (int32_t) (g_xvm_pc - 1), (char*) g_xvm_token_tab[tok].name);

    if(g_xvm_token_tab[tok].len == 1 )
    {
        printf("%d, ", (int32_t) (g_xvm_code[g_xvm_pc]));
    }
    else if(g_xvm_token_tab[tok].len == 2)
    {
        printf("%d, ", (int32_t) g_xvm_code[g_xvm_pc]);
    }
    else if(g_xvm_token_tab[tok].len == 3)
    {
        printf("%d, ", ((int32_t) g_xvm_code[g_xvm_pc]) * 0x100 +
                       ((int32_t) g_xvm_code[g_xvm_pc + 1]));
    }
// XXX: fixme - A number
    else if(g_xvm_token_tab[tok].len == 4)
    {
        printf("%d, ", (int32_t) g_xvm_code[g_xvm_pc + 0] * 0x10000 +
                       (int32_t) g_xvm_code[g_xvm_pc + 2]);
    }

    printf("\n");
}

/**
 *
 *
 * @param tok
 * @return void
 */
void debug_step(uint8_t tok)
{
    if(!g_step)
        return;

    if(!g_xvm_token_tab[tok].name)
        return;

    if(g_show_stack)
    {
        if(g_xvm_run_stat != XVM_ILLEGAL_OPCODE)
            show_stack();
    }
}

// end of file
