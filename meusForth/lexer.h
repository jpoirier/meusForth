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
#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


enum {
// XXX: todo - sync these up with the Forth versions
// keep these negative
    BYE                 = -30,
    SAVE_TO_ARRAY,
    SAVE_TO_BIN_FILE,
    OPEN_TXT_FILE,
    OPEN_BIN_FILE,
    IMPORT_LIBRARY,
    IMPORT_FUNCTION,
    IMPORT_END,
    WORD_DEF,
    VOC_DEF,
    STRING_DEF,
    STRING_DEF_END,
    INCLUDE_FILE,
    MACRO_DEF_NAME,
    MACRO_DEF_VAL,
    MACRO_DEF_END,
    VAR_DEF_NAME,
    VAR_DEF_VAL,
    VAR_DEF_END,
    BLK_CMT,
    LINE_CMT,
    LINE_CMT_END,
    BLK_CMT_END,
    OK_TO_COMPILE,
    PRINTED_HELP,
    WORD_HELP,
    WORD_REMOVE,
    WORD_REMOVE_END,
    ERROR_OCCURED,
    OK_TO_IMPORT_FUNC,
    NOT_OK_TO_IMPORT_FUNC
};

#define INST_ADDR(name)             (Label)&&I_##name
#define LABEL(name)                 I_##name:

#define PARAM_T                     1
#define NONPARAM_T                  0

#ifdef __GNUC__
    typedef void* Label;
#else
    typedef int32_t Label;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    extern int32_t lexer(int8_t* str, int32_t cnt);

#ifdef __cplusplus
}
#endif

#endif  // LEXER_H

// end of file
