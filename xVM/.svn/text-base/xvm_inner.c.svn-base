/**@file xvm_inner.c
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
#include <assert.h>

// config.h contains definitions therefore it should come before
// those items wrapped in conditional switches
#include "types.h"
#include "config.h"

#if HAVE_FP_NUMBERS
    #include <math.h>
    #include <string.h>
#endif


#include "xvm.h"
#include "xvm_inner.h"

#ifdef CONSOLE_MODE
    #include "debug.h"
    #include "variable.h"
#endif

#if HAVE_PRINTF
    #include "parser.h"
#endif

#if HAVE_FFI
    #include "ffi.h"
#endif

#if HAVE_FP_NUMBERS
    // Enable floating-point optimizations (C99)
    #pragma STDC FP_CONTRACT ON

    #ifdef CONSOLE_MODE
        static bool check_fixed_pt_types(int32_t cnt);
        static bool check_flting_pt_types(int32_t cnt);
    #endif
#endif



static inline void xvm_drop(void);
static inline void xvm_2drop(void);
static inline void xvm_rpush(uint32_t data);
static inline uint32_t xvm_rfetch(int32_t offset);
static inline uint32_t xvm_rpop(void);
static inline uint32_t xvm_next_4bytes(void);
static inline uint32_t xvm_next_2bytes(void);


//--------------------------------------
// user data memory
uint32_t        g_xvm_ram[XVM_U32_MAX_RAM + XVM_U32_MAX_VARS];

// Offset to the user defined variable section
uint32_t* const g_xvm_vars = &g_xvm_ram[XVM_U32_MAX_RAM];

//--------------------------------------
// string memory
#if HAVE_PRINTF
    int8_t          g_xvm_str[XVM_I8_MAX_STR];
    uint32_t        g_xvm_strndx   = 0;
#endif

//--------------------------------------
// code memory
uint8_t         g_xvm_code[XVM_U8_MAX_CODE];
uint8_t*        g_xvm_codeptr   = 0;
uint32_t        g_xvm_max_code  = XVM_U8_MAX_CODE;

//--------------------------------------
// data and return stack
uint32_t        g_xvm_stack[XVM_U32_MAX_STACK];
uint32_t        g_xvm_rstack[XVM_U32_MAX_STACK];

//--------------------------------------
//
int32_t         g_xvm_sp        = 0;                // Data stack pointer
int32_t         g_xvm_rp        = 0;                // Return stack pointer
uint32_t        g_xvm_pc        = 0;                // Program counter
int8_t          g_xvm_run_stat  = 0;                // Last run-time status

/**
 *
 *
 * @param void
 * @return void
 */
