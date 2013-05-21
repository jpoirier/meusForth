/**@file voc.c
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
#include "config.h"

#include "compiler.h"
#include "parser.h"
#include "code.h"
#include "files.h"
#include "voc.h"


static TVocabulary  voc[MAX_VOC_LEN];

// a running count
static int32_t      m_voc_cnt = 0;

static int8_t       m_curr_voc_name[VOC_NAME_MAX_CHARS];

static int8_t       m_search_order[MAX_VOCS][VOC_NAME_MAX_CHARS];
static int32_t      m_search_order_cnt = 0;

static int8_t       m_vocs[MAX_VOCS][VOC_NAME_MAX_CHARS];
static int32_t      m_vocs_cnt = 0;


/**
 *
 *
 * @param name
 * @return
 */
int32_t add_search_order(int8_t* name)
{
    if(m_search_order_cnt >= MAX_VOCS)
        return 0;

    if(find_voc((int8_t*) name) < 0)
    {
        DPRINTF_VA("'%s' ", (char*) name);
        printf("ERROR: unable to find the vocabulary.\n");
        compiler_error();

        return 0;
    }

    strncpy((char*) (m_search_order[m_search_order_cnt++]), (char const*) name, VOC_NAME_MAX_CHARS);

    return 1;
}

/**
 *
 *
 * @param name
 * @return
 */
int32_t add_voc(int8_t* name)
{
    int32_t n = find_voc((int8_t*) name);

    // Check if this is a new vocabulary
    if(n < 0)
    {
        if(m_vocs_cnt >= MAX_VOCS)
        {
            printf("ERROR: Vocabulary count exceeded.\n");
            compiler_error();

            return 0;
        }

        strncpy((char*) (m_vocs[m_vocs_cnt++]), (char const*) name, VOC_NAME_MAX_CHARS);
    }

    add_search_order((int8_t*) name);

    return 1;
}

/**
 *
 *
 * @param void
 * @return void
 */
void list_vocs(void)
{
    int32_t i;

    for(i = 0; i < m_vocs_cnt; i++)
    {
        printf("%-32s", (char*) (m_vocs[i]));

        if(i & 1)
            printf("\n");
    }

    if(i & 1)
        printf("\n");

    printf("Number of vocabularies: %d\n", m_vocs_cnt);
}

/**
 *
 *
 * @param void
 * @return void
 */
void list_order(void)
{
    int32_t i;

    for(i = 0; i < m_search_order_cnt; i++)
    {
        printf("%-32s",(char*) (m_search_order[i]));

        if(i & 1)
            printf("\n");
    }

    if(i & 1)
        printf("\n");

    printf("Current definitions to vocabulary %s\n",(char*) m_curr_voc_name);
}

/**
 *
 *
 * @param void
 * @return void
 */
void context_as_current(void)
{
    strncpy((char*) m_curr_voc_name, (char const*) (m_search_order[m_search_order_cnt - 1]), 32);
}

/**
 *
 *
 * @param fn
 * @return
 */
int32_t find_voc(int8_t* name)
{
    int32_t i;

    for(i = 0; i < m_vocs_cnt; i++)
    {
        if(strcmp((char const*) (m_vocs[i]), (char const*) name) == 0)
            return i;
    }

    return -1;
}

/**
 *
 *
 * @param void
 * @return void
 */
void clear_search_order(void)
{
    m_search_order_cnt = 0;

    add_search_order((int8_t*) "FORTH");
}

/**
 *
 *
 * @param void
 * @return void
 */
void reset_vocs(void)
{
    m_voc_cnt     = 0;
    m_vocs_cnt    = 0;

    add_voc((int8_t*) "FORTH");
    clear_search_order();
    strcpy((char*) m_curr_voc_name, "FORTH");
}

/**
 *
 *
 * @param name
 * @return void
 */
void set_curr_voc_name(int8_t* name)
{
    if(strlen((char const*) name) > 0)
        strncpy((char*) m_curr_voc_name,(char const*) name,32);
    else
        strcpy((char*) m_curr_voc_name, "forth");
}

/**
 *
 *
 * @param name
 * @param info
 * @param addr
 * @param len
 * @return
 */
