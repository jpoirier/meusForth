/**@file stack.cpp
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
 *
 * @cmd<<%PRJ% file=%PFE%>>
 */
#include <stdio.h>
#include <assert.h>

#include "stack.h"
#include "utilities.h"


static bool I32Stack_isEmpty(I32Stack self);
static bool I32Stack_isFull(I32Stack self);
static void I32Stack_rwlock(I32Stack self);
static void I32Stack_rwunlock(I32Stack self);

static bool PI8Stack_isEmpty(PI8Stack self);
static bool PI8Stack_isFull(PI8Stack self);
static void PI8Stack_rwlock(PI8Stack self);
static void PI8Stack_rwunlock(PI8Stack self);

static bool PVOIDStack_isEmpty(PVOIDStack self);
static bool PVOIDStack_isFull(PVOIDStack self);
static void PVOIDStack_rwlock(PVOIDStack self);
static void PVOIDStack_rwunlock(PVOIDStack self);

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
I32Stack I32Stack_construct(I32Stack  self,
                            size_t    size)
{
    assert(self);
    assert(size);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    self->m_stack       = (int32_t*) malloc(sizeof(int32_t) * size);

    assert(self->m_stack);

    self->m_stack_size  = size + 1;
    self->m_front       = 0;
    self->m_rear        = 0;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return self;
}

void I32Stack_destruct(I32Stack self)
{
    assert(self);

    // First, free the array that holds the pointers
    free(self->m_stack);

    // Second, free the c_class object
    free(self);
}

void I32Stack_push(I32Stack self,
                   int32_t  x)
{
    assert(self);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->isFull(self))
    {
        // increase by self->m_stack_size each time
        self->m_stack_size  = self->m_stack_size + self->m_stack_size;
        self->m_stack       = (int32_t*) realloc((void*) self->m_stack, sizeof(int32_t) * self->m_stack_size);

        assert(self->m_stack);
    }

    self->m_stack[self->m_rear] = x;
    self->m_rear                = self->m_rear + 1;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif
}

int32_t I32Stack_pop(I32Stack self)
{
    assert(self);

    int32_t item = I32_STACK_EMPTY;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(!(self->isEmpty(self)))
    {
       self->m_rear                 = self->m_rear - 1;
       item                         = self->m_stack[self->m_rear];
       self->m_stack[self->m_rear]  = 0;
    }

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return item;
}

bool I32Stack_isEmpty(I32Stack self)
{
    assert(self);

    bool status  = false;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->m_front > (self->m_rear - 1))
        status = true;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return status;
}

bool I32Stack_isFull(I32Stack self)
{
    assert(self);

    bool status = false;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->m_rear == self->m_stack_size)
        status = true;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return status;
}

void I32Stack_reset(I32Stack self)
{
    assert(self);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    self->m_front = 0;
    self->m_rear  = 0;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif
}

void I32Stack_rwlock(I32Stack self)
{
    assert(self);
}

void I32Stack_rwunlock(I32Stack self)
{
    assert(self);
}

VIRTUAL(I32Stack)
  VMETHOD(construct)    = I32Stack_construct;
  VMETHOD(destruct)     = I32Stack_destruct;
  VMETHOD(reset)        = I32Stack_reset;
  VMETHOD(isEmpty)      = I32Stack_isEmpty;
  VMETHOD(isFull)       = I32Stack_isFull;
  VMETHOD(push)         = I32Stack_push;
  VMETHOD(pop)          = I32Stack_pop;
  VMETHOD(rwlock)       = I32Stack_rwlock;
  VMETHOD(rwunlock)     = I32Stack_rwunlock;
END_VIRTUAL


/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
PI8Stack PI8Stack_construct(PI8Stack  self,
                            size_t    size)
{
    assert(self);
    assert(size);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    self->m_stack       = (int8_t**) malloc(sizeof(int8_t*) * size);

    assert(self->m_stack);

    self->m_stack_size  = size;
    self->m_front       = 0;
    self->m_rear        = 0;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return self;
}

void PI8Stack_destruct(PI8Stack self)
{
    assert(self);

    // First, free the data that's pointed to by the pointers contained in the array
    self->reset(self);

    // Second, free the array that holds the pointers
    free(self->m_stack);

    // Third, free the c_class object
    free(self);
}

void PI8Stack_push(PI8Stack self,
                   int8_t*  x)
{
    assert(self);
    assert(x);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->isFull(self))
    {
        // increase by self->m_stack_size each time
        self->m_stack_size  = self->m_stack_size + self->m_stack_size;
        self->m_stack       = (int8_t**) realloc((void*) self->m_stack, sizeof(int8_t*) * self->m_stack_size);

        assert(self->m_stack);
    }

    self->m_stack[self->m_rear] = (int8_t*) str_dup(x);
    self->m_rear                = self->m_rear + 1;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif
}

