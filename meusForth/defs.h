/**@file lexer.h
 *
 * @author Original author: Joseph D Poirier
 * @date   Creation date  : 16 August 2008
 *
 *
 * @version X.X.X
 *
 * VERSION    DATE/AUTHOR              COMMENT
 *            15Sept2007 Jpoirier         Added initial version information.
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
#ifndef DEFS_H
#define DEFS_H

#define PRINT_LEXED_TOKENS          0


// Standard printf
#ifdef NPRINTF
    #define PRINTF(fmt)
    #define PRINTF_VA(fmt, ...)
#else
    #define PRINTF(fmt)                printf(fmt)
    #define PRINTF_VA(fmt, ...)        printf(fmt, __VA_ARGS__)
#endif

// Debug printf
#ifdef NDEBUG
    #define DPRINTF(fmt)
    #define DPRINTF_VA(fmt, ...)
#else
    #define DPRINTF(fmt)                printf(fmt)
    #define DPRINTF_VA(fmt, ...)        printf(fmt, __VA_ARGS__)
#endif

// Error printf
#ifdef NEPRINTF
    #define EPRINTF(fmt)
    #define EPRINTF_VA(fmt, ...)
#else
    #define EPRINTF(fmt)                printf(fmt)
    #define EPRINTF_VA(fmt, ...)        printf(fmt, __VA_ARGS__)
#endif

// Don't conflict with ASCII values, therefore, start after 255.
#define TOKENS_LOWER_BOUND          (256)

// Start index should be zero.
#define TOKEN_SCALE_VAL             (TOKENS_LOWER_BOUND + 1)

// Start index should be 1
#define PARSER_SCALE_VAL            (TOKENS_LOWER_BOUND)
#define END_OF_TOKEN_STREAM         (0)


    // (1) Define code generating macro
#define GENERATE_TOKEN_IDS                                  \
        XMACRO(IDENT,           "IDENT          ",)         \
                                                            \
        XMACRO(PRINT,           "PRINT          ",)         \
        XMACRO(MALLOC,          "MALLOC         ",)         \
        XMACRO(FREE,            "FREE           ",)         \
                                                            \
        XMACRO(COLON,           "COLON          ",)         \
        XMACRO(COMMA,           "COMMA          ",)         \
        XMACRO(EQUALS,          "EQUALS         ",)         \
        XMACRO(EQEQ,            "EQEQ           ",)         \
        XMACRO(LBRACKET,        "LBRACKET       ",)         \
        XMACRO(LPAREN,          "LPAREN         ",)         \
        XMACRO(LCURLY,          "LCURLY         ",)         \
        XMACRO(PERIOD,          "PERIOD         ",)         \
        XMACRO(QUESTIONMARK,    "QUESTIONMARK   ",)         \
        XMACRO(RCURLY,          "RCURLY         ",)         \
        XMACRO(RPAREN,          "RPAREN         ",)         \
        XMACRO(RBRACKET,        "RBRACKET       ",)         \
        XMACRO(RETURN,          "RETURN         ",)         \
        XMACRO(SEMICOLON,       "SEMICOLON      ",)         \
                                                            \
        XMACRO(BREAK,           "BREAK          ",)         \
        XMACRO(CASE,            "CASE           ",)         \
        XMACRO(CONTINUE,        "CONTINUE       ",)         \
        XMACRO(DO,              "DO             ",)         \
        XMACRO(ELSE,            "ELSE           ",)         \
        XMACRO(FOR,             "FOR            ",)         \
        XMACRO(IF,              "IF             ",)         \
        XMACRO(SWITCH,          "SWITCH         ",)         \
        XMACRO(WHILE,           "WHILE          ",)         \
                                                            \
        XMACRO(TRUE,            "TRUE           ",)         \
        XMACRO(FALSE,           "FALSE          ",)         \
        XMACRO(SCON,            "SCON           ",)         \
        XMACRO(FCON,            "FCON           ",)         \
        XMACRO(ICON,            "ICON           ",)         \
                                                            \
        XMACRO(ARRAY,           "ARRAY          ",)         \
        XMACRO(VOID,            "VOID           ",)         \
        XMACRO(F32_T,           "F32_T          ",)         \
        XMACRO(F64_T,           "F64_T          ",)         \
        XMACRO(S8_T,            "S8_T           ",)         \
        XMACRO(U8_T,            "U8_T           ",)         \
        XMACRO(S16_T,           "S16_T          ",)         \
        XMACRO(U16_T,           "U16_T          ",)         \
        XMACRO(S32_T,           "S32_T          ",)         \
        XMACRO(U32_T,           "U32_T          ",)         \
        XMACRO(S64_T,           "S64_T          ",)         \
        XMACRO(U64_T,           "U64_T          ",)         \
                                                            \
        XMACRO(LOGAND,          "LOGAND         ",)         \
        XMACRO(LOGOR,           "LOGOR          ",)         \
                                                            \
        XMACRO(EQLEQL,          "EQLEQL         ",)         \
        XMACRO(GTHAN,           "GTHAN          ",)         \
        XMACRO(GEQ,             "GEQ            ",)         \
        XMACRO(LEQ,             "LEQ            ",)         \
        XMACRO(LTHAN,           "LTHAN          ",)         \
        XMACRO(NEQ,             "NEQ            ",)         \
                                                            \
        XMACRO(BITAND,          "BITAND         ",)         \
        XMACRO(BITANDEQ,        "BITANDEQ       ",)         \
        XMACRO(BITLSHIFT,       "BITLSHIFT      ",)         \
        XMACRO(BITLSHIFTEQ,     "BITLSHIFTEQ    ",)         \
        XMACRO(BITNOT,          "BITNOT         ",)         \
        XMACRO(BITOR,           "BITOR          ",)         \
        XMACRO(BITOREQ,         "BITOREQ        ",)         \
        XMACRO(BITRSHIFT,       "BITRSHIFT      ",)         \
        XMACRO(BITRSHIFTEQ,     "BITRSHIFTEQ    ",)         \
        XMACRO(BITXOR,          "BITXOR         ",)         \
        XMACRO(BITXOREQ,        "BITXOREQ       ",)         \
                                                            \
        XMACRO(LOGNOT,          "LOGNOT         ",)         \
                                                            \
        XMACRO(ADD,             "ADD            ",)         \
        XMACRO(ADDEQ,           "ADDEQ          ",)         \
        XMACRO(DECR,            "DECR           ",)         \
        XMACRO(DIV,             "DIV            ",)         \
        XMACRO(DIVEQ,           "DIVEQ          ",)         \
        XMACRO(INCR,            "INCR           ",)         \
        XMACRO(MOD,             "MOD            ",)         \
        XMACRO(MODEQ,           "MODEQ          ",)         \
        XMACRO(MULT,            "MULT           ",)         \
        XMACRO(MULTEQ,          "MULTEQ         ",)         \
        XMACRO(SUB,             "SUB            ",)         \
        XMACRO(SUBEQ,           "SUBEQ          ",)         \
                                                            \
        XMACRO(CONST,           "CONST          ",)         \
        XMACRO(ELLIPSIS,        "ELLIPSIS       ",)         \
        XMACRO(ENUM,            "ENUM           ",)         \
        XMACRO(EXCEPT,          "EXCEPT         ",)         \
        XMACRO(FINALLY,         "FINALLY        ",)         \
        XMACRO(GLOBAL,          "GLOBAL         ",)         \
        XMACRO(GOTO,            "GOTO           ",)         \
        XMACRO(NUKK,            "NULL           ",)         \
        XMACRO(IMPORT,          "IMPORT         ",)         \
        XMACRO(IN,              "IN             ",)         \
        XMACRO(PEND,            "PEND           ",)         \
        XMACRO(POINTER,         "POINTER        ",)         \
        XMACRO(SIZEOF,          "SIZEOF         ",)         \
        XMACRO(STRUCT,          "STRUCT         ",)         \
        XMACRO(TRY,             "TRY            ",)         \
        XMACRO(TYPEDEF,         "TYPEDEF        ",)         \
        XMACRO(UNION,           "UNION          ",)         \
                                                            \
        XMACRO(PP_INCLUDE,      "PP_INCLUDE     ",)         \
        XMACRO(PP_LINE,         "PP_LINE        ",)         \
        XMACRO(PP_DEFINE,       "PP_DEFINE      ",)         \
        XMACRO(PP_UNDEF,        "PP_UNDEF       ",)         \
        XMACRO(PP_IF,           "PP_IF          ",)         \
        XMACRO(PP_IFDEF,        "PP_IFDEF       ",)         \
        XMACRO(PP_IFNDEF,       "PP_IFNDEF      ",)         \
        XMACRO(PP_ELSE,         "PP_ELSE        ",)         \
        XMACRO(PP_ELIF,         "PP_ELIF        ",)         \
        XMACRO(PP_ENDIF,        "PP_ENDIF       ",)         \
        XMACRO(PP_ERROR,        "PP_ERROR       ",)

// TODO:
// -----
// Data types-
//      vs8 vu8, vs16, vu16, vs32, vu32, vs64, vu64, vf32, vf64,
//     tuple, map, list
//
// Instructions:
//

    // (2) Define X-Macro for generating enum members
    #define XMACRO(id, idString, val)     id,

    enum TOKEN_IDS
    {
        // Force ID's to start at 0.
        TOKEN_IDS_LOWERBOUND = TOKENS_LOWER_BOUND,
        GENERATE_TOKEN_IDS
        TOKEN_IDS_TOTAL
    };

    #undef XMACRO

    // See #e in xvmc.c


    // Example for printing textual info, see xvmc.c.
    // (3) Define X-Macro for generating string names





#ifdef __cplusplus
extern "C" {
#endif

    extern const char* tokenIdStrings[];

#ifdef __cplusplus
}
#endif

#endif  // DEFS_H

/* end of file */