void xvm_run(void)
{
#if HAVE_FP_NUMBERS
    double_t    d64_x1  = 0.0;
    double_t    d64_x2  = 0.0;
    float_t     f32_x1  = 0.0;
    float_t     f32_x2  = 0.0;
    float_t     f32_x3  = 0.0;

    #ifdef CONSOLE_MODE
        uint32_t    type_1  = 0;
        uint32_t    type_2  = 0;
        uint32_t    type_3  = 0;
        uint32_t    type_4  = 0;
        uint32_t    type_5  = 0;
        uint32_t    type_6  = 0;
        bool        bool_x  = 0;
    #endif
#endif

    uint32_t    u32_x   = 0;
    uint32_t    u32_x1  = 0;
    uint32_t    u32_x2  = 0;
    uint32_t    u32_x3  = 0;
    uint32_t    u32_x4  = 0;
    uint32_t    u32_x5  = 0;
    uint32_t    u32_x6  = 0;
    int64_t     i64_x1  = 0;
    int64_t     i64_x2  = 0;
    int32_t     i32_x   = 0;
    int32_t     i32_x1  = 0;
    int32_t     i32_x2  = 0;
    int32_t     i32_x3  = 0;
    int32_t     i32_x4  = 0;
    int16_t     i16_x   = 0;
    int8_t      i8_x    = 0;
    uint8_t     token   = 0;

    g_xvm_run_stat      = XVM_OK;

    while(g_xvm_run_stat == XVM_OK)
    {
        CHK_PC_REL_OVERFLOW(1);

        token = xvm_next_byte();

//printf("xvm_next_byte - token: %d\n", token);

#ifdef CONSOLE_MODE
        debug_tok(token);
#endif

        // INFO:

        switch(token)
        {
            case TOKEN_NOOP:                              // ( -- )
                break;
            case TOKEN_DROP:                              // ( x -- )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                xvm_drop();
                break;
            case TOKEN_TWO_DROP:                          // ( x1 x2 -- )
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                xvm_2drop();
                break;
            case TOKEN_DUP:                               // ( x -- x x )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x = xvm_fetch(1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_TWO_DUP:                           // ( x1 x2 -- x1 x2 x1 x2 )
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                GET_THE_TYPE(type_2);
                u32_x1 = xvm_fetch(2);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x2 = xvm_fetch(1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_SWAP:                              // ( x1 x2 -- x2 x1 )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_2);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                break;
            case TOKEN_PICK:
                CHK_DSTACK_UNDERFLOW(ONE_CELL);         // ( index -- x )
                u32_x = xvm_pop(); // index address for the fetch
                CHK_DSTACK_UNDERFLOW(u32_x);
                GET_THE_TYPE_AT_BASE_OFFSET(type_1, offset);
                u32_x1 = xvm_fetch_from_base(u32_x);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                break;
            case TOKEN_TWO_SWAP:                        // ( x1 x2 x3 x4 -- x3 x4 x1 x2 )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_4);
                u32_x4 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_3);
                u32_x3 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_2);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x3);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x4);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_OVER:                              // ( x1 x2 -- x1 x2 x1 )
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                GET_THE_TYPE_AT_TOP_OFFSET(type_2, 2);
                u32_x1 = xvm_fetch(2);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                break;
            case TOKEN_TWO_OVER:                          // ( x1 x2 x3 x4 -- x1 x2 x3 x4 x1 x2 )
                CHK_DSTACK_UNDERFLOW(FOUR_CELLS);
                GET_THE_TYPE_AT_TOP_OFFSET(type_1, 4);
                u32_x1 = xvm_fetch(4);
                CHK_DSTACK_UNDERFLOW(THREE_CELLS);
                GET_THE_TYPE_AT_TOP_OFFSET(type_2, 3);
                u32_x2 = xvm_fetch(3);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_ROT:                               // ( x1 x2 x3 -- x2 x3 x1 )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_3);
                u32_x3 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_2);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x3);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                break;
            case TOKEN_TWO_ROT:                           // ( x1 x2 x3 x4 x5 x6 -- x3 x4 x5 x6 x1 x2)
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_6);
                u32_x6 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_5);
                u32_x5 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_4);
                u32_x4 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_3);
                u32_x3 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_2);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x3);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x4);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x5);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x6);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_NIP:                               // ( x1 x2 -- x2 )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_2);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_TUCK:                              // ( x1 x2 -- x2 x1 x2)
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_2);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_TO_RSTACK:                         // >R     ( x -- ) R:( -- x )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x = xvm_pop();
                CHK_RSTACK_OVERFLOW(ONE_CELL);
                xvm_rpush(u32_x);
                break;
            case TOKEN_TWO_TO_RSTACK:                     // 2>R    (D: x1 x2 -- )  (R:  -- x1 x2 )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_2);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                GET_THE_TYPE(type_1);
                u32_x1 = xvm_pop();
                CHK_RSTACK_OVERFLOW(ONE_CELL);
                xvm_rpush(u32_x1);
                CHK_RSTACK_OVERFLOW(ONE_CELL);
                xvm_rpush(u32_x2);
                break;
            case TOKEN_FROM_RSTACK:                       // R>   (R:  x -- )  (D:  -- x )
                CHK_RSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_rpop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_TWO_FROM_RSTACK:                   // 2R>  (R:  x1 x2 -- )  (D:  -- x1 x2 )
                CHK_RSTACK_UNDERFLOW(ONE_CELL);
                u32_x2 = xvm_rpop();
                CHK_RSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_rpop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_FETCH_RSTACK:                      // R@     (R: x -- x )  (D:  -- x )
                CHK_RSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_rfetch(1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_TWO_FETCH_RSTACK:                  // 2R@    (R: x1 x2 -- x1 x2 ) (D: -- x1 x2 )
                CHK_RSTACK_UNDERFLOW(TWO_CELLS);
                u32_x1 = xvm_rfetch(2);
                CHK_RSTACK_UNDERFLOW(ONE_CELL);
                u32_x2 = xvm_rfetch(1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x2);
                break;
            case TOKEN_FETCH:                             // @  ( addr -- x ) Reads data from specified RAM address
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                CHK_RAM_BOUNDARY(u32_x);
                u32_x1 = g_xvm_ram[u32_x];
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1);
    #if HAVE_FP_NUMBERS
        #ifdef CONSOLE_MODE
                if((u32_x >= XVM_U32_MAX_RAM) && (u32_x < (XVM_U32_MAX_RAM + XVM_U32_MAX_VARS)))
                {
                    bool_x = var_def[u32_x - XVM_U32_MAX_RAM].type;
                    if(bool_x == VAR_VAL_TYPE_FP)
                        shadow_g_xvm_stack[g_xvm_sp] = THIS_IS_FP_NUM;
                    else // bool_x == VAR_VAL_TYPE_SC
                        shadow_g_xvm_stack[g_xvm_sp] = THIS_IS_NOT_FP_NUM;
                }
        #endif
    #endif
               break;
            case TOKEN_STORE:                             // !  ( x addr -- ) Writes data to specified RAM address
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();             // address
    #if HAVE_FP_NUMBERS
        #ifdef CONSOLE_MODE
                u32_x4 = shadow_g_xvm_stack[g_xvm_sp];  // save the data's type
        #endif
    #endif
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();            // data
                CHK_RAM_BOUNDARY(u32_x);
                g_xvm_ram[u32_x] = u32_x1;      // write data to RAM
    #if HAVE_FP_NUMBERS
        #ifdef CONSOLE_MODE
                if((u32_x >= XVM_U32_MAX_RAM) && (u32_x < (XVM_U32_MAX_RAM + XVM_U32_MAX_VARS)))
                {
                    if(u32_x4 == THIS_IS_FP_NUM)
                        var_def[u32_x - XVM_U32_MAX_RAM].type = VAR_VAL_TYPE_FP;
                    else // u32_x4 == THIS_IS_NOT_FP_NUM
                        var_def[u32_x - XVM_U32_MAX_RAM].type = THIS_IS_NOT_FP_NUM;
                }
        #endif
    #endif
                break;
            case TOKEN_AND:                               // ( x1 x2 -- x1&x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1 & u32_x2);
                break;
            case TOKEN_OR:                                // ( x1 x2 -- x1|x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1 | u32_x2);
                break;
            case TOKEN_XOR:                               // ( x1 x2 -- x1^x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1 ^ u32_x2);
                break;
            case TOKEN_NOT:                               // ( x -- ~x )
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(~u32_x);
                break;
            case TOKEN_LSHIFT:                            // << ( x1 x2 -- x1<<x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1 << u32_x2);
                break;
            case TOKEN_RSHIFT:                            // >> ( x1 x2 -- x1>>x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x2 = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x1 >> u32_x2);
                break;
            case TOKEN_INC:                               // 1+ ( x -- x+1 )
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) (i32_x + 1));
                break;
            case TOKEN_DEC:                               // 1- ( x -- x-1 )
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) (i32_x - 1));
                break;
            case TOKEN_ADD:                               // +  ( x1 x2 -- x1+x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) (i32_x1 + i32_x2));
                break;
            case TOKEN_SUB:                               // -  ( x1 x2 -- x1-x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) (i32_x1 - i32_x2));
                break;
            case TOKEN_MUL:                               // *  ( x1 x2 -- x1*x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) (i32_x1 * i32_x2));
                break;
            case TOKEN_DIV:                               // '/' ( x1 x2 -- x1/x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DIV_BY_ZERO(i32_x2);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) (i32_x1 / i32_x2));
                break;
            case TOKEN_MULDIV:                            // '*/'    ( x1 x2 x3 -- x1*x2/x3 )
                CHECK_FIXED_TYPES(THREE_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x3 = (int32_t) xvm_pop();
                CHK_DIV_BY_ZERO(i32_x3);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                i64_x1 = i32_x1 * i32_x2;
                i64_x2 = i64_x1 / i32_x3;
                i32_x1 = (int32_t) i64_x2;
                xvm_push(i32_x1);
                break;
            case TOKEM_MOD:                             // 'MOD'    ( x1 x2 -- remainder:x1/x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x3 = (int32_t) xvm_pop();
                CHK_DIV_BY_ZERO(i32_x3);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                i32_x1 = i32_x2 % i32_x3;
                xvm_push(i32_x1);
                break;
            case TOKEN_DIVMOD:                          // '/MOD'    ( x1 x2 -- remainder:x1/x2 quotient:x1/x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x4 = (int32_t) xvm_pop();
                CHK_DIV_BY_ZERO(i32_x4);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x3 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                i32_x2 = i32_x3 % i32_x4;
                xvm_push(i32_x2);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                i32_x1 = i32_x3 / i32_x4;
                xvm_push(i32_x1);
                break;
            case TOKEN_NEGATE:                          // '-'    ( x1 -- -x1 )
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                i32_x1 = -i32_x2;
                xvm_push(i32_x1);
                break;
            case TOKEN_ABS:                             // 'ABS'    ( x1 -- [x1] )
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                i32_x1 = abs(i32_x2);
                xvm_push(i32_x1);
                break;
            case TOKEN_MIN:                             // 'MIN'    ( x1 x2 -- x1|x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x3 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                if(i32_x3 < i32_x2)
                    i32_x1 = i32_x3;
                else
                    i32_x1 = i32_x2;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(i32_x1);
                break;
            case TOKEN_MAX:                             // 'MAX'    ( x1 x2 -- x1|x2 )
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x3 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                if(i32_x3 > i32_x2)
                    i32_x1 = i32_x3;
                else
                    i32_x1 = i32_x2;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(i32_x1);
                break;
            case TOKEN_GT:                                // >  ( x1 x2 -- flag ) If x1 > x2 then flag = -1 else flag = 0
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                if(i32_x1 > i32_x2)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_LT:                                // <  ( x1 x2 -- flag ) If x1 < x2 then flag = -1 else flag = 0
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                if(i32_x1 < i32_x2)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_GE:                                // >= ( x1 x2 -- flag ) If x1 >= x2 then flag = -1 else flag = 0
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                if(i32_x1 >= i32_x2)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_LE:                                // <= ( x1 x2 -- flag ) If x1 <= x2 then flag=  -1 else flag = 0
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                if(i32_x1 <= i32_x2)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_EQ:                                // =  ( x1 x2 -- flag ) If x1 = x2 then flag = -1 else flag = 0
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                if(i32_x1 == i32_x2)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_NEQ:                               // != ( x1 x2 -- flag ) If x1!=x2 then flag=-1 else flag=0
                CHECK_FIXED_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x2 = (int32_t) xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                if(i32_x1 != i32_x2)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_EQ0:                               // =0 ( x -- flag ) If x = 0 then flag = -1 else flag = 0
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                if(!u32_x1)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_NEQ0:                              // !=0    ( x -- flag ) If x!=0 then flag=-1 else flag=0
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x1 = xvm_pop();
                if(u32_x1)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_GT0:                               // >0 ( x -- flag ) If x > 0 then flag = -1 else flag = 0
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x = (int32_t) xvm_pop();
                if(i32_x > 0)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_LT0:                               // <0 ( x -- flag ) If x < 0 then flag = -1 else flag = 0
                CHECK_FIXED_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x = (int32_t) xvm_pop();
                if(i32_x < 0)
                    u32_x = FORTH_COND_TRUE;
                else
                    u32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_PC:                                // ( -- pc ) Put program counter on top of stack
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(g_xvm_pc);
                break;
            case TOKEN_SP:                                // ( -- sp ) Put data stack pointer on top of stack
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) g_xvm_sp);
                break;
            case TOKEN_RP:                                // ( -- rp ) Put return stack on top of stack
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push((uint32_t) g_xvm_rp);
                break;
            case TOKEN_FLUSH:                             // ( x1 x2 x3... -- ) Empty data stack
    #if HAVE_FP_NUMBERS
        #ifdef CONSOLE_MODE
                memset(shadow_g_xvm_stack, THIS_IS_NOT_FP_NUM, sizeof(int32_t) * XVM_U32_MAX_STACK);
        #endif
    #endif
                g_xvm_sp = SP_RESET;
                break;
    #if HAVE_PRINTF
            case TOKEN_PRINT:                             // .  ( x -- ) Print x
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
        #if HAVE_FP_NUMBERS
            #ifdef CONSOLE_MODE

                if(shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM)
                    bool_x = true;
            #endif
        #endif
                u32_x = xvm_pop();
        #if HAVE_FP_NUMBERS
                    if(bool_x)
                    {
                        memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                        printf("%f", (float) f32_x1);
                    }
                    else
        #endif
                    {
                        if(g_num_base == 10)
                            printf("%d", (int32_t) u32_x);
                        else
                            printf("0x%X", u32_x);
                    }
                break;
            case TOKEN_PRINT_STR:                             // .  ( index -- ) Print string at array[index]

                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                printf("%s", (char*) &g_xvm_str[u32_x]);

                break;
            case TOKEN_PRINT_LINEFEED:                         // .  ( -- ) Print a linefeed charcater
                    printf("\n");
                break;
    #endif // HAVE_PRINTF
            case TOKEN_LIT0:                              // ( -- 0 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(0);
                break;
            case TOKEN_LIT1:                              // ( -- 1 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(1);
                break;
            case TOKEN_LIT2:                              // ( -- 2 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(2);
                break;
            case TOKEN_LIT3:                              // ( -- 3 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(3);
                break;
            case TOKEN_LIT4:                              // ( -- 4 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(4);
                break;
            case TOKEN_LIT5:                              // ( -- 5 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(5);
                break;
            case TOKEN_LIT6:                              // ( -- 6 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(6);
                break;
            case TOKEN_LIT7:                              //( -- 7 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(7);
                break;
            case TOKEN_LIT8:                              // ( -- 8 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(8);
                break;
            case TOKEN_LIT9:                              // ( -- 9 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(9);
                break;
            case TOKEN_LIT10:                             // ( -- 10 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(10);
                break;
            case TOKEN_LIT11:                             // ( -- 11 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(11);
                break;
            case TOKEN_LIT12:                             // ( -- 12 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(12);
                break;
            case TOKEN_LIT13:                             // ( -- 13 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(13);
                break;
            case TOKEN_LIT14:                             // ( -- 14 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(14);
                break;
            case TOKEN_LIT15:                             // ( -- 15 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(15);
                break;
            case TOKEN_LIT_MINUS1:                       // ( -- -1 )
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(FORTH_COND_TRUE);
                break;
            case TOKEN_GOTO:                             // ( addr -- )
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                CHK_PC_OVERFLOW(u32_x);
                g_xvm_pc = u32_x;
                break;
            case TOKEN_CALL:                             // ( addr -- )
                CHK_RSTACK_OVERFLOW(ONE_CELL);
                xvm_rpush(g_xvm_pc);    // save PC
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop(); // new PC address
                CHK_PC_OVERFLOW(u32_x);
                g_xvm_pc = u32_x;
                break;
            case TOKEN_RETURN:                           // (R: addr -- )
                CHK_RSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_rpop();
                CHK_PC_OVERFLOW(u32_x);
                g_xvm_pc = u32_x;
                break;
            case TOKEN_JZ:                               // ( reladdr x -- ) Jump if x = 0
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();                  // condition value
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();        // relative address
                if(!u32_x)
                {
                    CHK_PC_REL_OVERFLOW(i32_x);
                    g_xvm_pc = (uint32_t) ((int32_t) g_xvm_pc + i32_x1);
                }
                break;
            case TOKEN_JNZ:                              // ( reladdr x -- ) Jump if x != 0
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                i32_x1 = (int32_t) xvm_pop();
                if(u32_x)
                {
                    CHK_PC_REL_OVERFLOW(i32_x);
                    g_xvm_pc = (uint32_t) ((int32_t) g_xvm_pc + i32_x1);
                }
                break;
            case TOKEN_RETZ:                             // ( x -- ) Return if x = 0
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                if(!u32_x)
                {
                    CHK_RSTACK_UNDERFLOW(ONE_CELL);
                    u32_x1 = xvm_rpop();
                    CHK_PC_OVERFLOW(u32_x1);
                    g_xvm_pc = u32_x1;
                }
                break;
            case TOKEN_RETNZ:                            // ( x -- ) Return if x 1+ 0
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                if(u32_x)
                {
                    CHK_RSTACK_UNDERFLOW(ONE_CELL);
                    u32_x1 = xvm_rpop();
                    CHK_PC_OVERFLOW(u32_x1);
                    g_xvm_pc = u32_x1;
                }
                break;
            case TOKEN_LIT:                              // ( -- val )
                CHK_PC_REL_OVERFLOW(ONE_CELL);
                u32_x = xvm_next_byte();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_SJMP:                             // ( -- )
                CHK_PC_REL_OVERFLOW(ONE_CELL);
                i8_x = (int8_t) xvm_next_byte();
                CHK_PC_REL_OVERFLOW(i8_x);
                g_xvm_pc = (uint32_t) ((int32_t) g_xvm_pc + i8_x);
                break;
            case TOKEN_SJZ:                              // ( x -- )
                CHK_PC_REL_OVERFLOW(ONE_CELL);
                i8_x = (int8_t) xvm_next_byte();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                if(!u32_x)
                {
                    CHK_PC_REL_OVERFLOW(i8_x);
                    g_xvm_pc = (uint32_t) ((int32_t) g_xvm_pc + i8_x);
                }
                break;
            case TOKEN_SJNZ:                             // ( x -- )
                CHK_PC_REL_OVERFLOW(ONE_CELL);
                i8_x = (int8_t) xvm_next_byte();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                if(u32_x)
                {
                    CHK_PC_REL_OVERFLOW(i8_x);
                    g_xvm_pc = (uint32_t) ((int32_t) g_xvm_pc + i8_x);
                }
                break;
            case TOKEN_LLIT:                             // ( -- val )
                CHK_PC_OVERFLOW(FOUR_CELLS);
                u32_x = xvm_next_4bytes();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                xvm_push(u32_x);
                break;
            case TOKEN_LGOTO:                            // ( -- )
                CHK_PC_OVERFLOW(TWO_CELLS);
                u32_x = xvm_next_2bytes();
                CHK_PC_REL_OVERFLOW(u32_x);
                g_xvm_pc = u32_x;
                break;
            case TOKEN_LJMP:                             // ( -- )
                CHK_PC_OVERFLOW(TWO_CELLS);
                i16_x = (int16_t) xvm_next_2bytes();
                CHK_PC_REL_OVERFLOW(i16_x);
                g_xvm_pc =  (uint32_t) ((int32_t) g_xvm_pc + i16_x);
                break;
            case TOKEN_LCALL:                            // ( -- )
                CHK_PC_OVERFLOW(TWO_CELLS);
                u32_x = xvm_next_2bytes();
                CHK_RSTACK_OVERFLOW(ONE_CELL);
                xvm_rpush(g_xvm_pc);
                CHK_PC_OVERFLOW(u32_x);
                g_xvm_pc = u32_x;
                break;
            case TOKEN_LJZ:                              // ( x -- )
                CHK_PC_OVERFLOW(TWO_CELLS);
                u32_x1 = xvm_next_2bytes();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                if(!u32_x)
                {
                    CHK_PC_REL_OVERFLOW(u32_x1);
                    g_xvm_pc = g_xvm_pc + u32_x1;
                }
                break;
            case TOKEN_LJNZ:                             // ( x -- )
                CHK_PC_OVERFLOW(TWO_CELLS);
                u32_x1 = xvm_next_2bytes();
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                if(u32_x)
                {
                    CHK_PC_REL_OVERFLOW(u32_x1);
                    g_xvm_pc = g_xvm_pc + u32_x1;
                }
                break;
