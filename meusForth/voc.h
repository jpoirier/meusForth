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
 */
#ifndef VOC_H
#define VOC_H

#define MAX_VOC_LEN         10000
#define MAX_VOCS            100

#define VOC_NAME_U8_CNT     32
#define VOC_INFO_U8_CNT     255
#define VOC_NAME_U8_CNT     32

typedef struct
{
  int8_t*   name[VOC_NAME_U8_CNT];
  int8_t*   info[VOC_INFO_U8_CNT];
  int8_t*   voc_name[VOC_NAME_U8_CNT];
  uint32_t  addr;
  uint32_t  len;
} TVocabulary;


#ifdef __cplusplus
extern "C"{
#endif

    extern int32_t add_search_order(int8_t* name);
    extern int32_t add_voc(int8_t* name);
    extern int32_t find_voc(int8_t* name);
    extern void clear_search_order(void);
    extern void list_vocs(void);
    extern void list_order(void);
    extern void show_voc(void);
    extern void context_as_current(void);
    extern int8_t* get_context(void);
    extern void set_curr_voc_name(int8_t *name);
    extern void reset_vocs(void);
    extern int32_t add_def_word(int8_t* name, int8_t* info, uint32_t addr, uint32_t len);
    extern void remove_def_word(int32_t index);
    extern int32_t find_def_word(int8_t const* word);
    extern uint32_t get_def_addr(int32_t n);
    extern int8_t* get_def_voc(int32_t n);
    extern int8_t* get_def_name(int32_t n);
    extern int8_t* get_def_comment(int32_t n);
    extern uint32_t get_def_addr(int32_t n);
    extern int32_t get_def_len(int32_t n);

#ifdef __cplusplus
}
#endif


#endif // VOC_H

// end of file

