/**@file ffi.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

#ifdef WIN32
    #include <windows.h>
#endif

#if defined(OSX) || defined(NIX)
    #include <dlfcn.h>
#endif

#include "config.h"
#include "ffi.h"
#include "hash.h"
#include "xvm.h"
#include "xvm_inner.h"
#include "compiler.h"

static bool ffi_pop_params(uint32_t params[], int32_t const param_cnt);

#if DSTACK_CHECK
    #define FFI_CHK_DSTACK_UNDERFLOW(x) \
            if(((int32_t) g_xvm_sp - (int32_t) x) < 0) { show_error(XVM_DSTACK_UNDERFLOW); status = false; break; }

    #define FFI_CHK_DSTACK_OVERFLOW(x) \
            if((g_xvm_sp + x) > (XVM_U32_MAX_STACK - 1)) { show_error(XVM_DSTACK_OVERFLOW); status = false; return; }
#else
    #define FFI_CHK_DSTACK_UNDERFLOW(x)
    #define FFI_CHK_DSTACK_OVERFLOW(x)
#endif

// Functions with no return value
typedef void (*ffi_vfunc_0)(void);
typedef void (*ffi_vfunc_1)(uint32_t);
typedef void (*ffi_vfunc_2)(uint32_t, uint32_t);
typedef void (*ffi_vfunc_3)(uint32_t, uint32_t, uint32_t);
typedef void (*ffi_vfunc_4)(uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*ffi_vfunc_5)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*ffi_vfunc_6)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*ffi_vfunc_7)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*ffi_vfunc_8)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*ffi_vfunc_9)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*ffi_vfunc_10)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);


// Functions with a return value
typedef uint32_t (*ffi_func_0)(void);
typedef uint32_t (*ffi_func_1)(uint32_t);
typedef uint32_t (*ffi_func_2)(uint32_t, uint32_t);
typedef uint32_t (*ffi_func_3)(uint32_t, uint32_t, uint32_t);
typedef uint32_t (*ffi_func_4)(uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (*ffi_func_5)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (*ffi_func_6)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (*ffi_func_7)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (*ffi_func_8)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (*ffi_func_9)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (*ffi_func_10)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

static Htable       m_dict          = 0;
static int8_t*      m_current_lib   = 0;
static int32_t      m_function_cnt  = -1;

static FUNC_INFO    m_functions[FFI_TOTAL_FUNC_CNT];

/**
 * Searches the library dictionary for a function.
 *
 * @param func_name the name of the function
 * @return the index of the function or -1 if not found
 */
int32_t ffi_find_func(int8_t const* func_name)
{
    FUNC_INFO*  func_info   = 0;
    int32_t     index       = -1;

    func_info = (FUNC_INFO*) ht_pvoid_find(m_dict,
                                           (int8_t*) func_name);

    if(func_info)
        index = func_info->index;

    return index;
}

/**
 * Creates a dictionary to store library and function names texturally.
 *
 * @param void
 * @return true if the dictionary was created and false if not
 */
bool ffi_init(void)
{
    bool status = false;

    // we want to be able to get at a library and/or function
    // texturally so we make room for libraries and functions
    m_dict = ht_create(FFI_TOTAL_REC_CNT + FFI_TOTAL_FUNC_CNT,
                       FFI_KEY_SZ,
                       FFI_REC_SZ);
    assert(m_dict);

    if(m_dict)
        status = true;

    return status;
}

/**
 * Pops the necessary parameters off of the data stack and stores
 * them in an array.
 *
 * @param params storage array for the parameters
 * @param param_cnt parameter count
 * @return true if successful and false if not
 */
bool ffi_pop_params(uint32_t        params[],
                    int32_t const   param_cnt)
{
    int32_t i       = 0;
    bool    status  = false;

    // sanity check the parameter count
    if(param_cnt > FFI_MAX_FUNC_PARAM_CNT)
        return status;

    status = true;

    // function parameters are read left-to-right where the left most
    // parameter is deepest down in the stack.
    for(i = (param_cnt - 1); i >= 0; i--)
    {
        FFI_CHK_DSTACK_UNDERFLOW(ONE_CELL);

        params[i] = xvm_pop();
    }

    return status;
}

/**
 * Calls a function.
 *
 * @param func_index the index to the function being called
 * @return true if successful and false if not
 */