//-----------------
    #if HAVE_FP_NUMBERS
            case TOKEN_LLITFP:                           // ( -- val )
                CHK_PC_OVERFLOW(FOUR_CELLS);
                u32_x = xvm_next_4bytes();
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FINC:                            // F1+ ( x -- x+1 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                f32_x1 += 1;
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FADD:                            // F+  ( x1 x2 -- x1+x2 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                f32_x1 = f32_x1 + f32_x2;
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FSUB:                            // F-  ( x1 x2 -- x1-x2 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                f32_x1 = f32_x1 - f32_x2;
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FMUL:                            // F-  ( x1 x2 -- x1*x2 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                f32_x1 = f32_x1 * f32_x2;
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FDIV:                            // F/  ( x1 x2 -- x1/x2 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                f32_x1 = f32_x1 / f32_x2;
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FMULDIV:                            // 'F*/'    ( x1 x2 x3 -- x1*x2/x3 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x3, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = (uint32_t) xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                d64_x1 = f32_x1 * f32_x2;
                d64_x2 = d64_x1 / f32_x3;
                f32_x1 = (float) d64_x2;
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
//----
//----
            case TOKEM_FMOD:                            // 'FMOD'    ( f1 f2 -- remainder: x1/x2 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                CHK_DIV_BY_ZERO(u32_x);
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                f32_x3 = (float) fmodf((float) f32_x1, (float) f32_x2);
                memcpy(&u32_x, &f32_x3, sizeof(uint32_t));
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FDIVMOD:                            //'F/MOD'    ( f1 f2 -- remainder: f1/f2 quotient: f1/f2)
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                CHK_DIV_BY_ZERO(u32_x);
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                f32_x3 = (float) fmodf((float) f32_x1, (float) f32_x2);
                memcpy(&u32_x, &f32_x3, sizeof(uint32_t));
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                f32_x3 = f32_x1 / f32_x2;
                memcpy(&u32_x, &f32_x3, sizeof(uint32_t));
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FNEGATE:                            // F-    ( f1 -- -f1 )
                CHECK_FLOAT_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                f32_x1 = -f32_x2;
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FABS:                            // FABS     ( f1 -- [f1] )
                CHECK_FLOAT_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                f32_x1 = (float) fabsf((float) f32_x2);
                memcpy(&u32_x, &f32_x1, sizeof(uint32_t));
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FMIN:                            // 'FMIN'    ( f1 f2 -- f1|f2 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 < f32_x2)
                    f32_x3 = f32_x1;
                else
                    f32_x3 = f32_x2;
                memcpy(&u32_x, &f32_x3, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FMAX:                            // 'FMAX'    ( f1 f2 -- f1|f2 )
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 > f32_x2)
                    f32_x3 = f32_x1;
                else
                    f32_x3 = f32_x2;
                memcpy(&u32_x, &f32_x3, sizeof(uint32_t));
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FLOAT_TYPE;
                xvm_push(u32_x);
                break;
            case TOKEN_FGT:                            // F>  ( f1 f2 -- flag ) If f1 > f2 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 > f32_x2)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FLT:                            // F<  ( f1 f2 -- flag ) If f1 > f2 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 < f32_x2)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FGE:                            // F>= ( f1 f2 -- flag ) If F1 >= F2 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 >= f32_x2)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FLE:                            // F<= ( f1 f2 -- flag ) If F1 <= F2 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 <= f32_x2)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FEQ:                            // F= ( f1 f2 -- flag ) If F1 == F2 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 == f32_x2)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FNEQ:                            // F!= ( f1 f2 -- flag ) If F1 != F2 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(TWO_CELLS);
                CHK_DSTACK_UNDERFLOW(TWO_CELLS);
                u32_x = xvm_pop();
                memcpy(&f32_x2, &u32_x, sizeof(uint32_t));
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 != f32_x2)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FEQ0:                            // F=0 ( f -- flag ) If f == 0 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 == 0)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FNEQ0:                            // F!=0 ( f -- flag ) If f == 0 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 != 0)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FGT0:                            // F>0 ( f -- flag ) If f > 0 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 > 0)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
            case TOKEN_FLT0:                            // F<0 ( f -- flag ) If f < 0 then flag = -1 else flag = 0
                CHECK_FLOAT_TYPES(ONE_CELL);
                CHK_DSTACK_UNDERFLOW(ONE_CELL);
                u32_x = xvm_pop();
                memcpy(&f32_x1, &u32_x, sizeof(uint32_t));
                if(f32_x1 < 0)
                    i32_x = FORTH_COND_TRUE;
                else
                    i32_x = FORTH_COND_FALSE;
                CHK_DSTACK_OVERFLOW(ONE_CELL);
                SET_AS_FIXED_TYPE;
                xvm_push(i32_x);
                break;