int8_t* PI8Stack_pop(PI8Stack self)
{
    assert(self);

    int8_t* item = 0;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(!(self->isEmpty(self)))
    {
       self->m_rear                 = self->m_rear - 1;
       item                         = self->m_stack[self->m_rear];
       self->m_stack[self->m_rear]  = 0;
    }

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return item;
}

bool PI8Stack_isEmpty(PI8Stack self)
{
    assert(self);

    bool status = false;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->m_front > (self->m_rear - 1))
        status = true;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return status;
}

bool PI8Stack_isFull(PI8Stack self)
{
    assert(self);

    bool status = false;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->m_rear == self->m_stack_size)
        status = true;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return status;
}

void PI8Stack_reset(PI8Stack self)
{
    int8_t* item = 0;

    assert(self);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    // iterate through the storage pointers as free each one
    while(true)
    {
        item = self->pop(self);

        if(item)
            free(item);
        else
            break;
    }

    self->m_front = 0;
    self->m_rear  = 0;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif
}

void PI8Stack_rwlock(PI8Stack self)
{
    assert(self);
}

void PI8Stack_rwunlock(PI8Stack self)
{
    assert(self);
}

VIRTUAL(PI8Stack)
  VMETHOD(construct)    = PI8Stack_construct;
  VMETHOD(destruct)     = PI8Stack_destruct;
  VMETHOD(reset)        = PI8Stack_reset;
  VMETHOD(isEmpty)      = PI8Stack_isEmpty;
  VMETHOD(isFull)       = PI8Stack_isFull;
  VMETHOD(push)         = PI8Stack_push;
  VMETHOD(pop)          = PI8Stack_pop;
  VMETHOD(rwlock)       = PI8Stack_rwlock;
  VMETHOD(rwunlock)     = PI8Stack_rwunlock;
END_VIRTUAL


/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
PVOIDStack PVOIDStack_construct(PVOIDStack  self,
                                size_t      size)
{
    assert(self);
    assert(size);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    self->m_stack       = (void**) malloc(sizeof(int8_t*) * size);

    assert(self->m_stack);

    self->m_stack_size  = size;
    self->m_front       = 0;
    self->m_rear        = 0;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return self;
}

void PVOIDStack_destruct(PVOIDStack self)
{
    assert(self);

    // First, free the array that holds the pointers
    free(self->m_stack);

    // Second, free the c_class object
    free(self);
}

void PVOIDStack_push(PVOIDStack self,
                     void*      x)
{
    assert(self);
    assert(x);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->isFull(self))
    {
        // increase by self->m_stack_size each time
        self->m_stack_size  = self->m_stack_size + self->m_stack_size;
        self->m_stack       = (void**) realloc((void*) self->m_stack, sizeof(void*) * self->m_stack_size);

        assert(self->m_stack);
    }

    self->m_stack[self->m_rear] = x;
    self->m_rear                = self->m_rear + 1;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif
}

void* PVOIDStack_pop(PVOIDStack self)
{
    assert(self);

    int8_t* item = 0;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(!(self->isEmpty(self)))
    {
       self->m_rear                 = self->m_rear - 1;
       item                         = self->m_stack[self->m_rear];
       self->m_stack[self->m_rear]  = 0;
    }

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return item;
}

void* PVOIDStack_peek(PVOIDStack self)
{
    assert(self);

    void* item = 0;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(!(self->isEmpty(self)))
        item = self->m_stack[self->m_rear - 1];

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return item;
}

bool PVOIDStack_isEmpty(PVOIDStack self)
{
    assert(self);

    bool status = false;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->m_front > (self->m_rear - 1))
        status = true;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return status;
}

bool PVOIDStack_isFull(PVOIDStack self)
{
    assert(self);

    bool status = false;

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(self->m_rear == self->m_stack_size)
        status = true;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif

    return status;
}

void PVOIDStack_reset(PVOIDStack    self,
                      bool          do_dealloc)
{
    void* item = 0;

    assert(self);

#ifdef RW_LOCK
    self->rwlock(self);
#endif

    if(do_dealloc)
    {
        // iterate through the storage pointers as free each one
        while(true)
        {
            item = self->pop(self);

            if(item)
                free(item);
            else
                break;
        }
    }

    self->m_front = 0;
    self->m_rear  = 0;

#ifdef RW_LOCK
    self->rwunlock(self);
#endif
}

void PVOIDStack_rwlock(PVOIDStack self)
{
    assert(self);
}

void PVOIDStack_rwunlock(PVOIDStack self)
{
    assert(self);
}

