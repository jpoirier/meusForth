/**@file files.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"
#include "files.h"
#include "xvm_inner.h"
#include "compiler.h"
#include "parser.h"
#include "code.h"
#include "voc.h"
#include "xvm.h"
#include "utilities.h"

static void file_error(int8_t* err);

PVOIDStack m_file_stack = 0;


/**
 * Closes the file on the top of the stack if.
 *
 * @param void
 * @return void
 */
void close_curr_file(void)
{
    TFileInfo*  file_info   = 0;
    FILE*       fp          = 0;

    file_info = (TFileInfo*) m_file_stack->pop(m_file_stack);

    if(file_info)
    {
        DPRINTF_VA("Attempting to close %s\n", file_info->pfe);

        if(file_info->pfe)
            free(file_info->pfe);

        fp = file_info->fp;

        free(file_info);

        fclose(fp);
    }
}

/**
 * Closes all open files and sets the file on the top of the stack to console.
 *
 * @param void
 * @return void
 */
void close_all_files(void)
{
    TFileInfo*  file_info   = 0;

    // reset and dealloc
    m_file_stack->reset(m_file_stack, true);

    file_info = (TFileInfo*) malloc(sizeof(TFileInfo));

    file_info->pfe  = str_dup("console");
    file_info->fp   = 0;
    file_info->line = 0;
    file_info->pos  = 0;

    m_file_stack->push(m_file_stack, file_info);
}

/**
 *
 *
 * @param err
 * @return void
 */
void file_error(int8_t* err)
{
    printf("ERROR: ");
    printf("%s.\n", (char*) err);
    compiler_error();
    close_all_files();
}

/**
 *
 *
 * @param pfe the path, file and extension (/path/file.ext)
 * @return whether the file opend or not
 */
