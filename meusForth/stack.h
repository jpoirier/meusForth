/**@file stack.h
 *
 * @author Original author: Joseph D Poirier
 * @date   Creation date  : 16 August 2008
 *
 *
 * @version X.X.X
 *
 * VERSION    DATE/AUTHOR              COMMENT
 *            20Sept08/Jpoirier         Added initial version information.
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
 */
#ifndef STACK_H
#define STACK_H

#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "c_class.h"

#define I32_STACK_EMPTY             (-1)


// - push expects a null terminated string
// - the array of pointers is increased by m_stack_size when the stack is full
// - isEmpty and isFull are internal methods
// - the client is required to free the object returned from pop

C_CLASS(I32Stack)

        int32_t     m_front;
        int32_t     m_rear;
        size_t      m_stack_size;
        int32_t*    m_stack;

        I32Stack    METHOD_VA(I32Stack, construct, size_t size);
        void        METHOD(I32Stack, destruct);
        void        METHOD(I32Stack, reset);
        bool        METHOD(I32Stack, isEmpty);
        bool        METHOD(I32Stack, isFull);
        void        METHOD_VA(I32Stack, push, int32_t x);
        int32_t     METHOD(I32Stack, pop);
        void        METHOD(I32Stack, rwlock);
        void        METHOD(I32Stack, rwunlock);

END_C_CLASS


C_CLASS(PI8Stack)

        int32_t     m_front;
        int32_t     m_rear;
        size_t      m_stack_size;
        int8_t**    m_stack;

        PI8Stack    METHOD_VA(PI8Stack, construct, size_t size);
        void        METHOD(PI8Stack, destruct);
        void        METHOD(PI8Stack, reset);
        bool        METHOD(PI8Stack, isEmpty);
        bool        METHOD(PI8Stack, isFull);
        void        METHOD_VA(PI8Stack, push, int8_t* x);
        int8_t*     METHOD(PI8Stack, pop);
        void        METHOD(PI8Stack, rwlock);
        void        METHOD(PI8Stack, rwunlock);

END_C_CLASS


C_CLASS(PVOIDStack)

        int32_t     m_front;
        int32_t     m_rear;
        size_t      m_stack_size;
        void**      m_stack;

        PVOIDStack  METHOD_VA(PVOIDStack, construct, size_t size);
        void        METHOD(PVOIDStack, destruct);
        void        METHOD_VA(PVOIDStack, reset, bool do_dealloc);
        bool        METHOD(PVOIDStack, isEmpty);
        bool        METHOD(PVOIDStack, isFull);
        void        METHOD_VA(PVOIDStack, push, void* x);
        void*       METHOD(PVOIDStack, pop);
        void*       METHOD(PVOIDStack, peek);
        void        METHOD(PVOIDStack, rwlock);
        void        METHOD(PVOIDStack, rwunlock);

END_C_CLASS

#ifdef __cplusplus
extern "C"{
#endif

        extern I32Stack    I32Stack_construct(I32Stack self, size_t size);
        extern void        I32Stack_destruct(I32Stack self);
        extern void        I32Stack_reset(I32Stack self);
        extern void        I32Stack_push(I32Stack self, int32_t x);
        extern int32_t     I32Stack_pop(I32Stack self);

        extern PI8Stack    PI8Stack_construct(PI8Stack self, size_t size);
        extern void        PI8Stack_destruct(PI8Stack self);
        extern void        PI8Stack_reset(PI8Stack self);
        extern void        PI8Stack_push(PI8Stack self, int8_t* x);
        extern int8_t*     PI8Stack_pop(PI8Stack self);

        extern PVOIDStack   PVOIDStack_construct(PVOIDStack self, size_t size);
        extern void         PVOIDStack_destruct(PVOIDStack self);
        extern void         PVOIDStack_reset(PVOIDStack self, bool do_dealloc);
        extern void         PVOIDStack_push(PVOIDStack self, void* x);
        extern void*        PVOIDStack_pop(PVOIDStack self);
        extern void*        PVOIDStack_peek(PVOIDStack self);

#ifdef __cplusplus
}
#endif


#endif /* STACK_H */