void ffi_call_func(int32_t func_index)
{
    FUNC_INFO*  func_info   = 0;
    uint32_t    ret_val     = 0;
    bool        status      = false;

    // sanity check the function index
    if(func_index > -1 && func_index < FFI_TOTAL_FUNC_CNT)
        func_info = &m_functions[func_index];
    else
        return;

    // sanity check the function handle
    if(!func_info->func_handle)
        return;

    //--------------------------------------------------------------------------
    //------ Pop the parameters off of the data stack
    //------ and store them in the array
    status = ffi_pop_params(func_info->params,
                            func_info->param_cnt);

    if(!status)
        return;

    //
    if(func_info->ret_val == FFI_RET_VOID)
    {
        switch(func_info->param_cnt)
        {
            case 0:
                ((ffi_vfunc_0) func_info->func_handle)();
                break;
            case 1:
                ((ffi_vfunc_1) func_info->func_handle)(func_info->params[0]);
                break;
            case 2:
                ((ffi_vfunc_2) func_info->func_handle)(func_info->params[0], func_info->params[1]);
                break;
            case 3:
                ((ffi_vfunc_3) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2]);
                break;
            case 4:
                ((ffi_vfunc_4) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2], func_info->params[3]);
                break;
            case 5:
                ((ffi_vfunc_5) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2], func_info->params[3], func_info->params[4]);
                break;
            case 6:
                ((ffi_vfunc_6) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2], func_info->params[3], func_info->params[4],
                                    func_info->params[5]);
                break;
            case 7:
                ((ffi_vfunc_7) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2], func_info->params[3], func_info->params[4],
                                    func_info->params[5], func_info->params[6]);
                break;
            case 8:
                ((ffi_vfunc_8) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2], func_info->params[3], func_info->params[4],
                                    func_info->params[5], func_info->params[6], func_info->params[7]);
                break;
            case 9:
                ((ffi_vfunc_9) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2], func_info->params[3], func_info->params[4],
                                    func_info->params[5], func_info->params[6], func_info->params[7],
                                    func_info->params[8]);
                break;
            case 10:
                ((ffi_vfunc_10) func_info->func_handle)(func_info->params[0], func_info->params[1],
                                    func_info->params[2], func_info->params[3], func_info->params[4],
                                    func_info->params[5], func_info->params[6], func_info->params[7],
                                    func_info->params[8], func_info->params[9]);
                break;
            default:
                status = false;
                break;
        }
    }
    else
    {
        switch(func_info->param_cnt)
        {
            case 0:
                ret_val = (uint32_t) ((ffi_func_0) func_info->func_handle)();
                break;
            case 1:
                ret_val = (uint32_t) ((ffi_func_1) func_info->func_handle)(func_info->params[0]);
                break;
            case 2:
                ret_val = (uint32_t) ((ffi_func_2) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1]);
                break;
            case 3:
                ret_val = (uint32_t) ((ffi_func_3) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2]);
                break;
            case 4:
                ret_val = (uint32_t) ((ffi_func_4) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2], func_info->params[3]);
                break;
            case 5:
                ret_val = (uint32_t) ((ffi_func_5) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2], func_info->params[3],
                                        func_info->params[4]);
                break;
            case 6:
                ret_val = (uint32_t) ((ffi_func_6) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2], func_info->params[3],
                                        func_info->params[4], func_info->params[5]);
                break;
            case 7:
                ret_val = (uint32_t) ((ffi_func_7) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2], func_info->params[3],
                                        func_info->params[4], func_info->params[5], func_info->params[6]);
                break;
            case 8:
                ret_val = (uint32_t) ((ffi_func_8) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2], func_info->params[3],
                                        func_info->params[4], func_info->params[5], func_info->params[6],
                                        func_info->params[7]);
                break;
            case 9:
                ret_val = (uint32_t) ((ffi_func_9) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2], func_info->params[3],
                                        func_info->params[4], func_info->params[5], func_info->params[6],
                                        func_info->params[7], func_info->params[8]);
                break;
            case 10:
                ret_val = (uint32_t) ((ffi_func_10) func_info->func_handle)(func_info->params[0],
                                        func_info->params[1], func_info->params[2], func_info->params[3],
                                        func_info->params[4], func_info->params[5], func_info->params[6],
                                        func_info->params[7], func_info->params[8], func_info->params[9]);
                break;
            default:
                status = false;
                break;
        }
    }

    if(status && func_info->ret_val == FFI_RET_NONVOID)
    {
        FFI_CHK_DSTACK_OVERFLOW(ONE_CELL);
        xvm_push(ret_val);
    }
}

/**
 * Free the currently opened library.
 *
 * @param void
 * @return void
 */
void ffi_import_end(void)
{
    if(m_current_lib)
    {
        free(m_current_lib);

        m_current_lib = 0;
    }
}

/**
 * Opens and registers a library.
 *
 * @param str_start start of the library name
 * @param str_end end of the library name
 * @return true if the library was imported and false if not
 */