bool open_file(int8_t* pfe)
{
    TFileInfo*  file_info   = 0;
    FILE*       fp          = 0;
    bool        status      = false;

    file_info = (TFileInfo*) m_file_stack->peek(m_file_stack);

    if(file_info)
    {
        if(!strcmp((char const*) pfe, (char const*) file_info->pfe))
        {
            file_error((int8_t*) "an include file cannot include itself");

            return status;
        }
    }

    DPRINTF_VA("Current open file   : %s\n", file_info->pfe);
    DPRINTF_VA("New file to open    : %s\n", pfe);

    fp = (FILE*) fopen((char const*) pfe, "r");

    if(!fp)
    {
        printf("ERROR: %s does not exist.\n", (char*) pfe);
        compiler_error();
    }
    else
    {

        file_info       = (TFileInfo*) malloc(sizeof(TFileInfo));

        file_info->pfe  = str_dup(pfe);
        file_info->fp   = fp;
        file_info->line = 0;
        file_info->pos  = 0;

        m_file_stack->push(m_file_stack, file_info);

        status          = true;
    }

    return status;
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return status of the compiled file
 */
bool save_compiled(int8_t* str_start,
                   int8_t* str_end)
{
    int8_t* byte_array[MAX_CHARS_TOKEN_NAME];

    FILE*   this_file       = 0;
    bool    status          = false;
    int32_t cnt             = str_end - str_start;
    int32_t last_addr       = 0;
    int32_t i               = 0;
    int32_t ndx_for_main    = 0;
    uint32_t addr           = 0;

    // we use 'main' to bootstrap the system
    ndx_for_main = find_def_word("main");

    if(ndx_for_main < 0)
    {
        status = false;

        printf("ERROR: no 'main' defined.\n");
        compiler_error();

        return status;
    }
//---

    int8_t* fname = (int8_t*) alloca(cnt + 1);

    memcpy(fname, str_start, cnt);

    // Add a null character
    fname[cnt] = '\0';

    this_file   = (FILE*) fopen((const char*) fname, "w");

    if(!this_file)
    {
        printf("ERROR: cannot open the file for writing.\n");
        compiler_error();

        return status;
    }

    status = true;
// XXX: 1. fix the EOLs so they're platform specific, 2.the compilation process
//      should use a file name specified by the use and there should be an option
//      to output a compiled file.

    fputs("\n/* Compiled bytecode */\n\n", this_file);

    // the array definition and opening brace
    fputs("uint8_t g_byte_code[] = {\n", this_file);

    // although this is the last address it's really just
    // an array index which is the byte code count
    last_addr = g_xvm_code_wradr;

    if(g_console_code_wradr > 2)
        last_addr += (g_console_code_wradr - 2);

    if((last_addr + 3) > XVM_U8_MAX_CODE)
    {
        status = false;

        printf("ERROR: XVM Code stack full, can't add 'main'.\n");
        compiler_error();

        return status;
    }

    // we use 'main' to bootstrap the application, so insert the call
    addr = get_def_addr(ndx_for_main);

    // Although we're adding the byte-code for calling 'main' to the g_xvm_code
    // array directly it should be innocuous because we don't actually increment
    // the system's code pointer, g_console_code_wradr.
    g_xvm_code[last_addr++] = TOKEN_LCALL;
    g_xvm_code[last_addr++] = (uint8_t) ((addr >> 8) & 0xFF);
    g_xvm_code[last_addr++] = (uint8_t) (addr & 0xFF);

    for(i = 0; i < last_addr; i++)
    {
        sprintf((char*) byte_array, "%d, ", g_xvm_code[i]);
        fputs((char*) byte_array, this_file);

        // 15 bytes per line
        if((i & 0xF) == 0xF)
            fputs("\n", this_file);
    }

    assert(0xFF >= BYTE_CODE_STRM_END);

    fputs("255\n};\n", this_file);

    fclose(this_file);

    return status;
}

/**
 *
 *
 * @param str_start
 * @param str_end
 * @return status of the compiled file
 */
bool save_bin_file(int8_t* str_start,
                   int8_t* str_end)
{
    FILE*       this_file       = 0;
    int8_t*     fname           = 0;
    bool        status          = false;
    int32_t     cnt             = str_end - str_start;
    int32_t     last_addr       = 0;
    int32_t     i               = 0;
    int32_t     ndx_for_main    = 0;
    uint32_t    addr            = 0;
    uint16_t    chk_sum         = 0;

    // we use 'main' to bootstrap the system
    ndx_for_main = find_def_word("main");

    if(ndx_for_main < 0)
    {
        status = false;

        printf("ERROR: no 'main' defined.\n");
        compiler_error();

        return status;
    }

    fname = (int8_t*) alloca(cnt + 1);

    memcpy(fname, str_start, cnt);

    // add a null character to terminate the string
    fname[cnt] = '\0';

    this_file = (FILE*) fopen((const char*) fname, "wb");

    if(!this_file)
    {
        status = false;

        printf("ERROR: cannot open the binary file for writing.\n");
        compiler_error();

        return status;
    }

    // although this is the last address it's really just
    // an array index which is the byte code count
    last_addr = g_xvm_code_wradr;

    if(g_console_code_wradr > 2)
        last_addr += (g_console_code_wradr - 2);

    if((last_addr + 3) > XVM_U8_MAX_CODE)
    {
        status = false;

        printf("ERROR: XVM Code stack full, can't add 'main'.\n");
        compiler_error();

        return status;
    }

    // we use 'main' to bootstrap the application, so insert the call
    addr = get_def_addr(ndx_for_main);

    // Although we're adding the byte-code for calling 'main' to the g_xvm_code
    // array directly it should be innocuous because we don't actually increment
    // the system's code pointer, g_console_code_wradr.
    g_xvm_code[last_addr++] = TOKEN_LCALL;
    g_xvm_code[last_addr++] = (uint8_t) ((addr >> 8) & 0xFF);
    g_xvm_code[last_addr++] = (uint8_t) (addr & 0xFF);

    // write the byte code to the file
    i = fwrite(g_xvm_code, sizeof(uint8_t), last_addr, this_file);

    // fwrite should have written all the byte code to the file
    if(i != last_addr)
    {
        status = false;

        printf("ERROR: error writing the compiled bytes to the binary file.\n");
        compiler_error();

        fclose(this_file);
        remove((const char*) fname);

        return status;
    }

    assert(0xFF >= BYTE_CODE_STRM_END);

    // insert our simulated end-of-byte-stream charcter, it's
    // just an illegal op value
    putc(0xFF, this_file);

    // compute the checksum
    chk_sum = checksum(g_xvm_code, last_addr);

    DPRINTF_VA("0x%X\n", chk_sum);

    // append the 2 checksum bytes
    putc((chk_sum >> 8), this_file);
    putc((chk_sum & 0xFF), this_file);

    fclose(this_file);

    status = true;

    return status;
}

/**
 *
 *
 * @param addr
 * @param count
 * @return
 */
uint16_t checksum(uint8_t*  addr,
                  int32_t   count)
{
    uint32_t sum = 0;

    // the main summing loop
    while(count > 1)
    {
        sum     = sum + *((uint16_t*) addr);
        count   = count - 2;
        addr    += 2;
    }

    // Add left-over byte, if any
    if(count > 0)
        sum = sum + *((uint8_t*) addr);

    // Fold 32-bit sum to 16 bits
    while(sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return (uint16_t) (~sum);
}


// end of file