int32_t add_def_word(int8_t*    name,
                     int8_t*    info,
                     uint32_t   addr,
                     uint32_t   len)
{
    if(m_voc_cnt < MAX_VOC_LEN)
    {
        strcpy((char*) (voc[m_voc_cnt].name), (char const*) name);
        strcpy((char*) (voc[m_voc_cnt].voc_name), (char const*) m_curr_voc_name);
        strcpy((char*) (voc[m_voc_cnt].info), (char const*) info);

        voc[m_voc_cnt].addr = addr;
        voc[m_voc_cnt].len  = len;

        m_voc_cnt++;

        return 1;
    }
    else
    {
        printf("ERROR: target vocabulary is full.\n");
        compiler_error();

        return 0;
    }
}

/**
 *
 *
 * @param index
 * @return
 */
void remove_def_word(int32_t index)
{
    int32_t i;

    // shift everything over by one starting at the index of the word being removed
    for(i = index; i < m_voc_cnt; i++)
        voc[i] = voc[i + 1];

    // decrement the count
    m_voc_cnt -= 1;

    // this shouldn't happen...
    if(m_voc_cnt < 0)
        m_voc_cnt = 0;
}

/**
 *
 *
 * @param name
 * @return
 */
int32_t find_def_word(int8_t const* word)
{
    int32_t i;
//    int32_t j;

    if(m_voc_cnt > 0)
    {
//        for(j = m_vocs_cnt - 1; j >= 0; j--) // for all vocs in search order
//        {
            for(i = 0; i < m_voc_cnt; i++) // for all words
            {
//                if(!strcmp((char const*) (voc[i].voc_name), (char const*)(m_search_order[j]))) // if voc name matches
//                {
                    if(!strcmp((char const*) (voc[i].name), (char const*) word)) // if word name matches
                    {
                        return i;
                    }
 //               }
            }
//       }
    }

    return -1;
}

/**
 *
 *
 * @param void
 * @return
 */
int8_t *get_context(void)
{
    return (int8_t*) (m_search_order[m_search_order_cnt - 1]);
}

/**
 *
 *
 * @param n
 * @return
 */
int8_t* get_def_voc(int32_t n)
{
    if((n < 0) || (n >= m_voc_cnt))
        return 0;

    return (int8_t*) (voc[n].voc_name);
}

/**
 *
 *
 * @param n
 * @return
 */
int8_t* get_def_name(int32_t n)
{
    if((n < 0) || (n >= m_voc_cnt))
        return 0;

    if(strcmp((char const*) (voc[n].voc_name), (char const*) get_context()) == 0)
        return (int8_t*) (voc[n].name);
    else
        return (int8_t*) "";
}

/**
 *
 *
 * @param n
 * @return
 */
int8_t *get_def_comment(int32_t n)
{
    if((n < 0) || ( n >= m_voc_cnt))
        return 0;

    return (int8_t*) (voc[n].info);
}

/**
 *
 *
 * @param n
 * @return
 */
uint32_t get_def_addr(int32_t n)
{
    if((n < 0) || (n >= m_voc_cnt))
        return 0;

    return (uint32_t) (voc[n].addr);
}

/**
 *
 *
 * @param n
 * @return
 */
int32_t get_def_len(int32_t n)
{
    if((n < 0) || (n >= m_voc_cnt))
        return 0;

    return (int32_t) (voc[n].len);
}

/**
 *
 *
 * @param fn
 * @return
 */
void show_voc(void)
{

    int32_t i       = 0;
    int32_t n       = 0;
    int32_t cnt     = 0;
    int32_t slen    = 0;
    int32_t len     = 0;

    printf("\n---- Vocabulary context: %s ----\n", (char*) get_context());

    for(i = 0; i < m_voc_cnt; i++)
    {
        slen = (int32_t) strlen((char const*) get_def_name(i));

        if(slen > 0)
        {
            n++;

            if(slen < 16)
                slen = 16;
            else
            {
                if(slen < 32)
                    slen = 32;
                else if (slen < 48)
                     slen = 48;
            }

            cnt += slen;

            if(cnt > 65)
            {
                printf("\n");

                cnt = slen;
            }

            if(slen == 16)
                printf("%-16s ", (char const*) get_def_name(i));
            else if(slen == 32)
                printf("%-32s ", (char const*) get_def_name(i));
            else
                printf("%-48s ", (const char*) get_def_name(i));

            len += get_def_len(i);
        }
    }

    if(n == 0)
        printf("\nVocabulary is empty\n\n");
    else
        printf("\n\nTotal %d words, length %d bytes\n\n", n, len);
}

// end of file