//----
//----
    #endif // HAVE_FP_NUMBERS
//------------
    #if HAVE_FFI
            case TOKEN_FFICALL:                         // _FFICALL    (  --  )
                CHK_PC_OVERFLOW(TWO_CELLS);
                i32_x1 = (int32_t) xvm_next_2bytes();
                ffi_call_func(i32_x1);              //
                break;
    #endif // HAVE_FFI
            default:
                g_xvm_run_stat = XVM_ILLEGAL_OPCODE;
                break;
        } // switch(token)

#ifdef CONSOLE_MODE
        debug_step(token);
#endif
    }
}

#if HAVE_FP_NUMBERS
    #ifdef CONSOLE_MODE
        /**
         *
         *
         * @param cnt the number of cells to check
         * @return true if successful false otherwise
         */
        bool check_fixed_pt_types(int32_t cnt)
        {
            bool status = false;

            if(cnt == ONE_CELL)
            {
                if(shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_NOT_FP_NUM)
                {
                    status = true;
                }
            }
            else if(cnt == TWO_CELLS)
            {
                if((shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 2] == THIS_IS_FP_NUM))
                {
                    status = true;
                }
            }
            else if(cnt == THREE_CELLS)
            {
                if((shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 2] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 3] == THIS_IS_FP_NUM))
                {
                    status = true;
                }
            }
            else if(cnt == FOUR_CELLS)
            {
                if((shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 2] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 3] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 4] == THIS_IS_FP_NUM))
                {
                    status = true;
                }
            }

            if(!status)
                printf("Note: fixed and floating point stack manipulation mistmatch.\n");

            return status;
        }
    #endif
