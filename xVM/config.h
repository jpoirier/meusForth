/**@file config.h
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
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define HAVE_NXT                                false
#define HAVE_FP_NUMBERS                         false
#define HAVE_FFI                                true
#define HAVE_PRINTF                             true

#define DSTACK_CHECK                            true
#define RSTACK_CHECK                            true
#define PC_CHECK                                true
#define PC_REL_CHECK                            true
#define RAM_BOUNDARY_CHECK                      true
#define DIV_BY_ZERO_CHECK                       true

#define STR_DEF_WITH_SPACE                      true

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define MAX_FILE_STACK                          1000
#define PFE_MAX_CHARS                           1024

#define XVM_U32_MAX_VARS                        (256)
#if HAVE_PRINTF
    #define XVM_I8_MAX_STR                          (64 * 1024)
#endif
#define XVM_U32_MAX_RAM                         (64 * 1024)
#define XVM_U8_MAX_CODE                         (64 * 1024)
#define XVM_U32_MAX_STACK                       (256)

#define MAX_CONSOLE_ADDR                        XVM_U32_MAX_RAM
//#define MAX_VOC_ADDR                        0xFFF0

#define FORTH_COND_FALSE                        0x00000000
#define FORTH_COND_TRUE                         0xFFFFFFFF

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#undef          NPRINTF
//#define         NPRINTF

//#undef          NDPRINTF
#define         NDPRINTF

//#undef          NEPRINTF
#define         NEPRINTF


// __func__ (C99) : The function where the printf is called
// __LINE__ : The line where the printf is called
// __FILE__ : The file where the printf is called

// Standard printf
#ifdef NPRINTF
    #define PRINTF(fmt)
    #define PRINTF_VA(fmt, ...)
#else
    #define PRINTF(fmt)                printf(fmt)
    #define PRINTF_VA(fmt, ...)        printf(fmt, __VA_ARGS__)
#endif

// Debug printf
#ifdef NDPRINTF
    #define DPRINTF(fmt)
    #define DPRINTF_VA(fmt, ...)
#else
    #define DPRINTF(fmt)                printf("[%s:%s:%d] " fmt, __FILE__, __func__, __LINE__)
    #define DPRINTF_VA(fmt, ...)        printf("[%s:%s:%d] " fmt, __FILE__, __func__, __LINE__, __VA_ARGS__)
#endif

// Error printf
#ifdef NEPRINTF
    #define EPRINTF(fmt)
    #define EPRINTF_VA(fmt, ...)
#else
    #define EPRINTF(fmt)                printf("[%s:%s:%d] " fmt, __FILE__, __func__, __LINE__)
    #define EPRINTF_VA(fmt, ...)        printf("[%s:%s:%d] " fmt, __FILE__, __func__, __LINE__, __VA_ARGS__)
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifdef __INTEL_COMPILER
    // OS X
    #ifdef __APPLE__
        // The comment in the __SANDBLASTER__ definition applies.
    #endif

    // GNUC on *nix
    #if !defined(__APPLE__) && defined(__GNUC__)
        #define restrict    __restrict
        #define inline      __inline
    #endif

    // *MS VC++ with Intel compiler
    #if defined(_MSC_VER) && !defined(__APPLE__) && !defined(__GNUC__)
        #define alloca  _alloca
        #ifndef __cplusplus
            #define inline  __inline
        #endif
    #endif
#endif

// *MS VC++ only
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    #ifndef __cplusplus
        #define inline  __inline
    #endif

    #define restrict    __restrict
#endif

/*----------------------- Language Standards -----------------------------------
- C89           __STDC__                                    ANSI X3.159-1989
- C90           __STDC_VERSION__                            ISO/IEC 9899:1990
- C94           __STDC_VERSION__        = 199409L           ISO/IEC 9899-1:1994
- C99           __STDC_VERSION__        = 199901L           ISO/IEC 9899:1999
- C++98         __cplusplus             = 199707L           ISO/IEC 14882:1998
- C++/CLI       __cplusplus_cli         = 200406L           ECMA-372
- EC++          __embedded_cplusplus                        Embedded C++
------------------------------------------------------------------------------*/
// Note that not all compliant compilers provide the correct pre-defined macros.
#ifdef __STDC__
    #define PREDEF_STANDARD_C_1989

    #ifdef __STDC_VERSION__
        #define PREDEF_STANDARD_C_1990

        #if (__STDC_VERSION__ >= 199409L)
            #define PREDEF_STANDARD_C_1994
        #endif

        #if (__STDC_VERSION__ >= 199901L)
            #define PREDEF_STANDARD_C_1999
        #endif
    #endif
#endif

#ifdef CONSOLE_MODE
    #include "debug.h"
#endif

#ifndef __cplusplus
    #ifndef PREDEF_STANDARD_C_1999
        #error "ISO C99 compiler is required for this code to compile and work properly"
    #endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H

// end of file
