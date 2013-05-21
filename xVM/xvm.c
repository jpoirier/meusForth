/**@file xvm.c
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
#include <assert.h>

#include "xvm.h"
#include "xvm_inner.h"

/**
 * Fetches the next byte from code memory.
 *
 * @param void
 * @return the fetched byte
 */
uint8_t xvm_next_byte(void)
{
    assert(g_xvm_codeptr);

    uint8_t x = *(g_xvm_codeptr + g_xvm_pc);

    g_xvm_pc += 1;

    return x;
}

/**
 * Fetches a data stack item at some offset of SP.
 *
 * @param offset the address offset
 * @return fetched item
 */
uint32_t xvm_fetch(int32_t offset)
{
    return g_xvm_stack[g_xvm_sp - offset];
}

/**
 * Fetches an item from the data stack from some offset of SP.
 *
 * @param offset the address offset
 * @return fetched item
 */
uint32_t xvm_fetch_from_base(int32_t offset)
{
    return g_xvm_stack[offset];
}

/**
 * Resets the VM by initializing SP and RP then calling xvm_restart.
 *
 * @param void
 * @return void
 */
void xvm_reset(void)
{
    g_xvm_sp = 0;
    g_xvm_rp = 0;

    xvm_restart();
}


// end of file
