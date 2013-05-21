/**@file main.c
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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
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
#include "debug.h"
#include "lexer.h"
#include "ffi.h"
#include "variable.h"

#include "../readline/readline.h"
#include "../readline/history.h"

static void outer_interpreter(void);
static int32_t get_line(int8_t* dst, size_t limit);
static void do_cleanup(void);
static void signal_handler(int32_t signal);


// meus : my   (m-eh-oo-s-Forth)
float meusForth_version = 1.16;
float xvm_version       = 1.16;
FILE* error_stream      = 0;        // Error log file
bool g_eof_status       = false;

/**
 *
 *
 * @param void
 * @return void
 */
void outer_interpreter(void)
{
    TFileInfo*  file_info   = 0;
    int32_t     lex_status  = 0;
    int32_t     char_cnt    = 0;

    printf("FORTH");

    while(1)
    {
        compiler_reset();

        printf("> ");

get_input:

        // consume a single line of characters
        char_cnt = get_line(g_tib, TIB_BUF_U8_SZ);

//printf("char_cnt: %d\n", char_cnt);
//printf("char: %s\n", g_tib);

        if(g_eof_status)
        {
            close_curr_file();
        }

        // If an empty line exists and:
        //  A) console mode then the user hit return and we stop processing
        //  or
        //  B) there's an open file so we continue processing.
//        if(g_tib[0] ==  ' ' && g_tib[1] ==  '\n')
        if(!char_cnt) // defines an empty line
        {
            file_info = (TFileInfo*) m_file_stack->peek(m_file_stack);

            if(file_info->fp)
                goto get_input;
            else
                goto end_processing; // console
        }

        set_exe_mode(EXE_TARGET);   // EXE_TARGET or EXE_CONSOLE

        g_can_execute = true;       // might be cleared by the SAVE directive

        // lex the line for tokens then process them
        lex_status = lexer(g_tib, char_cnt);

        // Check the lexer's processing status. Some scenarios
        // - We performed a non-compilation type of directive and there's
        //   nothing more to do for that line, in which case we break from
        //   the outer-interpreter's loop and consume another line.
        if(lex_status == BYE)
        {
            goto end_loop;
        }
        else if(lex_status == SAVE_TO_ARRAY)
        {
            g_can_execute   = false;
            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
        else if(lex_status == SAVE_TO_BIN_FILE)
        {
            g_can_execute   = false;
            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
        else if(lex_status == OPEN_BIN_FILE)
        {
            g_can_execute   = true;
            g_xvm_run_stat  = XVM_OK;

//            g_can_execute   = false;
//            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
        else if(lex_status == IMPORT_LIBRARY || lex_status == IMPORT_FUNCTION || lex_status == IMPORT_END)
        {
            g_can_execute   = false;
            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
        else if(lex_status == OPEN_TXT_FILE)
        {
            goto get_input;
        }
        else if(lex_status == WORD_DEF)
        {
            // A word definition that hasn't terminated yet - termination
            // requires a semicolon - implies there're more lines to process
            goto get_input;
        }
        else if(lex_status == BLK_CMT)
        {
            // A block comment that hasn't terminated yet, and there're
            // more lines to consume
            goto get_input;
        }
        else if(lex_status == MACRO_DEF_END)
        {
            g_can_execute   = false;
            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
        else if(lex_status == VAR_DEF_END)
        {
            g_can_execute   = false;
            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
//        else if(lex_status == STRING_DEF_END)
//        {
//            g_can_execute   = false;
//            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
//        }
//        else if(lex_status == LINE_CMT)
//        {
//            g_can_execute   = false;
//            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
//        }
//        else if(lex_status == LINE_CMT_END || lex_status == BLK_CMT_END)
//        {
//            g_can_execute   = false;
//            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
//        }
        else if(lex_status == PRINTED_HELP)
        {
            g_can_execute   = false;
            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
        else if(lex_status == WORD_HELP)
        {
            g_can_execute   = false;
            g_xvm_run_stat  = XVM_ILLEGAL_OPCODE;
        }
        else if(lex_status == ERROR_OCCURED)
        {
            g_xvm_run_stat = XVM_LEXICAL_ERROR;
        }
        else if(lex_status == OK_TO_COMPILE)
        {
            g_can_execute   = true;
            g_xvm_run_stat  = XVM_OK;
        }
        else if(lex_status == INCLUDE_FILE)
        {
            goto get_input;
        }

        file_info = (TFileInfo*) m_file_stack->peek(m_file_stack);

        if(file_info->fp)
        {
            goto get_input;
        }

        compilation_end();

        if(!g_err_cnt && g_can_execute)
        {
            if(g_console_code_wradr > 1)
            {
                set_exe_mode(EXE_TARGET);
                xvm_run();
            }
        }
        else if(!g_err_cnt)
        {
            set_exe_mode(EXE_TARGET);
            copy_con_to_target();
//            save_compiled();
        }

        if(((g_xvm_run_stat == XVM_OK) || (g_xvm_run_stat == XVM_ILLEGAL_OPCODE)) && !g_err_cnt)
        {

            g_xvm_run_stat = XVM_OK;
        }
        else
        {
            show_error(g_xvm_run_stat);

            g_xvm_run_stat = XVM_OK;
        }

end_processing:

        if(g_show_stack)
            show_stack();

        show_error(g_xvm_run_stat);

        xvm_restart();
    } // while

end_loop:

    return; // stops the compiler from complaining about the end_loop label
}

/**
 * Buffer the incoming characters until: the buffer is full or there's a
 * EOL or EOF character. When an EOL or EOF character is found it's replaced
 * with a combination space and EOF characters. Note that the Lexer loop
 * terminates on an EOL character and that tokens are bounded by spaces.
 *
 * @param dst destination buffer
 * @param limit destination buffer size
 * @return the number of characters processed
            else if(chr == '[')
            {
                putchar(8); // backspace
                arrow_key = true;
                limit++;

                continue;
            }

            if(arrow_key)
            {
                if(chr == 'A')      // KEY_UP
                {

                }
                else if(chr == 'B') // KEY_DOWN
                {

                }
                else if(chr == 'C') // KEY_RIGHT
                {

                }
                else if(chr == 'D') // KEY_LEFT
                {

                }
                else                // ??????
                {

                }
                putchar(8); // backspace
                // we consumed the arrow key so reset the flag
                arrow_key = false;
                limit++;

                continue;
            }
 */
int32_t get_line(int8_t*   dst,     // the output buffer
                 size_t    limit)   // maximum size of the buffer
{
    TFileInfo*      file_info           = 0;
    FILE*           fp                  = 0;
    int8_t* const   start_addr          = dst; // save the original start address
    int32_t         chr                 = 0;
    bool            space_flag          = false;
    bool            strip_ldng_spaces   = true;
//    bool            arrow_key           = false;

    assert(dst);

    memset(dst, 0, limit);

    g_eof_status    = false;
    file_info       = (TFileInfo*) m_file_stack->peek(m_file_stack);

    assert(file_info);

    fp = file_info->fp;

    // Note, we strip away leading spaces, ending spaces, and any extra spaces
    while(--limit > 3)
    {
        if(fp)
        {
            chr = fgetc(fp);

            if(chr == EOF)
            {
                limit++;

                g_eof_status = true;

                break;
            }
            else if(chr == '\n')
            {
                limit++;

                break;
            }
        }
        else
        {
            chr = getchar();

            if(chr == '\n')
            {
                limit++;

                break;
            }
#if 0
    // command key handling here...?
#endif
        }

        // skip the carriage return part of MSW EOLs
        if(chr == '\r')
        {
            limit++;
        }
        else if(isspace(chr))
        {
            // are we processing the beginning of the line
            if(strip_ldng_spaces)
            {
                limit++;
            }
            // if the _previous_ character was _not_ a space add it to the buffer
            else if(!space_flag)
            {
                space_flag  = true;
                *dst++      = (int8_t) chr;
            }
            else
            {
                limit++;
            }
        }
        else
        {
            strip_ldng_spaces   = false;
            space_flag          = false;
            *dst++              = (int8_t) chr;
        }
    } // while

    if(dst == start_addr)
        return 0;

    if(limit == 3)
        fprintf(stderr, "WARNING - input buffer overflow (buffer size: %d)\n", (int32_t) limit);

    // a line should terminate with a space _and_ a newline
    if(!space_flag)
        *dst++  = '\040';   // add a space

    *dst++  = '\n';         // add a newline
    *dst    = '\0';         // add a terminating character

    return (int32_t) (dst - start_addr);
}

/**
 * This is a system registered callback function. It performs any necessary
 * cleanup at application exit, provided it's a normal exit.
 *
 * @return void
 */
void do_cleanup(void)
{
    printf("    Exiting...\n" );

    m_file_stack->reset(m_file_stack, true);
    m_file_stack->destruct(m_file_stack);
    g_mode_stack->destruct(g_mode_stack);

    m_loop_astack->destruct(m_loop_astack);
    m_loop_nstack->destruct(m_loop_nstack);
    m_loop_fstack->destruct(m_loop_fstack);
    m_loop_lstack->destruct(m_loop_lstack);

    m_if_astack->destruct(m_if_astack);
    m_if_nstack->destruct(m_if_nstack);
    m_if_fstack->destruct(m_if_fstack);
    m_if_lstack->destruct(m_if_lstack);

#if HAVE_FFI
    ffi_close_libs();
#endif

    if(error_stream)
        fclose(error_stream);
}

void signal_handler(int32_t signal)
{
    struct tm*  ptr = 0;
    time_t      tm  = time(0);

    ptr             = localtime(&tm);

    switch(signal)
    {
        case SIGFPE:
            perror("---------------- \n");
            perror(asctime(ptr));
            perror("A floating point exception occured.\n");
            break;
        case SIGILL:
            perror("---------------- \n");
            perror(asctime(ptr));

            perror("An illegal instruction occured.\n");
            break;
        case SIGINT:
            // the user hit CTRL-C
            break;
        case SIGSEGV:
            perror("---------------- \n");
            perror(asctime(ptr));
            perror("A segmentation violation occured.\n");
            break;
        default:
            perror("---------------- \n");
            perror(asctime(ptr));
            perror("An unknown signal was caught.\n");
            break;
    }

    // pass a successful exit so our atexit handler is called
    exit(EXIT_SUCCESS);
}


/**
 * The entry point of meusForth.
 *
 * @param argc the number of command line arguments
 * @param argv the vectors for the arguments
 * @return return EXIT_SUCCESS
 */
int main(int argc, char* argv[])
{
    atexit(do_cleanup);

    // Setup a log file to catch errors piped to stderr.
    error_stream = freopen("./error_log.txt", "ab+", stderr);

    if(!error_stream)
        printf("Unable to create and/or open the error_log.txt file!\n" );

    // Our segway in to the interactive console
    printf("\n          meusForth (m-eh-oo-s-Forth) v%g\n", meusForth_version);
    printf("          xVM  v%g\n", xvm_version);
    printf("          \n");
    printf("  Built on      : %s, %s\n",  __TIME__, __DATE__);
    printf("\n");
    printf("  Variable count        : %d 32-bit words\n",   XVM_U32_MAX_VARS);
#if HAVE_PRINTF
    printf("  String memory         : %d KiB\n",            XVM_I8_MAX_STR / 1024);
#endif
    printf("  RAM memory            : %d KiB\n",            (int32_t) ((sizeof(uint32_t) * XVM_U32_MAX_RAM) / 1024));
    printf("  Code memory           : %d KiB\n",            XVM_U8_MAX_CODE / 1024);
    printf("  User Stack memory     : %d KiB\n\n",          (int32_t) ((sizeof(int32_t) * XVM_U32_MAX_STACK) / 1024));
    printf("  Return Stack memory   : %d KiB\n\n",          (int32_t) ((sizeof(int32_t) * XVM_U32_MAX_STACK) / 1024));
    printf("\n");
    printf("  Type 'helpall' to view the available directives and words.\n");
    printf("  Type 'bye' to exit.\n\n");

    // Note, xvm is reset from within the this function
    compiler_init();

#if HAVE_FFI
    ffi_init();
#endif

    // siganl handlers
    if(signal(SIGFPE, signal_handler) == SIG_ERR)
        perror("An error occured while setting the SIGFPE signal handler.\n");

    if(signal(SIGILL, signal_handler) == SIG_ERR)
        perror("An error occured while setting the SIGILL signal handler.\n");

    if(signal(SIGINT, signal_handler) == SIG_ERR)
        perror("An error occured while setting the SIGINT signal handler.\n");

    if(signal(SIGSEGV, signal_handler) == SIG_ERR)
        perror("An error occured while setting the SIGSEGV signal handler.\n");

//    raise(SIGFPE);
//    raise(SIGILL);
//    raise(SIGINT);
//    raise(SIGSEGV);

    // Jump in to the user interactive loop
    outer_interpreter();

    return EXIT_SUCCESS;
}

// end of file
