/**@file lexer.re
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
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xvm.h"
#include "xvm_inner.h"
#include "lexer.h"
#include "directives.h"
#include "compiler.h"
#include "voc.h"
#include "code.h"
#include "files.h"
#include "macro.h"
#include "variable.h"
#include "ffi.h"


#define YYCTYPE         int8_t
#define YYCURSOR        str
#define YYLIMIT         str + cnt
#define YYMARKER        marker
/*#define YYCTXMARKER*/

#define YYFILL(n)

int32_t lexer(int8_t* str, int32_t cnt)
{
    /* Obviously this makes the system non-reentrant */
#if HAVE_FFI
    static int32_t  ffi_lib_state   = NOT_OK_TO_IMPORT_FUNC;
#endif
    static int32_t  prev_state      = OK_TO_COMPILE;
    static int32_t  curr_state      = OK_TO_COMPILE;
    static bool     add_word        = false;
#if HAVE_PRINTF
    int32_t         tmp             = 0;
#endif
    int8_t*         str_start       = 0;
    int8_t*         marker          = 0;

    if(curr_state == BLK_CMT)
        goto c_blk_comment;

    if(curr_state != WORD_DEF && curr_state != IMPORT_FUNCTION)
    {
//        printf("\n OK_TO_COMPILE \n");
        curr_state = OK_TO_COMPILE;
    }

    for(;;)
    {

/*
    \0  : Null character
    \a  : Bell
    \b  : Backspace
    \t  : Horizontal tab
    \n  : Kine feed
    \v  : Vertical tab
    \f  : Form feed
    \r  : Carriage return
    \e  : Escape

    Note that some of the rules may use identifiers in their raw form, e.g.,
    you may see a newline in the form "\n" used rather than its token definition
    "NEWLINE" emdedded in a rule. That's because in most cases token definitions
    have an action associated with it, as defined within the parenthesis, that
    would be undesirable for the current process.

    Internal state, which is returned to the caller. Obviously this makes
    the system non-reentrant and not thread safe - you've been warned.
    ------------------------------------------------------------------------
    BYE                 (-1)
    WORD_DEF            (-2)
    BLK_CMT             (-3)
    LINE_CMT            (-4)
    LINE_CMT_END        (-5)
    OK_TO_COMPILE       (-6)
    PRINTED_HELP        (-7)
    ERROR_OCCURED       (-8)

    Items that can change the internal state:
        - the initial entry in to the lexer function
        - the start of comment
        - the end of comment
        - the start of word definition
        - the end of word definition


    We assume the print directives are used within a wrod definition, i.e.
    the value or string is compiled.
*/

/*
     Save the current pointer address. This will hold the start
     address of a token (when one is found), 'str - 1' is the
     end address of the token string.
*/
    str_start = str;
//printf("str_start: %s\n", str_start);

/*!re2c

        ANY                 = [^] ;                     /* Match anything */
        BLK_CMMT_START      = "/*\040" ;
        BLK_CMMT_END        = "*/\040" ;
        CPP_CMMT_START      = "//" ;
        FORTH_LINE_CMMT     = "\\\\" ;

 /*       WORD_IDENTIFIER     = ([!-/:-@[-`{-~_a-zA-Z0-9]+"\040") ; */
        WORD_IDENTIFIER     = ([\[\]!-/:;,\"-@$%^&*+=?><|-`'}{~_a-zA-Z0-9]+"\040") ;

/*        FORTH_WORDDEF_CMMT  = ("\(" [!-~] "\)""\040") ; */
        FORTH_WORDDEF_CMMT  = ("\("+[!-/\040:-@[-`{-~a-zA-Z0-9]+"\)\040") ;

        LINEFEED            = "\n" ;
        WHITESPACE          = [\a\b\t\v\f\r\040 ] ;      /* bell, backspace, horizontal tab, vertical tab, formfeed, carriage return, space character */
        DECI                = [0-9] ;                    /* Decimal: 0 thru 9 */
        OCT                 = [0-7] ;                    /* Decimal: 0 thru 9 */
        ALPHA               = [a-zA-Z] ;                 /* Alphanumeric: 'a' thru 'z' or 'A' thru 'Z' */
        HEXA                = [a-fA-F0-9] ;              /* Hex: 'a' thru 'f' or 'A' thru 'F' or '0' thru '9' */

        /* don't include embedded quotes */
 /*       PRINT_STR_DEF  = (".\"\040"+[!-/\040:-@[-`{-~a-zA-Z0-9]+"\"\040") ; */
        PRINT_STR_DEF  = (".\"\040"+[.!@$%\^&\*\-/\040:[`}{~a-zA-Z0-9]+"\"\040") ;

/*
        PRINT_STR_DEF  = (".\""+[!-/\040:-@[-`{-~a-zA-Z0-9]+"\"\040") ;
*/

*/

/*!re2c

        LINEFEED                            {
                                                if(curr_state == VAR_DEF_VAL)
                                                    curr_state = VAR_DEF_END;
                                                break;
                                            }
        WHITESPACE                          { str += 1; continue; }

        BLK_CMMT_START                      { prev_state = curr_state; curr_state = BLK_CMT; goto c_blk_comment; }
        CPP_CMMT_START                      { prev_state = curr_state; curr_state = LINE_CMT; goto cpp_line_comment; }
        FORTH_LINE_CMMT                     { prev_state = curr_state; curr_state = LINE_CMT; goto forth_line_comment; }

        "HELP\040"                          { curr_state = WORD_HELP; d_help(str); break; }
        "help\040"                          { curr_state = WORD_HELP; d_help(str); break; }

        "HELPALL\040"                       { curr_state = PRINTED_HELP; d_helpall(); break; }
        "helpall\040"                       { curr_state = PRINTED_HELP; d_helpall(); break; }

        "IF\040"                            { d_if(); continue; }
        "if\040"                            { d_if(); continue; }

        "ELSE\040"                          { d_else(); continue; }
        "else\040"                          { d_else(); continue; }

        "THEN\040"                          { d_then(); continue; }
        "then\040"                          { d_then(); continue; }

        "BEGIN\040"                         { d_begin(); continue; }
        "begin\040"                         { d_begin(); continue; }

        "UNTIL\040"                         { d_until(); continue; }
        "until\040"                         { d_until(); continue; }

        "DO\040"                            { d_do(); continue; }
        "do\040"                            { printf("\nxxx\n"); d_do(); continue; }

        "LOOP\040"                          { d_loop(); continue; }
        "loop\040"                          { d_loop(); continue; }

        "I\040"                             { d_i(); continue; }
        "i\040"                             { d_i(); continue; }

        "J\040"                             { d_j(); continue; }
        "j\040"                             { d_j(); continue; }

        "LEAVE\040"                         { d_leave(); continue; }
        "leave\040"                         { d_leave(); continue; }

        ":\040"                             { curr_state = WORD_DEF; add_word = true; continue; }

        ";\040"                             { curr_state = OK_TO_COMPILE; add_word = false; d_word_def_end(); continue; }

        "WORDS\040"                         { d_words(); continue; }
        "words\040"                         { d_words(); continue; }

        "VARIABLES\040"                     { d_variables(); continue; }
        "variables\040"                     { d_variables(); continue; }

        "REMOVE\040"                        { curr_state = WORD_REMOVE; continue; }
        "remove\040"                        { curr_state = WORD_REMOVE; continue; }

/*
        "VOCS\040"                          { d_vocs(); continue; }
        "vocs\040"                          { d_vocs(); continue; }

        "ORDER\040"                         { d_order(); continue; }
        "order\040"                         { d_order(); continue; }

        "VOCABULARY\040"                    { curr_state = VOC_DEF; continue; }
        "vocabulary\040"                    { curr_state = VOC_DEF; continue; }

        "ALSO\040"                          { d_also(); continue; }
        "also\040"                          { d_also(); continue; }

        "ONLY\040"                          { d_only(); continue; }
        "only\040"                          { d_only(); continue; }

        "DEFINITIONS\040"                   { d_vocdef(); continue; }
        "definitions\040"                   { d_vocdef(); continue; }
*/

        "IMPORT\040"                        {
#if HAVE_FFI
                                                curr_state = IMPORT_LIBRARY; ffi_lib_state = NOT_OK_TO_IMPORT_FUNC; continue;
#else
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "import\040"                        {
#if HAVE_FFI
                                                curr_state = IMPORT_LIBRARY; ffi_lib_state = NOT_OK_TO_IMPORT_FUNC; continue;
#else
                                                curr_state = ERROR_OCCURED;
#endif
                                            }

        "IMPORTEND\040"                    {
#if HAVE_FFI
                                                if(curr_state == IMPORT_FUNCTION)
                                                {

                                                    if(ffi_lib_state == OK_TO_IMPORT_FUNC)
                                                    {
                                                        ffi_import_end();
                                                    }

                                                    curr_state = IMPORT_END;
                                                }
                                                else
                                                {
                                                    curr_state = ERROR_OCCURED;
                                                    printf("ERROR: IMPORT_END identifier without an IMPORT.\n");
                                                    compiler_error();
                                                }
#else
                                                curr_state = ERROR_OCCURED;
#endif
                                                break;
                                            }
        "importend\040"                    {
#if HAVE_FFI
                                                if(curr_state == IMPORT_FUNCTION)
                                                {
                                                    if(ffi_lib_state == OK_TO_IMPORT_FUNC)
                                                    {
                                                        ffi_import_end();
                                                    }

                                                    curr_state = IMPORT_END;
                                                }
                                                else
                                                {
                                                    curr_state = ERROR_OCCURED;
                                                    printf("ERROR: import_end identifier without an import.\n");
                                                    compiler_error();
                                                }
#else
                                                curr_state = ERROR_OCCURED;
#endif
                                                break;
                                            }

        "FILE\040"                          { curr_state = OPEN_TXT_FILE; continue; }
        "file\040"                          { curr_state = OPEN_TXT_FILE; continue; }

        "FILEB\040"                         { curr_state = OPEN_BIN_FILE; continue; }
        "fileb\040"                         { curr_state = OPEN_BIN_FILE; continue; }

        "INCLUDE\040"                       { curr_state = INCLUDE_FILE; continue; }
        "include\040"                       { curr_state = INCLUDE_FILE; continue; }

        "#INCLUDE\040"                      { curr_state = INCLUDE_FILE; continue; }
        "#include\040"                      { curr_state = INCLUDE_FILE; continue; }

        "COMPA\040"                         { curr_state = SAVE_TO_ARRAY; continue; }
        "compa\040"                         { curr_state = SAVE_TO_ARRAY; continue; }

        "COMPB\040"                         { curr_state = SAVE_TO_BIN_FILE; continue; }
        "compb\040"                         { curr_state = SAVE_TO_BIN_FILE; continue; }

        "DEFINE\040"                        { curr_state = MACRO_DEF_NAME; continue; }
        "define\040"                        { curr_state = MACRO_DEF_NAME; continue; }

        "#DEFINE\040"                       { curr_state = MACRO_DEF_NAME; continue; }
        "#define\040"                       { curr_state = MACRO_DEF_NAME; continue; }

        "VARIABLE\040"                      { curr_state = VAR_DEF_NAME; continue; }
        "variable\040"                      { curr_state = VAR_DEF_NAME; continue; }

        "RESET\040"                         { d_reset(); continue; }
        "reset\040"                         { d_reset(); continue; }

        "DEBUG\040"                         { d_debug(); continue; }
        "debug\040"                         { d_debug(); continue; }

        "NODEBUG\040"                       { d_nodebug(); continue; }
        "nodebug\040"                       { d_nodebug(); continue; }

        "STEP\040"                          { d_step(); continue; }
        "step\040"                          { d_step(); continue; }

        "NOSTEP\040"                        { d_nostep(); continue; }
        "nostep\040"                        { d_nostep(); continue; }

        "STACKON\040"                       { d_showstackon(); continue; }
        "stackon\040"                       { d_showstackon(); continue; }

        "STACKOFF\040"                      { d_showstackoff(); continue; }
        "stackoff\040"                      { d_showstackoff(); continue; }

/*
        "RUN\040"                           { d_run(); continue; }
        "run\040"                           { d_run(); continue; }
*/
        "BASE\040"                          { d_base(); continue; }
        "base\040"                          { d_base(); continue; }

        "HEX\040"                           { d_hex(); continue; }
        "hex\040"                           { d_hex(); continue; }

        "DEC\040"                           { d_decimal(); continue; }
        "dec\040"                           { d_decimal(); continue; }

        "RECURSE\040"                       {
                                                if(curr_state != WORD_DEF)
                                                {
                                                    printf("ERROR: RECURSE is only valid within a word definition.\n");
                                                    compiler_error();
                                                    curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else if(g_current_word_def_addr == -1)
                                                {
                                                    printf("ERROR: No current valid word definition.\n");
                                                    compiler_error();
                                                    curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else
                                                {
                                                    d_recurse();
                                                    continue;
                                                }
                                            }
        "recurse\040"                       {
                                                if(curr_state != WORD_DEF)
                                                {
                                                    printf("ERROR: RECURSE is only valid within a word definition.\n");
                                                    compiler_error();
                                                    curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else if(g_current_word_def_addr == -1)
                                                {
                                                    printf("ERROR: No current valid word definition.\n");
                                                    compiler_error();
                                                    curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else
                                                {
                                                    d_recurse();
                                                    continue;
                                                }
                                            }

        "BYE\040"                           { curr_state = BYE; break; }
        "bye\040"                           { curr_state = BYE; break; }

        /* FFI format: return count : function name : parameter count     */
        ([0-1]+[" "]+[_a-zA-Z]+[" "]+[0-9]+[" "])   {
                                                        if(curr_state == IMPORT_FUNCTION)
                                                        {
#if HAVE_FFI
                                                            if(ffi_lib_state == OK_TO_IMPORT_FUNC)
                                                            {
                                                                if(!ffi_import_func(str_start, str - 1))
                                                                {
                                                                    curr_state = ERROR_OCCURED;
                                                                }
                                                            }
#else
                                                            curr_state = ERROR_OCCURED;
#endif
                                                            break;
                                                        }

                                                    }
        /*  Hexadecimal            Octal         Decimal     */
        ("0"[xX]HEXA+[" "])|("0"OCT+[" "])|([-+]?DECI+[" "])   {
                                                            if(curr_state == MACRO_DEF_VAL)
                                                            {
                                                                if(d_macro_value(str_start, (str - 1), MACRO_VAL_TYPE_SC))
                                                                    curr_state = MACRO_DEF_END;
                                                                else
                                                                    curr_state = ERROR_OCCURED;
                                                                break;
                                                            }
                                                            else if(curr_state == VAR_DEF_VAL)
                                                            {
#if HAVE_FP_NUMBERS
                                                                if(add_var_val(str_start, (str - 1), VAR_VAL_TYPE_SC))
#else
                                                                if(add_var_val(str_start, (str - 1)))
#endif
                                                                    curr_state = VAR_DEF_END;
                                                                else
                                                                    curr_state = ERROR_OCCURED;
                                                                break;
                                                            }
                                                            else
                                                            {
                                                                if(!compile_integer_str(str_start, (str - 1)))
                                                                    curr_state = ERROR_OCCURED;
                                                            }
                                                            continue;
                                                        }

        /* Floating point numbers */
        ([-+]?[0-9]+"\."?[0-9]+[" "])   {
#if HAVE_FP_NUMBERS
                                                            if(curr_state == MACRO_DEF_VAL)
                                                            {
                                                                if(d_macro_value(str_start, (str - 1), MACRO_VAL_TYPE_FP))
                                                                    curr_state = MACRO_DEF_END;
                                                                else
                                                                    curr_state = ERROR_OCCURED;
                                                                break;
                                                            }
                                                            else if(curr_state == VAR_DEF_VAL)
                                                            {
                                                                if(add_var_val(str_start, (str - 1), VAR_VAL_TYPE_FP))
                                                                    curr_state = VAR_DEF_END;
                                                                else
                                                                    curr_state = ERROR_OCCURED;
                                                                break;
                                                            }
                                                            else
                                                            {
                                                                compile_fp_str(str_start, (str - 1));
                                                            }
                                                            continue;
#else
                                                            printf("ERROR: Floating point numbers are not enabled in this build.\n");
                                                            compiler_error();
                                                            curr_state = ERROR_OCCURED;
                                                            break;
#endif
                                                        }

/*
        "S\"\040\""                           { printf("string def\n"); continue; }
        "s\"\040\""                           { printf("string def\n"); continue; }

        ".\"\040\""                           { printf("string def\n"); continue; }
*/
        PRINT_STR_DEF                       {
#if HAVE_PRINTF
                                                tmp = d_str_def(str_start, (str - 1));

                                                if(tmp != -1)
                                                {
                                                    compile_integer(tmp);
                                                    add_byte(TOKEN_PRINT_STR);
                                                }
                                                else
                                                {
                                                    curr_state = ERROR_OCCURED;
                                                }
                                                continue;
#else
                                                printf("ERROR: printf is not enabled in this build.\n");
                                                compiler_error();
#endif
                                            }

        "NOOP\040"                          { add_byte(TOKEN_NOOP); continue; }
        "noop\040"                          { add_byte(TOKEN_NOOP); continue; }
        "DROP\040"                          { add_byte(TOKEN_DROP); continue; }
        "drop\040"                          { add_byte(TOKEN_DROP); continue; }
        "2DROP\040"                         { add_byte(TOKEN_TWO_DROP); continue; }
        "2drop\040"                         { add_byte(TOKEN_TWO_DROP); continue; }
        "DUP\040"                           { add_byte(TOKEN_DUP); continue; }
        "dup\040"                           { add_byte(TOKEN_DUP); continue; }
        "2DUP\040"                          { add_byte(TOKEN_TWO_DUP); continue; }
        "2dup\040"                          { add_byte(TOKEN_TWO_DUP); continue; }
        "SWAP\040"                          { add_byte(TOKEN_SWAP); continue; }
        "swap\040"                          { add_byte(TOKEN_SWAP); continue; }
        "PICK\040"                          { add_byte(TOKEN_PICK); continue; }
        "pick\040"                          { add_byte(TOKEN_PICK); continue; }
        "2SWAP\040"                         { add_byte(TOKEN_TWO_SWAP); continue; }
        "2swap\040"                         { add_byte(TOKEN_TWO_SWAP); continue; }
        "OVER\040"                          { add_byte(TOKEN_OVER); continue; }
        "over\040"                          { add_byte(TOKEN_OVER); continue; }
        "2OVER\040"                         { add_byte(TOKEN_TWO_OVER); continue; }
        "2over\040"                         { add_byte(TOKEN_TWO_OVER); continue; }
        "ROT\040"                           { add_byte(TOKEN_ROT); continue; }
        "rot\040"                           { add_byte(TOKEN_ROT); continue; }
        "2ROT\040"                          { add_byte(TOKEN_TWO_ROT); continue; }
        "2rot\040"                          { add_byte(TOKEN_TWO_ROT); continue; }
        "NIP\040"                           { add_byte(TOKEN_NIP); continue; }
        "nip\040"                           { add_byte(TOKEN_NIP); continue; }
        "TUCK\040"                          { add_byte(TOKEN_TUCK); continue; }
        "tuck\040"                          { add_byte(TOKEN_TUCK); continue; }
        ">R\040"                            { add_byte(TOKEN_TO_RSTACK); continue; }
        ">r\040"                            { add_byte(TOKEN_TO_RSTACK); continue; }
        "2>R\040"                           { add_byte(TOKEN_TWO_TO_RSTACK); continue; }
        "2>r\040"                           { add_byte(TOKEN_TWO_TO_RSTACK); continue; }
        "R>\040"                            { add_byte(TOKEN_FROM_RSTACK); continue; }
        "r>\040"                            { add_byte(TOKEN_FROM_RSTACK); continue; }
        "2R>\040"                           { add_byte(TOKEN_TWO_FROM_RSTACK); continue; }
        "2r>\040"                           { add_byte(TOKEN_TWO_FROM_RSTACK); continue; }
        "R@\040"                            { add_byte(TOKEN_FETCH_RSTACK); continue; }
        "r@\040"                            { add_byte(TOKEN_FETCH_RSTACK); continue; }
        "2R@\040"                           { add_byte(TOKEN_TWO_FETCH_RSTACK); continue; }
        "2r@\040"                           { add_byte(TOKEN_TWO_FETCH_RSTACK); continue; }
        "@\040"                             { add_byte(TOKEN_FETCH); continue; }
        "!\040"                             { add_byte(TOKEN_STORE); continue; }
        "AND\040"                           { add_byte(TOKEN_AND); continue; }
        "and\040"                           { add_byte(TOKEN_AND); continue; }
        "OR\040"                            { add_byte(TOKEN_OR); continue; }
        "or\040"                            { add_byte(TOKEN_OR); continue; }
        "XOR\040"                           { add_byte(TOKEN_XOR); continue; }
        "xor\040"                           { add_byte(TOKEN_XOR); continue; }
        "NOT\040"                           { add_byte(TOKEN_NOT); continue; }
        "not\040"                           { add_byte(TOKEN_NOT); continue; }
        "<<\040"                            { add_byte(TOKEN_LSHIFT); continue; }
        ">>\040"                            { add_byte(TOKEN_RSHIFT); continue; }
        "1+\040"                            { add_byte(TOKEN_INC); continue; }
        "1-\040"                            { add_byte(TOKEN_DEC); continue; }
        "+\040"                             { add_byte(TOKEN_ADD); continue; }
        "-\040"                             { add_byte(TOKEN_SUB); continue; }
        "*\040"                             { add_byte(TOKEN_MUL); continue; }
        "/\040"                             { add_byte(TOKEN_DIV); continue; }
        "*/\040"                            { add_byte(TOKEN_MULDIV); continue; }
        "MOD\040"                           { add_byte(TOKEM_MOD); continue; }
        "mod\040"                           { add_byte(TOKEM_MOD); continue; }
        "/MOD\040"                          { add_byte(TOKEN_DIVMOD); continue; }
        "/mod\040"                          { add_byte(TOKEN_DIVMOD); continue; }
        "NEGATE\040"                        { add_byte(TOKEN_NEGATE); continue; }
        "negate\040"                        { add_byte(TOKEN_NEGATE); continue; }
        "ABS\040"                           { add_byte(TOKEN_ABS); continue; }
        "abs\040"                           { add_byte(TOKEN_ABS); continue; }
        "MIN\040"                           { add_byte(TOKEN_MIN); continue; }
        "min\040"                           { add_byte(TOKEN_MIN); continue; }
        "MAX\040"                           { add_byte(TOKEN_MAX); continue; }
        "max\040"                           { add_byte(TOKEN_MAX); continue; }
        ">\040"                             { add_byte(TOKEN_GT); continue; }
        "<\040"                             { add_byte(TOKEN_LT); continue; }
        ">=\040"                            { add_byte(TOKEN_GE); continue; }
        "<=\040"                            { add_byte(TOKEN_LE); continue; }
        "=\040"                             { add_byte(TOKEN_EQ); continue; }
        "!=\040"                            { add_byte(TOKEN_NEQ); continue; }
        "=0\040"                            { add_byte(TOKEN_EQ0); continue; }
        "!=0\040"                           { add_byte(TOKEN_NEQ0); continue; }
        ">0\040"                            { add_byte(TOKEN_GT0); continue; }
        "<0\040"                            { add_byte(TOKEN_LT0); continue; }
        "PC\040"                            { add_byte(TOKEN_PC); continue; }
        "pc\040"                            { add_byte(TOKEN_PC); continue; }
        "SP\040"                            { add_byte(TOKEN_SP); continue; }
        "sp\040"                            { add_byte(TOKEN_SP); continue; }
        "RP\040"                            { add_byte(TOKEN_RP); continue; }
        "rp\040"                            { add_byte(TOKEN_RP); continue; }
        "FLUSH\040"                         { add_byte(TOKEN_FLUSH); continue; }
        "flush\040"                         { add_byte(TOKEN_FLUSH); continue; }
        ".\040"                             {
#if HAVE_PRINTF
                                                add_byte(TOKEN_PRINT); continue;
#else
                                                printf("ERROR: printf is not enabled in this build.\n");
                                                compiler_error();
#endif
                                            }

        "LF\040"                            {
#if HAVE_PRINTF
                                                add_byte(TOKEN_PRINT_LINEFEED); continue;
#else
                                                printf("ERROR: printf is not enabled in this build.\n");
                                                compiler_error();
#endif
                                            }
        "lf\040"                            {
#if HAVE_PRINTF
                                                add_byte(TOKEN_PRINT_LINEFEED); continue;
#else
                                                printf("ERROR: printf is not enabled in this build.\n");
                                                compiler_error();
#endif
                                            }

        "F1+\040"                           {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FINC); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "f1+\040"                           {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FINC); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "F+\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FADD); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "f+\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FADD); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "F-\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FSUB); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "f-\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FSUB); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "F*\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FMUL); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "f*\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FMUL); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "F/\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FDIV); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "f/\040"                            {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FDIV); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "F*/\040"                           {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FMULDIV); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }
        "f*/\040"                           {
#if HAVE_FP_NUMBERS
                                                add_byte(TOKEN_FMULDIV); continue;
#else
                                                printf("ERROR: Floating point operations are not enabled in this build.\n");
                                                compiler_error();
                                                curr_state = ERROR_OCCURED;
#endif
                                            }


        FORTH_WORDDEF_CMMT                  {
                                                if(curr_state == WORD_DEF)
                                                {
                                                    add_worddef_cmt(str_start, (str -1));
                                                }
                                                continue;
                                            }

        /*
            Currently the "end" string pointer being passed to a function isn't modified,
            i.e. it points to the start of the next item!!!!
         */
        WORD_IDENTIFIER                     {
                                                if(curr_state == WORD_DEF)
                                                {
                                                    // Are we adding a new word definition?
                                                    if(add_word == true)
                                                    {
                                                        add_word = false;
                                                        d_word_def(str_start, (str - 1));
                                                    }
                                                    else
                                                    {
                                                        if(compile_word(str_start, (str - 1)) < 0)
                                                            curr_state = ERROR_OCCURED;
                                                    }
                                                }
                                                else if(curr_state == IMPORT_LIBRARY)
                                                {
#if HAVE_FFI
                                                    /*
                                                        if there's an error importing the library
                                                        we keep the parser going normally by setting
                                                       state equal to IMPORT_FUNCTION but we set
                                                       the ffi_lib_state to NOT_OK_TO_IMPORT_FUNC
                                                    */
                                                    if(!ffi_import_lib(str_start, str - 1))
                                                    {
                                                        ffi_lib_state = NOT_OK_TO_IMPORT_FUNC;
                                                    }
                                                    else
                                                    {
                                                        ffi_lib_state = OK_TO_IMPORT_FUNC;
                                                    }
#else
                                                    printf("ERROR: FFI interface is not enabled in this build.\n");
                                                    compiler_error();
#endif
                                                    curr_state = IMPORT_FUNCTION;
                                                    break;
                                                }
                                                else if(curr_state == SAVE_TO_ARRAY)
                                                {
                                                    /* pass the PFE */
                                                    if(!d_compa(str_start, str - 1))
                                                        curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else if(curr_state == SAVE_TO_BIN_FILE)
                                                {
                                                    /* pass the PFE */
                                                    if(!d_compb(str_start, str - 1))
                                                        curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else if(curr_state == OPEN_BIN_FILE)
                                                {
                                                    if(!d_bin_file_run(str_start, str - 1))
                                                        curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else if(curr_state == OPEN_TXT_FILE)
                                                {
                                                    if(!d_include(str_start, str - 1))
                                                        curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else if(curr_state == INCLUDE_FILE)
                                                {
                                                    if(!d_include(str_start, str - 1))
                                                        curr_state = ERROR_OCCURED;
                                                    break;
                                                }
                                                else if(curr_state == VOC_DEF)
                                                {
                                                    if(!d_newvoc(str_start, (str - 1)))
                                                        curr_state = ERROR_OCCURED;
                                                }
                                                else if(curr_state == MACRO_DEF_NAME)
                                                {
                                                    if(d_macro_name(str_start, (str - 1)))
                                                        curr_state = MACRO_DEF_VAL;
                                                    else
                                                        curr_state = ERROR_OCCURED;
                                                }
                                                else if(curr_state == VAR_DEF_NAME)
                                                {
                                                    if(d_var_name(str_start, (str - 1)))
                                                        curr_state = VAR_DEF_VAL;
                                                    else
                                                        curr_state = ERROR_OCCURED;
                                                }
                                                else if(curr_state == WORD_REMOVE)
                                                {
                                                    d_remove_word(str_start, (str - 1));
                                                    curr_state = WORD_REMOVE_END;
                                                }
/*
                                                else if(curr_state == STRING_DEF)
                                                {
printf("-- %s\n", str_start);
                                                    if(utmp = d_str_def(str_start, (str - 1)))
                                                    {
                                                        compile_integer_str((int8_t*) &utmp);
                                                        curr_state = STRING_DEF_END;
                                                    }
                                                    else
                                                        curr_state = ERROR_OCCURED;
                                                }
*/
                                                else
                                                {
                                                    if(compile_word(str_start, str - 1) < 0)
                                                        curr_state = ERROR_OCCURED;
                                                }
                                                continue;
                                            }

    /*    ANY                                 { str += 1; continue; } */


*/

c_blk_comment:

        /*
            Skip everything between the start and end of the C comment. The end
            of the C comment serves as a termination boundary. Process any new
            lines the same as is done above.
         */

/*!re2c

        BLK_CMMT_END                    { curr_state = prev_state; continue; }
        LINEFEED                        { break; }
        ANY                             { goto c_blk_comment; }

*/

cpp_line_comment:

        /*
            Skip everything after the start of the CPP comment but before a new
            line. The new ine serves as a termination boundary. Process the new
            line the same as is done above.
         */

/*!re2c

        LINEFEED                        { curr_state = prev_state; break; }
        ANY                             { goto cpp_line_comment; }

*/

forth_line_comment:

        /*
            Skip everything after the start of the Forth line comment but before
            a new line. The new ine serves as a termination boundary. Process the
            new line the same as is done above.
         */

/*!re2c

        LINEFEED                        { curr_state = prev_state; break; }
        ANY                             { goto forth_line_comment; }

*/

    } /* for(;;) */

    return curr_state;
}

/* end of file */

