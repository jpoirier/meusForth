/**@file parser.c
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
#include <ctype.h>
#include <assert.h>

#include "types.h"
#include "config.h"
#include "parser.h"
#include "compiler.h"
#include "code.h"
#include "files.h"
#include "macro.h"
#include "files.h"

//int8_t      g_tib[TIB_BUF_U8_SZ];               // Terminal Input Buffer
int8_t*     g_tib = 0;               // Terminal Input Buffer
int8_t      g_words[MAX_WORDS][MAX_STR_LEN];    // parsed words

int32_t     g_curr_word_no          = 0;
int32_t     g_num_of_words          = 0;
int32_t     g_pars_ctrl             = PRS_DEFAULT;
int32_t     g_num_base              = 10;

/**
 * Resets the TIB by setting the first element of the buffer to zero.
 *
 * @param void
 * @return void
 */
void clr_tib(void)
{
//    g_tib[0] = 0;
    if(g_tib)
    {
        free(g_tib);
        g_tib = 0;
    }
}

/**
 * Clears the TIB and words buffer by calling their respective
 * clearing functions.
 *
 * @param void
 * @return void
 */
void reset_parser(void)
{
    clr_tib();
    clr_words();
}

/**
 * Check the ASCII string to see if it's a number.
 *
 * @param str the string to be checked
 * @return true if the string is a number and flase otherwise
 */
bool is_number(int8_t* str)
{
    int32_t i       = 0;
    int32_t base    = g_num_base;
    int32_t start   = 0;
    int32_t val     = 0;

    if(*str <= 0x20)
        return false;

    if((*str == '-') || (*str == '+'))
    {
        if(!(*(str + 1)))
            return false; // single sign

        start   = 1;
    }
    else // unsigned can be a hex number
    {
        if((*(str) == '0') && ((*(str + 1) == 'x') || (*(str + 1) == 'X')))
        {
            start   = 2;
            base    = 16;
        }
    }

    for(i = start; i < 32; i++)
    {
        if(*(str + i) <= 0x20)
            break;
        else
        {
            if(*(str + i) < '0')
                return false;

            if(*(str + i) > 'F')
                return false;

            if((*(str + i) > '9') && (*(str+i) < 'A'))
                return false;

            if(base == 10)
            {
                if(*(str + i) > '9')
                    return false;

                val = val * base + *(str + i) - '0';
            }
            else
            {
                if(*(str + i) <= '9')
                    val = val * base + *(str + i) - '0';
                else
                    val = val * base + *(str + i) + 10 - 'A';
            }
        }
    }

    return true;
}

/**
 * Clears the words buffer by setting the first element of each row
 * to zero.
 *
 * @param void
 * @return void
 */
void clr_words(void)
{
    int32_t i       = 0;

    g_num_of_words  = 0;
    g_curr_word_no  = 0;

    for(i = 0; i < MAX_WORDS; i++)
        g_words[i][0] = 0;
}

/**
 * Fetches the next word to parse.
 *
 * @param void
 * @return a pointer to the word or null
 */
int8_t* next_parsed_word(void)
{
    if(g_curr_word_no < g_num_of_words)
        return (int8_t*) (g_words[g_curr_word_no++]);
    else
        return 0;
}

/**
 * Prints out all the words in the words buffer via stdio.
 *
 * @param void
 * @return void
 */
void see_words(void)
{
    int32_t i;

    for(i = 0; i < MAX_WORDS; i++)
    {
        if(!g_words[i][0])
        {
            printf("%d words\n", i);

            return;
        }
        else
            printf("%s \n", (char*) g_words[i]);
    }
}

// end of file