#endif

#if HAVE_FP_NUMBERS
    #ifdef CONSOLE_MODE
        /**
         *
         *
         * @param cnt  the number of cells to check
         * @return true if successful false otherwise
         */
        bool check_flting_pt_types(int32_t cnt)
        {
            bool status = false;

            if(cnt == ONE_CELL)
            {
                if(shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM)
                {
                    status = true;
                }
            }
            else if(cnt == TWO_CELLS)
            {
                if((shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 2] == THIS_IS_FP_NUM))
                {
                    status = true;
                }
            }
            else if(cnt == THREE_CELLS)
            {
                if((shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 2] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 3] == THIS_IS_FP_NUM))
                {
                    status = true;
                }
            }
            else if(cnt == FOUR_CELLS)
            {
                if((shadow_g_xvm_stack[g_xvm_sp - 1] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 2] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 3] == THIS_IS_FP_NUM) &&
                   (shadow_g_xvm_stack[g_xvm_sp - 4] == THIS_IS_FP_NUM))
                {
                    status = true;
                }
            }

            if(!status)
                printf("Note: floating and fixed point stack manipulation mistmatch.\n");

            return status;
        }
    #endif
#endif

/**
 * Drops one item from the data stack
 *
 * @param void
 * @return the popped item
 */