bool ffi_import_lib(int8_t* str_start,
                    int8_t* str_end)
{
    FFI_LIB*    ffi_lib     = 0;
    void*       handle      = 0;
    int8_t*     rec         = 0;
    bool        status      = false;
    int32_t     cnt         = str_end - str_start;
    int8_t*     lib_name    = (int8_t*) calloc(FFI_KEY_SZ, sizeof(int8_t));

    // a local copy of the library name
    strncpy((char*) lib_name, (char const*) str_start, cnt);

    lib_name[cnt] = '\0';

//printf("lib_name: %s\n", lib_name);

    // check if we already have the library open
    rec = ht_rec_find(m_dict, lib_name);

    if(!rec)
    {
#ifdef WIN32
        handle = (void*) LoadLibrary((char const*) lib_name);
#else /* OSX || NIX */
        handle = (void*) dlopen((char const*) lib_name, RTLD_LAZY);
#endif

        // if we get a valid handle to the library make an information entry
        // for it in the dictionary of libs
        if(handle)
        {
            ffi_lib = (FFI_LIB*) malloc(sizeof(FFI_LIB));

            assert(ffi_lib);

            ffi_lib->lib_handle = (void*) handle;

            // there're no functions associated with this library yet
            ffi_lib->func_cnt   = -1;

            status  = ht_insert(m_dict,
                                lib_name,
                                (void*) ffi_lib,
                                FFI_NO_RECORD);

            if(!status)
            {
                free(ffi_lib);

#ifdef WIN32
                FreeLibrary((HMODULE) handle);
#else /* OSX || NIX */
                dlclose(handle);
#endif
                printf("ERROR: unable to insert the library into the dictionary, library file not imported.\n");
                compiler_error();
            }
// XXX: replace the use of the global with a stack of library names!
            // save the name of the current library being processed
            m_current_lib = lib_name;
        }
        else
        {
            printf("ERROR: unable to open the library file.\n");
            compiler_error();
        }
    }
    else
    {
        printf("ERROR: the library file already registerd.\n");
        compiler_error();
    }

    return status;
}

/**
 * Attempts to import a function from the currently registered library. The
 * format for the function is: return count (0 or 1), function name, and
 * parameter count.
 *
 * @param str_start start of the function name
 * @param str_end points end of the function name
 * @return true if the fumction was imported and false if not
 */
