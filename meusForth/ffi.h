/**@file ffi.h
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
 */
#ifndef FFI_H
#define FFI_H

#include <stdint.h>
#include <stdbool.h>

enum {
    FFI_RET_VOID            = 0,    // return void
    FFI_RET_NONVOID         = 1,    // return non-void
    FFI_MAX_FUNC_PARAM_CNT  = 10,   // the max number of parameters per function
    FFI_MAX_FUNC_CNT        = 50,   // the max number of functions per library

    FFI_NO_RECORD           = 0,

    //
    FFI_TOTAL_REC_CNT       = 50,   // the total number of records
    FFI_KEY_SZ              = 256,  // the size of the key in chars
    // The dictionary takes a second parameter, a void pointer, which we'll
    // be using instead of the record but we put something here as a buffer
    FFI_REC_SZ              = 4,    // the record size in chars
    FFI_TOTAL_FUNC_CNT      = FFI_MAX_FUNC_CNT * FFI_TOTAL_REC_CNT
};

// This construct contains a function handle as well asinformation about the function.
typedef struct FUNC_INFO {
    void*       func_handle;
    int32_t     param_cnt;
    bool        ret_val;
    int32_t     index;
    uint32_t    params[FFI_MAX_FUNC_PARAM_CNT];
} FUNC_INFO;

// This construct contains the dynamic libraries handle as well
// as any functions that have been registered.
typedef struct FFI_LIB {
    void*       lib_handle;                     // the library's handle
    int32_t     func_cnt;                       // the number of functions registered
    FUNC_INFO*  functions[FFI_MAX_FUNC_CNT];    // pointers to the function handles
} FFI_LIB;


#ifdef __cplusplus
extern "C"{
#endif

    extern int32_t ffi_find_func(int8_t const* func_name);
    extern bool ffi_init(void);
    extern void ffi_import_end(void);
    extern bool ffi_import_lib(int8_t* str_start, int8_t* str_end);
    extern bool ffi_import_func(int8_t* str_start, int8_t* str_end);
    extern void ffi_close_lib(int8_t* lib_name);
    extern void ffi_call_func(int32_t func_index);
    extern void ffi_close_libs(void);

#ifdef __cplusplus
}
#endif

#endif // FFI_H

// end of file


