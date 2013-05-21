/**@file xvm.h
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
#ifndef XVM_H
#define XVM_H

#include "types.h"
#include "config.h"

#define MAGIC_NUMBER                0xFF00FF00

// error codes
#define XVM_OK                      0
#define XVM_DSTACK_OVERFLOW         1
#define XVM_RSTACK_OVERFLOW         2
#define XVM_DSTACK_UNDERFLOW        3
#define XVM_RSTACK_UNDERFLOW        4
#define XVM_ILLEGAL_OPCODE          5
#define XVM_PC_OVERFLOW             6
#define XVM_ILLEGAL_FORTH_PROGRAM   7
#define XVM_UNDEFINED_TOKEN         8
#define XVM_DIVISION_BY_0           9
#define XVM_STOPPED                 10
#define PARSE_NEXT_LINE             11
#define XVM_LEGAL_OPCODE            12
#define XVM_RAM_OVERFLOW            13
#define XVM_RAM_UNDERFLOW           14
#define XVM_LEXICAL_ERROR           15
#define XVM_PROCESSING_ENDS         16

#ifdef __cplusplus
extern "C"{
#endif

    extern uint8_t xvm_next_byte(void);


    extern uint32_t xvm_fetch(int32_t offset);
    extern uint32_t xvm_fetch_from_base(int32_t offset);


    extern void xvm_reset(void);

#ifdef __cplusplus
}
#endif

#endif // XVM_H

// end of file