bool ffi_import_func(int8_t*   str_start,
                     int8_t*   str_end)
{
    int8_t      delim[]         = " ";
    void*       lib_handle      = 0;
    void*       func_handle     = 0;
    FFI_LIB*    ffi_lib         = 0;
    FUNC_INFO*  func_info       = 0;
    int8_t*     rec             = 0;
    int8_t*     func_name       = 0;
    int8_t*     str_param_cnt   = 0;
    int32_t     str_cnt         = str_end - str_start;
    int8_t*     str_ret_val     = 0;
    int32_t     ret_val         = 0;
    int32_t     param_cnt       = 0;
    int32_t     status          = -1;
    int8_t*     end_ptr         = 0;
    int8_t*     lib_name        = (int8_t*) calloc(FFI_KEY_SZ, sizeof(int8_t));
    int8_t*     func_str        = (int8_t*) calloc(FFI_KEY_SZ, sizeof(int8_t));
    int8_t*     str             = (int8_t*) alloca(str_cnt + 1);

    strncpy((char*) str, (char const*) str_start, str_cnt);
    str[str_cnt] = '\0';

    // tokenize the strings, there should be 3 strings:
    //  1. the return count
    //  2. the function name
    //  3. the parameter count
    str_ret_val     = (int8_t*) strtok((char*) str, (char const*) delim);
    func_name       = (int8_t*) strtok((char*) 0, (char const*) delim);
    str_param_cnt   = (int8_t*) strtok((char*) 0, (char const*) delim);

    if(!str_ret_val || !func_name || !str_param_cnt)
    {
        printf("ERROR: function importing requires a return value, a function name, and a parameter count.\n");
        compiler_error();

        free(func_str);
        free(lib_name);

        return status;
    }

//printf("str_ret_val: %s\n", str_ret_val);
//printf("func_name: %s\n", func_name);
//printf("str_param_cnt: %s\n", str_param_cnt);

    //--------------------------------------------------------------------------
    //----- Return count processing
    end_ptr = str_ret_val;
    errno   = ~(ERANGE);
    ret_val = strtol((char*) str_ret_val, (char**) &end_ptr, 0);

//    if(errno != ERANGE && ret_val <= INT_MAX && ret_val >= INT_MIN && end_ptr != str_ret_val)
//    {
//    }
//    else
    if(errno == ERANGE || ret_val > INT_MAX || ret_val < INT_MIN || end_ptr == str_ret_val)
    {
        printf("ERROR: a function's return value must be a non-negative integer value.\n");
        compiler_error();

        free(func_str);
        free(lib_name);

        return status;
    }

    //--------------------------------------------------------------------------
    //----- Parameter count processing
    end_ptr = str_param_cnt;
    errno   = ~(ERANGE);
    param_cnt = strtol((char*)  str_param_cnt, (char**) &end_ptr, 0);

//    if(errno != ERANGE && param_cnt <= INT_MAX && param_cnt >= INT_MIN && end_ptr != str_param_cnt)
//    {
//    }
//    else
    if(errno == ERANGE || param_cnt > INT_MAX || param_cnt < INT_MIN || end_ptr == str_param_cnt)
    {
        printf("ERROR: a function's parameter count must be a non-negative integer value.\n");
        compiler_error();

        free(func_str);
        free(lib_name);

        return status;
    }

    //--------------------------------------------------------------------------
    //----- Function name processing
// XXX: layered error handling needs to be added
    // copy the function name
    memset(func_str, 0, FFI_KEY_SZ);
    strcpy((char*) func_str, (char const*) func_name);

    //----- check if the function name already exists
    rec = ht_rec_find(m_dict, func_str);

    if(rec)
    {
        printf("ERROR: the function has already been imported.\n");
        compiler_error();

        free(func_str);
        free(lib_name);

        return status;
    }

    //--------------------------------------------------------------------------
    //-----
    // get a copy of the current library we're using
    strcpy((char*) lib_name, (char const*) m_current_lib);

    if(lib_name)
    {
        // search for the library info structure
        ffi_lib = (FFI_LIB*) ht_pvoid_find(m_dict, lib_name);

        if(ffi_lib)
        {
            // get the library's handle
            lib_handle = ffi_lib->lib_handle;

            assert(lib_handle);

            // try to register our function
#ifdef WIN32
            func_handle = (void*) GetProcessAddress((HMODULE) lib_handle, (char const*) func_str);
#else /* OSX || NIX */
            func_handle = (void*) dlsym(lib_handle, (char const*) func_str);
#endif

            if(func_handle)
            {
                // the function was registered successfully, now we want to
                // have quick access to it from xVM so we append it to the
                // array of functions, we'll use its index as a way to
                // retrieve the function's info structure
                m_function_cnt          += 1;
                func_info               = &m_functions[m_function_cnt];
                func_info->index        = m_function_cnt;

                // set the rest of the function specific information
                func_info->func_handle  = func_handle;
                func_info->param_cnt    = param_cnt;
                func_info->ret_val      = ret_val;

//printf("func_str            : %s\n", func_str);
//printf("m_function_cnt      : %d\n", m_function_cnt);
//printf("param_cnt           : %d\n", param_cnt);
//printf("ret_val             : %d\n", ret_val);
//printf("func_info           : 0x%X\n", func_info);
//printf("func_handle         : 0x%X\n", func_handle);

                // increment the library's function count and copy
                // the funcion info structure pointer to it's element
                ffi_lib->func_cnt += 1;
                ffi_lib->functions[ffi_lib->func_cnt] = func_info;

// XXX: add a check to see if we're within the max function count for a library

                // we want to be able to get to a function's info via
                // its textural name as well so we store it in the dictionary
                status  = ht_insert(m_dict,
                                    func_str,
                                    (void*) func_info,
                                    FFI_NO_RECORD);
            } // if(func_handle)
        } // if(ffi_lib)
    } // if(lib_name)

    // check if everything went okay
    if(status == -1)
        status = false;
    else
        status = true;

    // free-up the dynamic memory
    free(func_str);
    free(lib_name);

    return status;
}

/**
 * Close a library.
 *
 * @param lib_name the library to close
 * @return void
 */
void ffi_close_lib(int8_t* lib_name)
{
    FFI_LIB* ffi_lib = 0;

    ffi_lib = (FFI_LIB*) ht_pvoid_find(m_dict,
                                       lib_name);

    if(ffi_lib)
    {
#ifdef WIN32
        FreeLibrary((HMODULE) ffi_lib->lib_handle);
#else /* OSX || NIX */
        dlclose(ffi_lib->lib_handle);
#endif

        // delete the key
        ht_delete(m_dict,
                  lib_name);

        // free the lib structure
        free(ffi_lib);
    }
}

/**
 * Close all the existing library entries.
 *
 * @param void
 * @return void
 */
void ffi_close_libs(void)
{
    ht_obliterate(m_dict);
}

// end of file
