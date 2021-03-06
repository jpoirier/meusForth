/**@file directives.c
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
#include <ctype.h>
#include <assert.h>

#include "types.h"
#include "config.h"
#include "xvm.h"
#include "xvm_inner.h"
#include "code.h"
#include "compiler.h"
#include "parser.h"
#include "files.h"
#include "debug.h"
#include "macro.h"
#include "variable.h"
#include "directives.h"
#include "voc.h"
#include "token_info.h"

// used to save the system state of arrays
static uint32_t p_g_xvm_ram[XVM_U32_MAX_RAM];
#if HAVE_PRINTF
    static int8_t   p_g_xvm_str[XVM_I8_MAX_STR];
#endif
static uint32_t p_g_xvm_stack[XVM_U32_MAX_STACK];
static uint32_t p_g_xvm_rstack[XVM_U32_MAX_STACK];
static uint8_t  p_g_xvm_code[XVM_U8_MAX_CODE];
static uint32_t p_g_xvm_vars[XVM_U32_MAX_VARS];
static int8_t   m_def_comment[MAX_CHARS_TOKEN_INFO];


int8_t* g_dir_str = 0;

// DIRECTIVES_CNT should be some value larger than the items listed here.
directive_info directives[DIRECTIVES_CNT] =
{
  (int8_t*) "HELP",         (int8_t*) "Provides help on a specific word or directive. Syntax: \"HELP BLABLA\" where \"BLABLA\" is the word.",
  (int8_t*) "HELPALL",      (int8_t*) "List all the existing words and directives.",

  (int8_t*) "--clear",      (int8_t*) "Clear the terminal's input buffer history.",
  (int8_t*) "--list",       (int8_t*) "List the terminal's input buffer history.",
  (int8_t*) "--pwd",        (int8_t*) "Print the current working directory.",
  (int8_t*) "--cd dir",     (int8_t*) "Change the current working directory to the one specified by dir. E.g.> --cd ~/",
  (int8_t*) "--edit arg [optional]", (int8_t*) "Run the _terminal_ editor defined by arg and pass the following options . E.g.> --cmd nano ./myFile",

  (int8_t*) "\\\\",         (int8_t*) "Denotes a line comment.",
  (int8_t*) "//",           (int8_t*) "Denotes a line comment.",
  (int8_t*) "\\*",          (int8_t*) "Denotes the beginning of a block comment.",
  (int8_t*) "*/",           (int8_t*) "Denotes the end of a block comment.",
  (int8_t*) "( -- )",       (int8_t*) "Stack affect comment.",

  (int8_t*) "IF",           (int8_t*) "( flag -- ) Start of conditional 'IF' statement .. IF .. ELSE .. THEN.",
  (int8_t*) "ELSE",         (int8_t*) "( -- ) Optional part of conditional 'IF' statement .. IF .. ELSE .. THEN.",
  (int8_t*) "THEN",         (int8_t*) "( -- ) Finish of conditional 'IF' statement .. IF .. ELSE .. THEN.",

  (int8_t*) "BEGIN",        (int8_t*) "( -- ) Start of BEGIN ... UNTIL loop.",
  (int8_t*) "UNTIL",        (int8_t*) "( flag -- ) Finish of BEGIN ... UNTIL loop, where loop repeats if flag equals 0.",
  (int8_t*) "DO",           (int8_t*) "( Ne Ns -- ) ( R: -- Ne Ns ) Starts a counted loop. Cycle from Ns to Ne. Also see LOOP, I, J. Example: 10 0 DO .. LOOP.",
  (int8_t*) "LOOP",         (int8_t*) "( R: Ne Ns -- ) Finish a counted loop. Cycle from Ns to Ne. Also see DO, I, J.",
  (int8_t*) "I",            (int8_t*) "( -- x ) Put the inner loop counter on the stack. Also see DO, LOOP, I.",

  (int8_t*) "J",            (int8_t*) "( -- x ) Put the outer loop counter on the stack. Also see DO, LOOP, J.",
  (int8_t*) "LEAVE",        (int8_t*) "( -- ) Immediately exit the current loop. Also see DO, LOOP.",
  (int8_t*) ":",            (int8_t*) "( -- ) Start of a word definition. The following example defines the word FOO > : FOO 1 2 + ;",
  (int8_t*) ";",            (int8_t*) "( -- ) End of a word definition.",
  (int8_t*) "FILE",         (int8_t*) "Open the specified text file. Example: FILE my_file.f",
  (int8_t*) "FILEB",        (int8_t*) "Open the specified binary file. Example: FILE my_file.fd",

  (int8_t*) "INCLUDE",      (int8_t*) "Include the specified file. Files can be nested. Example: INCLUDE MyFile.f",
  (int8_t*) "#INCLUDE",     (int8_t*) "Include the specified file. Files can be nested. Example: #INCLUDE MyFile.f",
  (int8_t*) "COMPA",        (int8_t*) "Compiles vocabulary to a file in array format. Example: COMPA byte_array.c",
  (int8_t*) "COMPB",        (int8_t*) "Compiles vocabulary to a file in binary format. Example: COMB forthFile.mfd",
  (int8_t*) "DEFINE",       (int8_t*) "Create a simple 'C' like macro definition. Example: #define PortA 0x80",
  (int8_t*) "#DEFINE",      (int8_t*) "Create a simple 'C' like macro definition. Example: #define PortA 0x80",

  (int8_t*) "VARIABLE",     (int8_t*) "Create variable definition. Example: VARIABLE myVar",

  (int8_t*) "RESET",        (int8_t*) "Reset the compiler, forgetting vocabulary.",
  (int8_t*) "REMOVE",       (int8_t*) "Remove a word or variable from its respective list. Example: REMOVE myWord",
  (int8_t*) "DEBUG",        (int8_t*) "Switch on the debug mode.",
  (int8_t*) "NODEBUG",      (int8_t*) "Switch off the debug mode.",
  (int8_t*) "BASE",         (int8_t*) "Indicate the current base, HEXADECIMAL or DECIMAL.",
  (int8_t*) "HEX",          (int8_t*) "Change the base to HEXADECIMAL.",

  (int8_t*) "RECURSE",      (int8_t*) "Recursively calls the word that RECURSE is used in.",
  (int8_t*) "DEC",          (int8_t*) "Change the base to DECIMAL.",
  (int8_t*) ".\" \"",       (int8_t*) "Text string definition and print command. Example: .\" This is some text\"",
  (int8_t*) "WORDS",        (int8_t*) "List the words in the context vocabulary.",

  (int8_t*) "IMPORT",       (int8_t*) "Import a dynamic library. Example: IMPORT ./myLib.so",
  (int8_t*) "IMPORTEND",    (int8_t*) "End of dynamic library import.",

  (int8_t*) "STEP",         (int8_t*) "Execute one token at time.",
  (int8_t*) "NOSTEP",       (int8_t*) "Switch off the execution of one token at time.",

  (int8_t*) "STACKOFF",     (int8_t*) "Stop the stack contents from being printed out to the terminal window.",
  (int8_t*) "STACKON",      (int8_t*) "Print the stack contents out to the terminal window - default is on.",