VIRTUAL(PVOIDStack)
  VMETHOD(construct)    = PVOIDStack_construct;
  VMETHOD(destruct)     = PVOIDStack_destruct;
  VMETHOD(reset)        = PVOIDStack_reset;
  VMETHOD(isEmpty)      = PVOIDStack_isEmpty;
  VMETHOD(isFull)       = PVOIDStack_isFull;
  VMETHOD(push)         = PVOIDStack_push;
  VMETHOD(pop)          = PVOIDStack_pop;
  VMETHOD(peek)         = PVOIDStack_peek;
  VMETHOD(rwlock)       = PVOIDStack_rwlock;
  VMETHOD(rwunlock)     = PVOIDStack_rwunlock;
END_VIRTUAL


/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
#ifdef PC_TEST_STACK
#include <stdio.h>

// to compile: gcc -o stack -std=c99 -DPC_TEST_STACK stack.c

#define ARRAY_CNT       20


int8_t*      i8_ary[ARRAY_CNT]  = {
                                    (int8_t*) "Item 1\0",
                                    (int8_t*) "Item 2\0",
                                    (int8_t*) "Item 3\0",
                                    (int8_t*) "Item 4\0",
                                    (int8_t*) "Item 5\0",
                                    (int8_t*) "Item 6\0",
                                    (int8_t*) "Item 7\0",
                                    (int8_t*) "Item 8\0",
                                    (int8_t*) "Item 9\0",
                                    (int8_t*) "Item 10 - a lot longer than the other items\0",
                                    (int8_t*) "Item 11\0",
                                    (int8_t*) "Item 12\0",
                                    (int8_t*) "Item 13\0",
                                    (int8_t*) "Item 14\0",
                                    (int8_t*) "Item 15\0",
                                    (int8_t*) "Item 16\0",
                                    (int8_t*) "Item 17\0",
                                    (int8_t*) "Item 18\0",
                                    (int8_t*) "Item 19\0",
                                    (int8_t*) "Item 20 - a lot longer than the other items\0"
                            };

int32_t     i32_ary[ARRAY_CNT] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};


int32_t main(int32_t argc, char* argv[])
{
    int8_t*     p_i8    = 0;
    int32_t     i       = 0;
    int32_t     x       = 0;

    I32Stack stk_i32  = CONSTRUCT_VA(I32Stack, construct, ARRAY_CNT);
    PI8Stack stk_pi8    = CONSTRUCT_VA(PI8Stack, construct, ARRAY_CNT);
    PVOIDStack stk_pv = CONSTRUCT_VA(PVOIDStack, construct, ARRAY_CNT);

    // I32Stack ----------------------------------------------------------------
    printf("\n\n");
    printf("------ Testing the I32Stack c_class -----\n");

    for(i = 0; i < ARRAY_CNT; i++)
    {
        stk_i32->push(stk_i32, i32_ary[i]);
    }

    i = ARRAY_CNT;

    while(i--)
    {
        x = stk_i32->pop(stk_i32);

        if(x != i32_ary[i])
            printf("Error comparing item at index %d\n", i);
        else
            printf("Item %d passed\n", i);
    }

    // PI8Stack ----------------------------------------------------------------
    printf("\n\n");
    printf("------ Testing the PI8Stack c_class -----\n");

    for(i = 0; i < ARRAY_CNT; i++)
    {
        stk_pi8->push(stk_pi8, (int8_t*) &i8_ary[i]);
    }

    i = ARRAY_CNT;

    while(i--)
    {
        p_i8 = stk_pi8->pop(stk_pi8);

        if(strcmp((char const*) p_i8, (char const*) &i8_ary[i]))
            printf("Error comparing item at index %d\n", i);
        else
            printf("Item %d passed\n", i);

        if(p_i8)
            free(p_i8);
    }

    // PVOIDStack --------------------------------------------------------------
    printf("\n\n");
    printf("------ Testing the PVOIDStack c_class -----\n");

    for(i = 0; i < ARRAY_CNT; i++)
    {
        stk_pv->push(stk_pv, (void*) &i8_ary[i]);
    }

    i = ARRAY_CNT;

    while(i--)
    {
        p_i8 = (int8_t*) stk_pv->pop(stk_pv);

        if((uint32_t) p_i8 != (uint32_t) &i8_ary[i])
            printf("Error comparing item at index %d\n", i);
        else
            printf("Item %d passed\n", i);
    }

    stk_i32->destruct(stk_i32);
    stk_pi8->destruct(stk_pi8);
    stk_pv->destruct(stk_pv);

    return 0;
}
#endif /* PC_TEST_JStack */

/* end of file */

