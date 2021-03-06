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



#ifdef WIN32
    #include <process.h>
    #include <direct.h>

    #define getcwd          _getcwd
    #define chdir           _chdir
#endif

#if defined(OSX) || defined(NIX)
    #include <unistd.h>
#endif

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

static int8_t* strip_white(int8_t* string);
static void outer_interpreter(void);
//static int32_t get_line(int8_t* dst, size_t limit);
static int32_t get_line(bool do_prompt);
static void do_cleanup(void);
static void signal_handler(int32_t signal);


// meus : my   (m-eh-oo-s-Forth)
float meusForth_version = 1.17;
float xvm_version       = 1.17;
FILE* error_stream      = 0;        // Error log file
bool g_eof_status       = false;


// Strip white space from the start and end of STRING.  Return a pointer into STRING.
int8_t* strip_white(int8_t* string)
{
    int8_t* s;
    int8_t* t;

    // strip white space at the start of the string
    for(s = string; whitespace(*s); s++)
        /* no code */ ;

    if(*s == 0)
        return s;

    // new size, excluding the null character at the end
    t = s + strlen((char*) s) - 1;

    // strip whitespace from the end of the string
    while(t > s && whitespace(*t))
        t--;

    // move the null character to the end of the string
    *++t = '\0';

    return s;
}

/**
 *
 *
 * @param void
 * @return void
 */
