/**@file xvm_inner.h
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
#ifndef XVM_INNER_H
#define XVM_INNER_H

#if DSTACK_CHECK
    #define CHK_DSTACK_UNDERFLOW(x) \
            if(((int32_t) g_xvm_sp - (int32_t) x) < 0) { show_error(XVM_DSTACK_UNDERFLOW); break; }

    #define CHK_DSTACK_OVERFLOW(x) \
            if((g_xvm_sp + x) > (XVM_U32_MAX_STACK - 1)) { show_error(XVM_DSTACK_OVERFLOW); break; }
#else
    #define CHK_DSTACK_UNDERFLOW(x)
    #define CHK_DSTACK_OVERFLOW(x)
#endif

#if RSTACK_CHECK
    #define CHK_RSTACK_UNDERFLOW(x) \
            if(x  < 0) { show_error(XVM_RSTACK_UNDERFLOW); break; }

    #define CHK_RSTACK_OVERFLOW(x) \
            if(x > (XVM_U32_MAX_STACK + 1)) { show_error(XVM_RSTACK_OVERFLOW); break; }
#else
    #define CHK_RSTACK_UNDERFLOW(x)
    #define CHK_RSTACK_OVERFLOW(x)
#endif

#if PC_CHECK
    #define CHK_PC_OVERFLOW(x) \
            if( x > (g_xvm_max_code - 1)) { show_error(XVM_PC_OVERFLOW); break; }
#else
    #define CHK_PC_OVERFLOW(x)
#endif

#if PC_REL_CHECK
    #define CHK_PC_REL_OVERFLOW(x) \
            if((g_xvm_pc + (int32_t) x) > (g_xvm_max_code - 1)) { show_error(XVM_PC_OVERFLOW); break; }
#else
    #define CHK_PC_REL_OVERFLOW(x)
#endif

#if RAM_BOUNDARY_CHECK
    #define CHK_RAM_BOUNDARY(x) \
            if(x > ((XVM_U32_MAX_RAM + XVM_U32_MAX_VARS) - 1)) { show_error(XVM_RAM_OVERFLOW); break; }
#else
    #define CHK_RAM_BOUNDARY(x)
#endif

#if DIV_BY_ZERO_CHECK
    #define CHK_DIV_BY_ZERO(x) \
            if(!x) { show_error(XVM_DIVISION_BY_0); break; }
#else
    #define CHK_DIV_BY_ZERO(x)
#endif

#if HAVE_FP_NUMBERS
    #ifdef CONSOLE_MODE
        #define SET_AS_FIXED_TYPE \
            shadow_g_xvm_stack[g_xvm_sp] = THIS_IS_NOT_FP_NUM

        #define SET_AS_FLOAT_TYPE \
            shadow_g_xvm_stack[g_xvm_sp] = THIS_IS_FP_NUM

        #define SET_AS_THIS_TYPE \
            shadow_g_xvm_stack[g_xvm_sp]


        #define GET_THE_TYPE(type) \
            type = shadow_g_xvm_stack[g_xvm_sp - 1]

        #define GET_THE_TYPE_AT_BASE_OFFSET(type, offset) \
            type = shadow_g_xvm_stack[offset]

        #define GET_THE_TYPE_AT_TOP_OFFSET(type, offset) \
            type = shadow_g_xvm_stack[g_xvm_sp - offset]


        #define CHECK_FIXED_TYPES(cnt) \
                if(!check_fixed_pt_types(cnt)) { break; }

        #define CHECK_FLOAT_TYPES(cnt)  \
                if(!check_flting_pt_types(cnt)) { break; }
    #else
        #define SET_AS_FIXED_TYPE
        #define SET_AS_FLOAT_TYPE
        #define SET_AS_THIS_TYPE

        #define GET_THE_TYPE(type)
        #define GET_THE_TYPE_AT_BASE_OFFSET(type, offset)
        #define GET_THE_TYPE_AT_TOP_OFFSET(type, offset)

        #define CHECK_FIXED_TYPES(cnt)
        #define CHECK_FLOAT_TYPES(cnt)
    #endif
#else
    #define SET_AS_FIXED_TYPE
    #define SET_AS_FLOAT_TYPE
    #define SET_AS_THIS_TYPE

    #define GET_THE_TYPE(type)
    #define GET_THE_TYPE_AT_BASE_OFFSET(type, offset)
    #define GET_THE_TYPE_AT_TOP_OFFSET(type, offset)

    #define CHECK_FIXED_TYPES(cnt)
    #define CHECK_FLOAT_TYPES(cnt)
#endif

#define ONE_CELL            1
#define TWO_CELLS           2
#define THREE_CELLS         3
#define FOUR_CELLS          4
#define FIVE_CELLS          5
#define SIX_CELLS           6

#define SP_RESET            0


enum {
    TOKEN_NOOP              = 0,
    TOKEN_DROP,
    TOKEN_TWO_DROP,
    TOKEN_DUP,
    TOKEN_TWO_DUP,
    TOKEN_SWAP,
    TOKEN_PICK,
    TOKEN_TWO_SWAP,
    TOKEN_OVER,
    TOKEN_TWO_OVER,
    TOKEN_ROT,                      // 10

    TOKEN_TWO_ROT,
    TOKEN_NIP,
    TOKEN_TUCK,
    TOKEN_TO_RSTACK,
    TOKEN_TWO_TO_RSTACK,
    TOKEN_FROM_RSTACK,
    TOKEN_TWO_FROM_RSTACK,
    TOKEN_FETCH_RSTACK,
    TOKEN_TWO_FETCH_RSTACK,
    TOKEN_FETCH,                    // 20

    TOKEN_STORE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_XOR,
    TOKEN_NOT,
    TOKEN_LSHIFT,
    TOKEN_RSHIFT,
    TOKEN_INC,
    TOKEN_DEC,
    TOKEN_ADD,                      // 30

    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_MULDIV,
    TOKEM_MOD,
    TOKEN_DIVMOD,
    TOKEN_NEGATE,
    TOKEN_ABS,
    TOKEN_MIN,
    TOKEN_MAX,                      // 40

    TOKEN_GT,
    TOKEN_LT,
    TOKEN_GE,
    TOKEN_LE,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_EQ0,
    TOKEN_NEQ0,
    TOKEN_GT0,
    TOKEN_LT0,                      // 50

    TOKEN_PC,
    TOKEN_SP,
    TOKEN_RP,
    TOKEN_FLUSH,
#if HAVE_PRINTF
    TOKEN_PRINT,
    TOKEN_PRINT_STR,
    TOKEN_PRINT_LINEFEED,
#endif
    TOKEN_LIT0,
    TOKEN_LIT1,
    TOKEN_LIT2,                     // 60

    TOKEN_LIT3,
    TOKEN_LIT4,
    TOKEN_LIT5,
    TOKEN_LIT6,
    TOKEN_LIT7,
    TOKEN_LIT8,
    TOKEN_LIT9,
    TOKEN_LIT10,
    TOKEN_LIT11,
    TOKEN_LIT12,                    // 70

    TOKEN_LIT13,
    TOKEN_LIT14,
    TOKEN_LIT15,
    TOKEN_LIT_MINUS1,
    TOKEN_GOTO,
    TOKEN_CALL,
    TOKEN_RETURN,
    TOKEN_JZ,
    TOKEN_JNZ,
    TOKEN_RETZ,                    // 80

    TOKEN_RETNZ,
    TOKEN_LIT,
    TOKEN_SJMP,
    TOKEN_SJZ,
    TOKEN_SJNZ,
    TOKEN_LLIT,
    TOKEN_LGOTO,
    TOKEN_LJMP,
    TOKEN_LCALL,
    TOKEN_LJZ,                     // 90

    TOKEN_LJNZ,                     // 91
#if HAVE_FP_NUMBERS
    TOKEN_LLITFP,
    TOKEN_FINC,
    TOKEN_FADD,
    TOKEN_FSUB,
    TOKEN_FMUL,
    TOKEN_FDIV,
    TOKEN_FMULDIV,                  // 98




    TOKEM_FMOD,
    TOKEN_FDIVMOD,
    TOKEN_FNEGATE,
    TOKEN_FABS,
    TOKEN_FMIN,
    TOKEN_FMAX,
    TOKEN_FGT,
    TOKEN_FLT,
    TOKEN_FGE,
    TOKEN_FLE,
    TOKEN_FEQ,
    TOKEN_FNEQ,
    TOKEN_FEQ0,
    TOKEN_FNEQ0,
    TOKEN_FGT0,
    TOKEN_FLT0,
#endif
#if HAVE_FFI
    TOKEN_FFICALL,                  // 92 or 99
#endif
    ERROR,
    MAX_TOKEN_NO,
    BYTE_CODE_STRM_END,
    TOKEN_ILLEGAL = BYTE_CODE_STRM_END
};


#ifdef __cplusplus
extern "C"{
#endif

    extern uint32_t g_xvm_ram[XVM_U32_MAX_RAM + XVM_U32_MAX_VARS];

    extern uint8_t g_xvm_code[XVM_U8_MAX_CODE];
    extern uint8_t* g_xvm_codeptr;
    extern uint32_t g_xvm_max_code;
#if HAVE_PRINTF
    extern int8_t   g_xvm_str[XVM_I8_MAX_STR];
    extern uint32_t g_xvm_strndx;
#endif
    extern uint32_t g_xvm_stack[XVM_U32_MAX_STACK];
    extern uint32_t g_xvm_rstack[XVM_U32_MAX_STACK];

    extern uint32_t* const g_xvm_vars;

    extern int32_t g_xvm_sp;
    extern int32_t g_xvm_rp;
    extern uint32_t g_xvm_pc;
    extern int8_t g_xvm_run_stat;

    extern void xvm_run(void);
    extern void xvm_restart(void);

    /**
     * Pushes an item on to the data stack.
     *
     * @param data the item to be pushed
     * @return void
     */
    static inline void xvm_push(uint32_t data)
    {
        g_xvm_stack[g_xvm_sp] = data;
        g_xvm_sp += 1;
    }

    /**
     * Pops an item off of the data stack.
     *
     * @param void
     * @return the popped item
     */
    static inline uint32_t xvm_pop(void)
    {
        uint32_t x = 0;

        g_xvm_sp -= 1;

        x = g_xvm_stack[g_xvm_sp];

    #if HAVE_FP_NUMBERS
        #ifdef CONSOLE_MODE
            shadow_g_xvm_stack[g_xvm_sp] = THIS_IS_NOT_FP_NUM;
        #endif
    #endif

        return x;
    }

#ifdef __cplusplus
}
#endif

#endif // XVM_INNER_H

// end of file
