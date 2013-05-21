/**@file parser.h
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
 */
#ifndef PARSER_H
#define PARSER_H

#define MAX_TIB_LEN             256
#define TIB_BUF_U8_SZ           (64 * 1024)
#define MAX_STR_LEN             32
#define MAX_WORDS               64
#define PRS_DEFAULT             0
#define PRS_PRE_LINE_COMMENT    1
#define PRS_LINE_COMMENT        2
#define PRS_BLOCK_COMMENT       3
#define PRS_BLOCK_COMMENT_C     4
#define PRS_STRING              5

#ifdef __cplusplus
extern "C"{
#endif

    extern int32_t g_num_base;
    extern int32_t g_num_of_words;
    extern int32_t g_curr_word_no;
    extern int32_t g_pars_ctrl;

//    extern int8_t g_tib[TIB_BUF_U8_SZ];
    extern int8_t* g_tib;
    extern int8_t g_words[MAX_WORDS][MAX_STR_LEN];

    extern void clr_tib(void);
    extern void reset_parser(void);
    extern bool is_number(int8_t* str);
    extern void clr_words(void);
    extern int8_t* next_parsed_word(void);
    extern void see_words(void);

#ifdef __cplusplus
}
#endif


#endif // PARSER_H

// end of file