void outer_interpreter(void)
{
    TFileInfo*  file_info       = 0;
    int32_t     lex_status      = 0;
    int32_t     char_cnt        = 0;
    bool        do_prompt       = false;

//    printf("FORTH");

    while(1)
    {
        compiler_reset();

//        printf("> ");
        do_prompt = true;

get_input:

        // consume a single line of characters
        char_cnt = get_line(do_prompt);

        do_prompt = false;

//printf("char_cnt: %d\n", char_cnt);
//if(char_cnt)
//    printf("char: %s\n", (char*) g_tib);

        if(g_eof_status)
        {
            close_curr_file();
        }

        // If an empty line exists and:
        //  1. console mode then the user hit return and we stop processing
        //          or
        //  2. there's an open file so we continue processing.
//        if(g_tib[0] ==  ' ' && g_tib[1] ==  '\n')
        if(!char_cnt) // an empty line
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

        // Check the processing status post lexer. The scenarios:
        //  A)  We performed a non-compilation type of directive and there's
        //      nothing more to do, in which case we break from the
        //      outer-interpreter's loop and call get_line.
        // B)
        if(lex_status == BYE)
        {
            goto end_loop;
        }
        else if(lex_status == SAVE_TO_ARRAY)
        {
            g_can_execute   = false;
        }
        else if(lex_status == SAVE_TO_BIN_FILE)
        {
            g_can_execute   = false;
        }
        else if(lex_status == OPEN_BIN_FILE)
        {
            g_can_execute   = true;
        }
        else if(lex_status == IMPORT_LIBRARY || lex_status == IMPORT_FUNCTION || lex_status == IMPORT_END)
        {
            g_can_execute   = false;
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
        }
        else if(lex_status == VAR_DEF_END)
        {
            g_can_execute   = false;
        }
//        else if(lex_status == STRING_DEF_END)
//        {
//            g_can_execute   = false;
//        }
//        else if(lex_status == LINE_CMT)
//        {
//            g_can_execute   = false;
//        }
//        else if(lex_status == LINE_CMT_END || lex_status == BLK_CMT_END)
//        {
//            g_can_execute   = false;
//        }
        else if(lex_status == PRINTED_HELP)
        {
            g_can_execute   = false;
        }
        else if(lex_status == WORD_HELP)
        {
            g_can_execute   = false;
        }
        else if(lex_status == ERROR_OCCURED)
        {
            g_err_cnt++;
        }
        else if(lex_status == OK_TO_COMPILE)
        {
            g_can_execute   = true;
        }
        else if(lex_status == INCLUDE_FILE)
        {
            goto get_input;
        }

        // Continue processing any open files.
        file_info = (TFileInfo*) m_file_stack->peek(m_file_stack);

        if(file_info->fp)
        {
            goto get_input;
        }

        compilation_end();

        if(!g_err_cnt)
        {
            if(g_can_execute)
            {
                if(g_console_code_wradr > 1)
                {
                    set_exe_mode(EXE_TARGET);
                    xvm_run();
                }
                else
                {
// XXX: print an error message
                }
            }
            else
            {
                set_exe_mode(EXE_TARGET);
                copy_con_to_target();
            }
        }
        else
        {
//            show_error(g_xvm_run_stat);
        }

end_processing:

        if(g_show_stack)
            show_stack();

// XXX: not required anymore since readline prints out the prompt
//        show_error(g_xvm_run_stat);

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
 */
int32_t get_line(bool do_prompt)
{
    TFileInfo*      file_info           = 0;
    FILE*           fp                  = 0;
    int8_t*         the_chars           = 0;

    int32_t         chr                 = 0;
    bool            space_flag          = false;

    int32_t         in_char_cnt         = 0;
    int8_t*         temp                = 0;
    int8_t*         itemp               = 0; // intermediate temp buffer
    int8_t*         iitemp              = 0; // intermediate temp buffer
    int8_t*         dst                 = 0;

    int8_t*         tok1                = 0;
    int8_t*         tok2                = 0;

    g_eof_status    = false;
    file_info       = (TFileInfo*) m_file_stack->peek(m_file_stack);

    assert(file_info);

    fp = file_info->fp;

    // some garbage collection, free a previously allocated g_tib buffer
    if(g_tib)
        free(g_tib);

    if(fp)
    {
        // maximum length of a line in a file
        in_char_cnt = 1024 + 4;

        // g_tib gets the worst case count
        g_tib = (int8_t*) calloc(in_char_cnt + 4, sizeof(int8_t));
        temp  = (int8_t*) calloc(in_char_cnt + 4, sizeof(int8_t));

        if(!g_tib)
        {
            fprintf(stderr, "ERROR - unable to allocate fp's \"g_tib\" buffer! (%s, %s, %s, %s)\n", __FILE__, __func__, __TIME__, __DATE__);

            return 0;
        }
        else if(!temp)
        {
            fprintf(stderr, "ERROR - unable to allocate fp's \"temp\" buffer! (%s, %s, %s, %s)\n", __FILE__, __func__, __TIME__, __DATE__);

            if(g_tib)
                free(g_tib);

            return 0;
        }

        // fgets returns a null pointer for a read-error as well as an end-of-file
        if(!fgets((char*) temp, 1024, fp))
        {
            if(feof(fp))
                g_eof_status = true;

            free(g_tib);
            free(temp);

            return 0;
        }

        // the character count for this line
        in_char_cnt = strlen((char const*) temp);

        // at this point fgets terminates on a newline
        // strip the newline from the end of the string
        if(temp[in_char_cnt - 2] == '\r')
        {
            temp[in_char_cnt - 2] = '\0';
            in_char_cnt -= 2;
        }
        else if(temp[in_char_cnt - 1] == '\n')
        {
            temp[in_char_cnt - 1] = '\0';
            in_char_cnt -= 1;
        }
        else
        {
            printf("ERROR - unknown line termination in file!\n");

            free(g_tib);
            free(temp);

            return 0;
        }

        // strip whitespace from the start and end of the line
        itemp = strip_white(temp);

        // at this point all whitesapce at the end of the line has been
        //  removed but we want at least one whitesapce character
        in_char_cnt             = strlen((char const*) itemp);
//        itemp[in_char_cnt++]    = '\040';
//        itemp[in_char_cnt]      = '\0';
        the_chars               = itemp;
        dst                     = g_tib;
    }
    else
    {
        if(do_prompt)
            temp = (int8_t*) readline("FORTH> ");
        else
            temp = (int8_t*) readline("");

        in_char_cnt = rl_end;

        if(!in_char_cnt)
        {
            if(temp)
                free(temp);

            return 0;
        }
        else
        {
            // g_tib gets the worst case count
            g_tib = (int8_t*) calloc(in_char_cnt + 4, sizeof(int8_t));

            if(!g_tib)
            {
                fprintf(stderr, "ERROR - unable to allocate the \"g_tib\" buffer! (%s, %s, %s, %s)\n", __FILE__, __func__, __TIME__, __DATE__);

                if(temp)
                    free(temp);

                return 0;
            }

            dst = g_tib;
        }

        itemp = strip_white(temp);

        if(strcmp((char*) itemp, "--list") == 0)
        {
            HIST_ENTRY** list;
            int i;

            list = history_list();

            if(list)
            {
                for(i = 0; list[i]; i++)
                    printf ("%d: %s\r\n", i, list[i]->line);
            }

            add_history((char*) itemp);
            free(temp);

            return 0;
        }
        else if(strcmp((char*) itemp, "--clear") == 0)
        {
            clear_history();
            add_history((char*) itemp);
            free(temp);

            return 0;
        }
        else if(strcmp((char*) itemp, "--pwd") == 0)
        {
            // how deep?
            char dir[2048];
            char* s;

            s = getcwd(dir, sizeof(dir) - 1);

            if(s == 0)
                printf("Error getting pwd: %s\n", dir);
            else
                printf ("    %s\n", dir);

            add_history((char*) itemp);
            free(temp);

            return 0;
        }

        iitemp = (int8_t*) malloc(strlen((char*) itemp));
        memcpy(iitemp, itemp, strlen((char*) itemp));

        // split in to two strings separated by whitesapce/tab/newline
        tok1 = (int8_t*) strtok((char*) iitemp, "    \n");
        tok2 = (int8_t*) strtok((char*) 0, "    \n");

        if(tok1)
        {
            if(strcmp((char*) tok1, "--cd") == 0)
            {
                if(tok2)
                {
                    // we want everything after the --cd command
                    if((chdir((char*) (itemp + 5))) == -1)
                        printf("Unable to cd to %s\n", (char*) (itemp + 5));
                }
                else
                    printf("The --cd command requires a valid path argument...\n");

                add_history((char*) itemp);
                free(temp);

                return 0;
            }
            else if(strcmp((char*) tok1, "--edit") == 0)
            {
                if(tok2)
                {
                    // we want everything after the --edit command
                    system((char*) (itemp + 7));
                }
                else
                    printf("The --edit command requires a valid argument...\n");

                add_history((char*) itemp);
                free(temp);

                return 0;
            }
        }

        add_history((char*) itemp);

        // at this point all whitesapce at the end of the line has been
        // removed but we want at least one whitesapce character
        in_char_cnt             = strlen((char const*) itemp);
//        itemp[in_char_cnt++]    = '\040';
//        itemp[in_char_cnt]      = '\0';
        the_chars               = itemp;
    }

    while(in_char_cnt--)
    {
        chr = *the_chars++;

        // sanity check
        if(chr == EOF)
        {
            g_eof_status = true;

            break;
        }

        // skip the carriage return part of MSW EOLs
        // XXX: does readline ahndle MSW EOLs?
//        if(chr == '\r')
//            continue;

        if(isspace(chr))
        {
            // if the _previous_ character was _not_ a space add it to the buffer
            if(!space_flag)
            {
                space_flag  = true;
                *dst++      = (int8_t) chr;
            }
        }
        else
        {
            space_flag  = false;
            *dst++      = (int8_t) chr;
        }
    }

    free(temp);

    // characters to process
    if(dst == g_tib)
    {
        free(g_tib);

        return 0;
    }

    // the last character before the newline _must_ be a space
    if(*dst != '\040')
        *dst++  = '\040';   // add a space
//    if(!space_flag)
//        *dst++  = '\040';   // add a space

    // the last character _must_ be a newline
    *dst++  = '\n';         // add a newline
    *dst    = '\0';         // add a terminating character

    return ((int32_t) (dst - g_tib));
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

    if(g_tib)
    {
        free(g_tib);
    }
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