/*
  (int8_t*) "RUN",          (int8_t*) "Execute all tokens",

  (int8_t*) "VOCS",         (int8_t*) "List all the vocabularies.",
  (int8_t*) "ORDER",        (int8_t*) "List the search order and the current vocabulary",
  (int8_t*) "VOCABULARY",   (int8_t*) "Create a new vocabulary and make it current. Example: VOCABULARY MyVoc",
  (int8_t*) "ALSO",         (int8_t*) "Add a vocabulary to the search order. Example: ALSO MyVoc",
  (int8_t*) "ONLY",         (int8_t*) "Clear the search order and set the context. Example: ONLY FORTH",
  (int8_t*) "DEFINITIONS",  (int8_t*) "The last vocabulary in the search order is made the current one",
*/
    0 , 0 // end of the list
};

/**
 *
 *
 * @param void
 * @return void
 */
void d_reset(void)
{
    reset_vocs();
    xvm_reset();
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return
 */
bool d_compa(int8_t* str_start,
             int8_t* str_end)
{
    bool status = false;

    if(!g_err_cnt)
    {
        set_exe_mode(EXE_TARGET);

        copy_con_to_target();

        g_can_execute = false;

        status = save_compiled(str_start,
                               str_end);
    }

    return status;
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return
 */
bool d_compb(int8_t* str_start,
             int8_t* str_end)
{
    bool status = false;

    if(!g_err_cnt)
    {
        set_exe_mode(EXE_TARGET);

        copy_con_to_target();

        g_can_execute = false;

        status = save_bin_file(str_start,
                               str_end);
    }

    return status;
}

/**
 *
 *
 * @param str_start the start of the PFE string
 * @param str_end the end of the PFE string
 * @return whether processing was successful (true) or not (false)
 */
bool d_bin_file_run(int8_t* str_start,
                    int8_t* str_end)
{
    FILE*           source_file     = 0;
    int32_t         pfe_len         = str_end - str_start;
    int32_t         len             = 0;
    int32_t         cnt             = 0;
    bool            status          = false;

    // used to save the system state of arrays
//    uint32_t*   p_g_xvm_ram         = 0;
//    uint32_t*   p_g_xvm_stack       = 0;
//    uint32_t*   p_g_xvm_rstack      = 0;
//    uint8_t*    p_g_xvm_code        = 0;
    uint8_t*    p_g_xvm_codeptr     = 0;
//    int8_t*     p_g_xvm_str         = 0;

    // used to save the system state of non-arrays
#if HAVE_PRINTF
    uint32_t    t_g_xvm_strndx      = 0;
#endif
    uint32_t    t_g_xvm_max_code    = 0;
    int32_t     t_g_xvm_sp          = 0;
    int32_t     t_g_xvm_rp          = 0;
    uint32_t    t_g_xvm_pc          = 0;
    uint16_t    chk_sum             = 0;
    uint16_t    t_u16               = 0;
    int8_t      t_g_xvm_run_stat    = 0;

    int8_t*     pfe                 = alloca(pfe_len + 1);

    if(pfe_len < PFE_MAX_CHARS)
    {
        // create a null terminated string for the file name
        strncpy((char*) pfe,
                (char const*) str_start,
                pfe_len);

        pfe[pfe_len] = '\0';

        source_file = (FILE*) fopen((char const*) pfe, "rb");

        if(!source_file)
        {
            status = false;

            printf("ERROR: the binary file does not exist.\n");
            compiler_error();

            return status;
        }
        else
        {
            // the 'status' was originally initialized to false here and
            // reset to true at the end of this code block but with the code
            // block being so long it's better if it's set explicitly within
            // the error handler.

            // save the system state for non-array types
#if HAVE_PRINTF
            t_g_xvm_strndx      = g_xvm_strndx;
#endif
            p_g_xvm_codeptr     = g_xvm_codeptr;
            t_g_xvm_max_code    = g_xvm_max_code;
            t_g_xvm_sp          = g_xvm_sp;
            t_g_xvm_rp          = g_xvm_rp;
            t_g_xvm_pc          = g_xvm_pc;
            t_g_xvm_run_stat    = g_xvm_run_stat;

            // initialize the system to pristine form
#if HAVE_PRINTF
            g_xvm_strndx        = 0;
#endif
            g_xvm_codeptr       = 0;
            g_xvm_max_code      = XVM_U8_MAX_CODE;
            g_xvm_sp            = 0;
            g_xvm_rp            = 0;
            g_xvm_pc            = 0;
            g_xvm_run_stat      = XVM_OK;

            //-------------------- XVM Code array
            memcpy(p_g_xvm_code, g_xvm_code, XVM_U8_MAX_CODE);

            //-------------------- Binary file handling
            fseek(source_file, 0, SEEK_END);

            len = ftell(source_file);

            fseek(source_file, 0, SEEK_SET);

            cnt = fread(g_xvm_code, sizeof(char), len, source_file);

            if(cnt != len)
            {
                status = false;

                printf("ERROR: problems reading the binary file.\n");
                compiler_error();

                fclose(source_file);

                goto end_proc;
            }

            fclose(source_file);

            // retrieve the end-of-byte-stream, i.e. the illegal token
            t_u16 = g_xvm_code[len - 3];

            if(t_u16 != 0xFF)
            {
                status = false;

                printf("ERROR: invalid or corrupt end-of-byte-stream token in the binary file.\n");
                compiler_error();

                goto end_proc;
            }

            // retrieve the appended 2-byte checksum value
            t_u16 = (uint16_t) ((((uint16_t) g_xvm_code[len - 2]) << 8) | ((uint16_t) g_xvm_code[len - 1]));

            // calculate a new checksum, exclude the end-of-byte-stream
            // token as well as the previously calculated checksum
            chk_sum = checksum(g_xvm_code, len - 3);

            // do the two checksum values match
            if(t_u16 != chk_sum)
            {
                status = false;

                printf("ERROR: checksum is invalid invalid.\n");
                compiler_error();

                goto end_proc;
            }

            //-----------------------------------------------------------------
            // A series of temporary copies being done as we progress

            //-------------------- XVM RAM array
            memcpy(p_g_xvm_ram, g_xvm_ram, sizeof(uint32_t) * XVM_U32_MAX_RAM);

            //-------------------- XVM Str array
#if HAVE_PRINTF
            memcpy(p_g_xvm_str, g_xvm_str, XVM_I8_MAX_STR);
#endif

            //-------------------- XVM Stack array
            memcpy(p_g_xvm_stack, g_xvm_stack, sizeof(uint32_t) * XVM_U32_MAX_STACK);

            //-------------------- XVM Rstack array
            memcpy(p_g_xvm_rstack, g_xvm_rstack, sizeof(uint32_t) * XVM_U32_MAX_STACK);

            //-------------------- XVM Var array
            memcpy(p_g_xvm_vars, g_xvm_vars, sizeof(uint32_t) * XVM_U32_MAX_VARS);

            //-------------------- Code Ptr assignment
            g_xvm_codeptr = g_xvm_code;

            xvm_run();

            status = true;
        }
    }
    else
    {
        printf("ERROR: The file PATH/FILE/EXTENSION exceeds the 1,024 character limit.\n");
        compiler_error();
    }

    memcpy(g_xvm_vars, p_g_xvm_vars, sizeof(uint32_t) * XVM_U32_MAX_VARS);
    memcpy(g_xvm_rstack, p_g_xvm_rstack, sizeof(uint32_t) * XVM_U32_MAX_STACK);
    memcpy(g_xvm_stack, p_g_xvm_stack, sizeof(uint32_t) * XVM_U32_MAX_STACK);
#if HAVE_PRINTF
    memcpy(g_xvm_str, p_g_xvm_str, XVM_I8_MAX_STR);
#endif
    memcpy(g_xvm_ram, p_g_xvm_ram, sizeof(uint32_t) * XVM_U32_MAX_RAM);

end_proc:

    memcpy(g_xvm_code, p_g_xvm_code, XVM_U8_MAX_CODE);

#if HAVE_PRINTF
    g_xvm_strndx        = t_g_xvm_strndx;
#endif
    g_xvm_codeptr       = p_g_xvm_codeptr;
    g_xvm_max_code      = t_g_xvm_max_code;
    g_xvm_sp            = t_g_xvm_sp;
    g_xvm_rp            = t_g_xvm_rp;
    g_xvm_pc            = t_g_xvm_pc;
    g_xvm_run_stat      = t_g_xvm_run_stat;

    return status;
}

/**
 *
 *
 * @param str_start the start of the PFE string
 * @param str_end the end of the PFE string
 * @return whether the file was included or not
 */
bool d_include(int8_t* str_start,
               int8_t* str_end)
{
    int32_t pfe_len     = str_end - str_start;
    bool    status      = false;
    int8_t* pfe         = alloca(pfe_len + 1);

    strncpy((char*) pfe,
            (char const*) str_start,
            pfe_len);

    pfe[pfe_len] = '\0';

    status = open_file(pfe);

    return status;
}

/**
 * Prints the number base currently in use.
 *
 * @param void
 * @return void
 */
void d_base(void)
{
    if(g_num_base == 10)
        printf("Base: DECIMAL\n");
    else
        printf("Base: HEX\n");
}

/**
 * Changes the number base to hex.
 *
 * @param void
 * @return void
 */
void d_hex(void)
{
    g_num_base = 16;
}

/**
 * Changes the number base to decimal
 *
 * @param void
 * @return void
 */
void d_decimal(void)
{
    g_num_base = 10;
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_recurse(void)
{
    compile_call((uint32_t) g_current_word_def_addr);
}


/**
 *
 *
 * @param str_start the start of the macro name string
 * @param str_end the end of the macro name string
 * @return
 */
bool d_macro_name(int8_t* str_start,
                  int8_t* str_end)
{
    int32_t cnt     = str_end - str_start;
    bool    status  = false;

    if(cnt > MAX_CHARS_MACRO_NAME)
    {
        printf("ERROR: Macro name cannot be longer than %d characters.\n", MAX_CHARS_MACRO_NAME);
        compiler_error();
    }
    else
    {
        status = false;

        memset(g_macro_name, '\0', MAX_CHARS_MACRO_NAME + 1);

        strncpy((char*) g_macro_name, (char const*) str_start, cnt);

// XXX: replace is_number with the following
//    errno = ~(ERANGE);
//    val = strtol((char*)  str, (char**) &end_ptr, 0);
//    fval = strtof((char*)  str, (char**) &end_ptr);
//    if(errno != ERANGE && val <= INT_MAX && val >= INT_MIN && end_ptr != str)

        if(is_number((int8_t*) g_macro_name))
        {
            printf("ERROR: a number cannot be used as a macro name.\n");
            compiler_error();
        }
        else
        {
            status = true;
        }
    }

    return status;
}

/**
 *
 *
 * @param str_start the start of the macro name string
 * @param str_end the end of the macro name string
 * @return
 */
bool d_var_name(int8_t* str_start,
                  int8_t* str_end)
{
    int32_t cnt     = str_end - str_start;
    bool    status  = false;

    if(cnt > MAX_CHARS_VAR_NAME)
    {
        printf("ERROR: Variable name cannot be longer than %d characters.\n", MAX_CHARS_VAR_NAME);
        compiler_error();
    }
    else
    {
        status = false;

        memset(g_var_name, '\0', MAX_CHARS_VAR_NAME + 1);

        strncpy((char*) g_var_name, (char const*) str_start, cnt);

// XXX: replace is_number with the following
//    errno = ~(ERANGE);
//    val = strtol((char*)  str, (char**) &end_ptr, 0);
//    fval = strtof((char*)  str, (char**) &end_ptr);
//    if(errno != ERANGE && val <= INT_MAX && val >= INT_MIN && end_ptr != str)

        if(is_number((int8_t*) g_var_name))
        {
            printf("ERROR: a number cannot be used as a variable name.\n");
            compiler_error();
        }
        else
        {
            add_var_def();
            status = true;
        }
    }

    return status;
}
#if HAVE_PRINTF
    /**
     *
     *
     * @param str_start
     * @param str_end
     * @return
     */
    int32_t d_str_def(int8_t* str_start,
                      int8_t* str_end)
    {
        int8_t* end     = str_end - 1; // set to point to the quote character
        int32_t cnt     = 0;
        int32_t status  = -1;

        // set the pointer to the string start
    #if STR_DEF_WITH_SPACE
        str_start   = str_start + 3;
    #else
        str_start   = str_start + 2;
    #endif

        if(*end != '\"')
        {
            printf("ERROR: the string doesn't terminate with a quote.\n");
            compiler_error();
        }
        else if((g_xvm_strndx + cnt + 1) >= XVM_I8_MAX_STR) // include room for a null character
        {
            printf("ERROR: not enough string storage available.\n");
            compiler_error();
        }
        else
        {
            // a null character to terminate the string
            *end++ = '\0';

            cnt     = str_end - str_start;
            status  = (int32_t) g_xvm_strndx;

            strncpy((char*) &g_xvm_str[g_xvm_strndx], (char const*) str_start, cnt);

            g_xvm_strndx = g_xvm_strndx + cnt;
        }

        return status;
    }
#endif

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return void
 */
bool d_macro_value(int8_t*  str_start,
                   int8_t*  str_end,
                   bool     val_type)
{
    bool        status  = false;
    int32_t     cnt     = str_end - str_start;

    memset(g_macro_str, '\0', MAX_CHARS_MACRO_STR + 1);

    strncpy((char*) g_macro_str, (char const*) str_start, cnt);

// XXX: replace is_number with the following
//    errno = ~(ERANGE);
//    val = strtol((char*)  str, (char**) &end_ptr, 0);
//    fval = strtof((char*)  str, (char**) &end_ptr);
//    if(errno != ERANGE && val <= INT_MAX && val >= INT_MIN && end_ptr != str)

    if(is_number((int8_t*) g_macro_str))
    {
        if(add_macro_def(val_type))
            status = true;
    }
    else
    {
        printf("ERROR: a number must be used for a macro assignment.\n");
        compiler_error();
    }

    return status;
}

/**
 *
 *
 * @param str
 * @return void
 */
void d_help(int8_t* str)
{
    int8_t      word[MAX_TIB_LEN + 1];
    int8_t*     str_start   = str;
    int8_t*     str_end     = 0;
    int8_t*     p_voc       = 0;
    int8_t*     p_name      = 0;
    int8_t*     p_comment   = 0;
    int32_t     n           = 0;
    int32_t     i           = 0;
    int32_t     cnt         = 0;

    // look for the space character at the end of the TIB string
    str_end = (int8_t*) strchr((char const*) str, 0x20);

    if(!str_end)
    {
        printf("\nSorry, there is no help for the word %s\n", (char*) word);
        printf("\nTo view all available words type: HELPALL\n");

        return;
    }

    cnt = str_end - str_start;

    for(i = 0; i < cnt; i++)
        word[i] = *str_start++;

    word[cnt] = '\0';

    cnt++;

    // Check the user's word definitions first
    n = find_def_word((int8_t*) word);

    if(n >= 0)
    {
        p_voc       = get_def_voc(n);
        p_name      = get_def_name(n);
        p_comment   = get_def_comment(n);

        printf("\nVocabulary:  %s, word:  %s   %s\n",
                (char*) p_voc,
                (char*) p_name,
                (char*) p_comment);

        return;
    }

    // Directives and tokens are all upper-case
    cnt--;
    str_start = str;

    for(i = 0; i < cnt; i++)
        word[i] = (int8_t) toupper((int32_t) (*str_start++));

    word[cnt] = '\0';
    cnt++;

    // directive words
    n = find_std_word((int8_t const*) word);

    if(n >= 0)
    {
        printf("\nCompiler directive:  %s   %s\n",
                (char*) word,
                (char*) (directives[n].info));

        return;
    }

    n = find_token(word, cnt);

    // Primitive tokens
    if(n >= 0)
    {
        p_name      = token_name(n);
        p_comment   = token_comment(n);

        printf("\nToken:  %s   %s\n",
                (char*) p_name,
                (char*) p_comment);

        return;
    }

    printf("\nSorry, there is no help for the word %s\n", (char*) word);
    printf("\nTo view all available words type: HELPALL\n");
}

/**
 * Prints information the directives as well as primitives.
 *
 * @param void
 * @return void
 */
void d_helpall(void)
{
    uint32_t    i = 0;

    printf("\n------------------\n");
    printf("--- Directives ---\n");
    printf("------------------\n");

    for(i = 0; i < DIRECTIVES_CNT; i++)
    {
        // check it's the end of the list
        if(directives[i].name == 0)
            break;

        printf("  %s   %s\n",
               (char*) (directives[i].name),
               (char*) (directives[i].info));
    }

    printf("\n-------------\n");
    printf("--- Words ---\n");
    printf("-------------\n");

    for(i = 0; i < MAX_TOKEN_NO; i++)
    {
        // check it's the end of the list
        if(g_xvm_token_tab[i].name == 0)
            break;

        // exclude internal opcodes
        if(g_xvm_token_tab[i].name[0] != '_')
            printf("  %s   %s\n",
                    (char*) (g_xvm_token_tab[i].name),
                    (char*) (g_xvm_token_tab[i].info));
    }
}
/**
 * Turns byte-code debugging on.
 *
 * @param void
 * @return void
 */
void d_debug(void)
{
    g_debug = true;
}

/**
 * Turns byte-code debugging off.
 *
 * @param void
 * @return void
 */
void d_nodebug(void)
{
    g_debug = false;
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_step(void)
{
    g_step = true;
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_nostep(void)
{
    g_step = false;
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_showstackon(void)
{
    g_show_stack = true;
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_showstackoff(void)
{
    g_show_stack = false;
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_run(void)
{
    g_step = false;
}

/**
 * Prints out all the user defined words and the context they're defined in.
 *
 * @param void
 * @return void
 */
void d_words(void)
{
    show_voc();
}

/**
 * Prints out all the user defined words and the context they're defined in.
 *
 * @param void
 * @return void
 */
void d_variables(void)
{
    show_vars();
}

/**
 * Prints out all the currently defined vocabulary contexts.
 *
 * @param void
 * @return void
 */
void d_vocs(void)
{
    list_vocs();
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_order(void)
{
    list_order();
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return
 */
bool d_newvoc(int8_t* str_start,
              int8_t* str_end)
{
    int8_t name[VOC_NAME_MAX_CHARS + 1];
    int32_t i       = 0;
    int32_t cnt     = str_end - str_start;
    bool    status  = false;

    if(cnt > VOC_NAME_MAX_CHARS)
    {
        printf("ERROR: Vocabulary name cannot be longer than 32 charaters.\n");
        compiler_error();
    }
    else
    {
        memset(name, 0, VOC_NAME_MAX_CHARS + 1);

        for(i = 0; i < cnt; i++)
            name[i] = (int8_t) toupper((int32_t) (*str_start++));

        name[cnt] = '\0';

        if(!add_voc((int8_t*) name))
        {
            printf("ERROR: too many vocabularies.\n");
            compiler_error();
        }
        else
            status = true;
    }

    return status;
}

/**
 *
 *
 * @param mode
 * @return void
 */
void d_also(void)
{
    int8_t* str = next_parsed_word();

    if(str)
    {
        clear_search_order();

        if(!strcmp((char*) str, "FORTH"))
            return;

        add_search_order(str);
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_only(void)
{
    int8_t* str = next_parsed_word();

    if(str)
    {
        clear_search_order();

        if(!strcmp((char*) str, "FORTH"))
            return;

        add_search_order((int8_t*) str);
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_vocdef(void)
{
    context_as_current();
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_if(void)
{
    select_comp_stack(IF_STACK);

    name_push("IF");
    add_byte(TOKEN_SJZ);            // _SJZ
    add_byte((uint8_t) 0);          // 1-byte addr
    addr_push(g_comp_wradr - 1);
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_else(void)
{
    int8_t*     tmp     = 0;
    uint32_t    addr    = 0;
    int32_t     offs    = 0;

    select_comp_stack(IF_STACK);

    tmp = name_pop();

    if(tmp)
    {
        if(!strcmp((char const*) tmp, "IF"))
        {
            name_push("ELSE");
            add_byte(TOKEN_SJMP);               // _SJMP
            add_byte((uint8_t) 0);              // 1-byte addr

            addr = addr_pop();

            if(addr == I32_STACK_EMPTY)
            {
                printf("ERROR: address stack underflow.\n");
                compiler_error();
            }
            else
            {
                offs                = g_comp_wradr - addr - 1;
                *(g_comp_cp + addr) = (uint8_t) offs;

                addr_push(g_comp_wradr - 1);
            }
        }
        else
        {
            printf("ERROR: found an ELSE directive without an IF directive.\n");
            compiler_error();
        }

        free(tmp);
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_then(void)
{
    uint32_t    addr    = 0;
    int32_t     offs    = 0;
    int8_t*     tmp     = 0;
    int32_t     mode    = -1;

    select_comp_stack(IF_STACK);

    tmp = name_pop();

    if(tmp)
    {
        if(!strcmp((char const*) tmp, "IF") || !strcmp((char const*) tmp, "ELSE"))
        {
            addr = addr_pop();

            if(addr == I32_STACK_EMPTY)
            {
                printf("ERROR d_then-1: address stack underflow.\n");
                compiler_error();
            }
            else
            {
                offs                = g_comp_wradr - addr - 1;
                *(g_comp_cp + addr) = (uint8_t) offs;

                mode = mode_pop();

                if(mode == I32_STACK_EMPTY)
                {
                    printf("ERROR d_then-2: mode stack underflow.\n");
                    compiler_error();
                }
                else
                {
                    select_comp_stack(mode);
                }
            }
        }
        else
        {
            printf("ERROR: found a THEN directive without an IF directive.\n");
            compiler_error();
        }

        free(tmp);
    }
}

/**
 *
 *
 * @param start
 * @param end
 * @return void
 */
void d_remove_word(int8_t* start,
                   int8_t* end)
{
    int32_t index   = 0;
    size_t  cnt     = end - start;
    int8_t* word    = (int8_t*) calloc(cnt, sizeof(int8_t));

    strncpy((char*) word, (char const*) start, cnt);
    word[cnt] = '\0';

    index = find_def_word(word);

    if(index != -1)
    {
        remove_def_word(index);
    }
    else
    {
        index = find_var_def(word);

        if(index != -1)
        {
            remove_def_var(index);
        }
        else
        {
            printf("ERROR: \"%s\" not found in the vocabulary or variable list.\n", (char*) word);
            compiler_error();
        }
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_begin(void)
{
    select_comp_stack(LOOP_STACK);
    name_push("BEGIN");
    addr_push(g_comp_wradr - 1);
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_until(void)
{
    int8_t* tmp     = 0;
    int32_t offset  = 0;
    int32_t mode    = -1;

    select_comp_stack(LOOP_STACK);

    tmp = name_pop();

    if(tmp)
    {
        if(!strcmp((char const*) tmp, "BEGIN"))
        {
            add_byte(TOKEN_SJNZ);       // _SJNZ

            offset = addr_pop();

            if(offset == I32_STACK_EMPTY)
            {
                printf("ERROR d_until-1: address stack underflow.\n");
                compiler_error();
            }
            else
            {
                offset = offset - g_comp_wradr;

                add_byte((uint8_t) offset);

                mode = mode_pop();

                if(mode == I32_STACK_EMPTY)
                {
                    printf("ERROR d_until-2: mode stack underflow.\n");
                    compiler_error();
                }
                else
                {
                    select_comp_stack(mode);
                }
            }
        }
        else
        {
            printf("ERROR: found an UNTIL directive without a BEGIN directive.\n");
            compiler_error();
        }

        free(tmp);
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_do(void) // ( Ne Ns -- )
{
    select_comp_stack(LOOP_STACK);

    name_push("DO");
    add_byte(TOKEN_TWO_TO_RSTACK);      // 2>R
    addr_push(g_comp_wradr);
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_leave(void)
{
    int32_t mode = -1;

    select_comp_stack(LOOP_STACK);

    name_push("LEAVE");
    add_byte(TOKEN_SJMP);       // _SJMP
    add_byte((uint8_t) 0);
    addr_push(g_comp_wradr);

    mode = mode_pop();

    if(mode == I32_STACK_EMPTY)
    {
        printf("ERROR d_leave: mode stack underflow.\n");
        compiler_error();
    }
    else
    {
        select_comp_stack(mode);
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_loop(void)
{
    int8_t* tmp     = 0;
    int32_t offs    = 0;
    int32_t addr    = 0;
    int32_t mode    = -1;

    select_comp_stack(LOOP_STACK);

    tmp = name_pop();

    if(tmp)
    {
        while(!strcmp((char const*) tmp, "LEAVE"))
        {
            if(tmp)
                free(tmp);

            addr = addr_pop();

            if(addr == I32_STACK_EMPTY)
            {
                printf("ERROR d_loop-1: address stack underflow.\n");
                compiler_error();
            }
            else
            {
// XXX: WTF - a magic number!!!
                offs                    = g_comp_wradr - addr + 9;
                *(g_comp_cp + addr - 1) = (uint8_t) offs;

                tmp                     = name_pop();
            }
        }
//printf("--- %s\n", tmp);
        if(!strcmp((char const*) tmp, "DO"))
        {
            add_byte(TOKEN_TWO_FROM_RSTACK);    // 2R>
            add_byte(TOKEN_INC);                // 1+
            add_byte(TOKEN_TWO_DUP);            // 2DUP
            add_byte(TOKEN_TWO_TO_RSTACK);      // 2>R
            add_byte(TOKEN_EQ);                 // =
            add_byte(TOKEN_SJZ);                // _SJZ

            offs = addr_pop();

            if(offs == I32_STACK_EMPTY)
            {
                printf("ERROR d_loop-2: address stack underflow.\n");
                compiler_error();
            }
            else
            {
                offs = offs - g_comp_wradr - 1;

                add_byte((uint8_t) offs);
                add_byte(TOKEN_TWO_FROM_RSTACK);    // 2R>
                add_byte(TOKEN_TWO_DROP);           // 2DROP

                mode = mode_pop();

                if(mode == I32_STACK_EMPTY)
                {
                    printf("ERROR d_loop-3: mode stack underflow.\n");
                    compiler_error();
                }
                else
                {
                    select_comp_stack(mode);
                }
            }
        }
        else
        {
            printf("ERROR: found a LOOP directive without a DO directive.\n");
            compiler_error();
        }

        if(tmp)
            free(tmp);
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_i(void)
{
    add_byte(TOKEN_TWO_FROM_RSTACK);    // 2R>
    add_byte(TOKEN_DUP);                // DUP
    add_byte(TOKEN_FROM_RSTACK);        // TOKEN_FROM_RSTACK
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_j(void)
{
    add_byte(TOKEN_TWO_FROM_RSTACK);    // 2R>

    d_i();

    add_byte(TOKEN_ROT);                // ROT
    add_byte(TOKEN_ROT);                // ROT
    add_byte(TOKEN_TWO_TO_RSTACK);      // 2>R
}

void add_worddef_cmt(int8_t* cmt_start,
                     int8_t* cmt_end)
{
    int32_t cnt = cmt_end - cmt_start;

    if(cnt < (MAX_CHARS_TOKEN_INFO - 1))
    {
        strncpy((char*) m_def_comment, (char const*) cmt_start, cnt);
        m_def_comment[cnt] = '\0';
    }
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return void
 */
void d_word_def(int8_t* str_start,
                int8_t* str_end)
{
    int32_t cnt = str_end - str_start;
    int8_t* str = (int8_t*) alloca(cnt + 1);

    set_comp_mode(COMP_TARGET);
    addr_push(g_comp_wradr);

    // Used when the RECURSE directive is in the word
    g_current_word_def_addr = (int32_t) g_comp_wradr;

    strncpy((char*) str, (char const*) str_start, cnt);
    str[cnt] = '\0';

    name_push(str);
}

/**
 *
 *
 * @param void
 * @return void
 */
void d_word_def_end(void)
{
    int8_t*     name        = 0;
    uint32_t    start_addr  = 0;
    int32_t     len         = 0;

    name = name_pop();

    if(name)
    {
        add_byte(TOKEN_RETURN);

        start_addr = addr_pop();

        if(start_addr == I32_STACK_EMPTY)
        {
            printf("ERROR: address stack underflow.\n");
            compiler_error();
        }
        else
        {
            len = g_comp_wradr - start_addr;

            add_def_word(name, (int8_t*) m_def_comment, start_addr, len);
            set_comp_mode(COMP_CONSOLE);

            g_current_word_def_addr = -1;
        }

        free(name);
    }
    else
    {
        printf("ERROR: name stack underflow at word definition end.\n");
        compiler_error();
    }
}

/**
 *
 *
 * @param w_name
 * @return
 */
int32_t find_std_word(int8_t const* w_name)
{
    int32_t i = 0;

    for(i = 0; i < DIRECTIVES_CNT; i++)
    {
        // check it's the end of the list
        if(directives[i].name == 0)
            break;

        if(!((int8_t*) (directives[i].name)))
            return -1;

        if(!strcmp((char const*) w_name, (char const*) (directives[i].name)))
            return i;
    }

    return -2;
}

// end of file