inline void xvm_drop(void)
{
    g_xvm_sp -= 1;

#if HAVE_FP_NUMBERS
    #ifdef CONSOLE_MODE
        shadow_g_xvm_stack[g_xvm_sp] = THIS_IS_NOT_FP_NUM;
    #endif
#endif
}

/**
 * Drops two items from the data stack
 *
 * @param void
 * @return the popped item
 */
inline void xvm_2drop(void)
{
    g_xvm_sp -= 2;

#if HAVE_FP_NUMBERS
    #ifdef CONSOLE_MODE
        shadow_g_xvm_stack[g_xvm_sp]        = THIS_IS_NOT_FP_NUM;
        shadow_g_xvm_stack[g_xvm_sp + 1]    = THIS_IS_NOT_FP_NUM;
    #endif
#endif
}

/**
 * Pushes an item on to the return stack.
 *
 * @param data item to be pushed
 * @return void
 */
inline void xvm_rpush(uint32_t data)
{
    g_xvm_rstack[g_xvm_rp] = data;
    g_xvm_rp += 1;
}

/**
 * Fetches an item from the return stack from some offset of RP.
 *
 * @param offset
 * @return the fetched item
 */
inline uint32_t xvm_rfetch(int32_t offset)
{
    return g_xvm_rstack[g_xvm_rp - offset];
}

