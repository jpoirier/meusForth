/**@file code.c
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

#include "types.h"
#include "parser.h"
#include "xvm.h"
#include "xvm_inner.h"
#include "compiler.h"
#include "code.h"

//------------------------------------------------
// xvm_code is defined in xvm.c
//uint8_t         target_name[1000][32];

uint32_t            g_xvm_code_wradr        = 0;            // write address
static uint32_t     m_xvm_code_rdadr        = 0;            // read address

//------------------------------------------------
uint8_t             g_console_code[MAX_CONSOLE_ADDR];       // tmp buffer to compile console text
uint32_t            g_console_code_wradr    = 0;            // write address
uint32_t            g_console_code_rdadr    = 0;            // read address

//------------------------------------------------
//uint8_t             g_voc_code[MAX_VOC_ADDR];               // local vocabulary
//uint32_t            g_voc_code_wradr      = 0;
//uint32_t            g_voc_code_rdadr      = 0;

//------------------------------------------------
static uint32_t     m_max_code_rdadr        = 0;
static uint32_t     m_code_rdadr            = 0;
static int32_t      m_exe_mode              = EXE_CONSOLE;

/**
 * Saves the current executable mode then sets the system
 * to the requested executablemode.
 *
 * @param mode the requested mode
 * @return void
 */
void set_exe_mode(int32_t mode)
{
    switch(m_exe_mode) // save the current exe addr based on the current mode
    {
        case EXE_TARGET:
            m_xvm_code_rdadr        = m_code_rdadr;
            break;
        default: // EXE_CONSOLE
            g_console_code_rdadr    = m_code_rdadr;
            break;
    }

    switch(mode) // set write addr
    {
        case EXE_TARGET:
            m_max_code_rdadr    = XVM_U8_MAX_CODE;
            m_code_rdadr        = m_xvm_code_rdadr;
            g_xvm_codeptr       = g_xvm_code;
            break;
        default: // EXE_CONSOLE
            m_max_code_rdadr    = MAX_CONSOLE_ADDR;
            m_code_rdadr        = g_console_code_rdadr;
            g_xvm_codeptr       = g_console_code;
            break;
    }

    g_xvm_max_code  = m_max_code_rdadr;
    m_exe_mode      = mode;
}

/**
 * Fetches the next byte from the byte stream.
 *
 * @param void
 * @return the next byte or the XVM_STOPPED flag
 */
uint8_t get_next_byte(void)
{
    if(g_xvm_pc < g_comp_wradr)
        return xvm_next_byte();
    else
        g_xvm_run_stat = XVM_STOPPED;

   return 0;
}

// end of file