/**
 * Pops an item off of the return stack.
 *
 * @param void
 * @return the popped item
 */
inline uint32_t xvm_rpop(void)
{
    uint32_t x = 0;

    g_xvm_rp    -= 1;
    x           = g_xvm_rstack[g_xvm_rp];

    return x;
}

/**
 * Fetch the next number, 4 bytes, from code memory.
 *
 * @param void
 * @return the fetched number
 */
inline uint32_t xvm_next_4bytes(void)
{
    uint32_t value = 0;

    assert(g_xvm_codeptr);

    value =  (((uint32_t) *(g_xvm_codeptr + g_xvm_pc))     << 24);
    value |= (((uint32_t) *(g_xvm_codeptr + g_xvm_pc + 1)) << 16);
    value |= (((uint32_t) *(g_xvm_codeptr + g_xvm_pc + 2)) <<  8);
    value |=  ((uint32_t) *(g_xvm_codeptr + g_xvm_pc + 3));

    g_xvm_pc += 4;

    return value;
}

/**
 * Fetch a (2 byte) word from code memory.
 *
 * @param void
 * @return the fetched word
 */
inline uint32_t xvm_next_2bytes(void)
{
    uint32_t value = 0;

    assert(g_xvm_codeptr);

    value =   ((uint32_t) *(g_xvm_codeptr + g_xvm_pc)) << 8;
    value |=  ((uint32_t) *(g_xvm_codeptr + g_xvm_pc + 1));

    g_xvm_pc += 2;

    return value;
}

/**
 * Restarts the VM by initializing PC, error flag, and the code-pointer.
 *
 * @param void
 * @return void
 */
void xvm_restart(void)
{
    g_xvm_pc        = 0;
    g_xvm_run_stat  = XVM_OK;
    g_xvm_codeptr   = g_xvm_code;
}

// end of file
