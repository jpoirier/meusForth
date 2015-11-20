/*******************************************************************************
# Copyright 2004: Commonwealth of Australia.
#
# Developed by the Computer Network Vulnerability Team,
# Information Security Group.
# Department of Defence.
#
# Michael Cohen <scudette@users.sourceforge.net>
#
# Joseph D Poirier <jdpoirier@gmail.com>
#   Major modifications:
#   - removed the dependency on talloc
#   - removed inheritance and polymorphism, creates basic classes only
#   - reduced to a single header file
#   - standardized data type usage via stdint.h and stdbool.h
#
#
# * This program is free software; you can redistribute it and/or
# * modify it under the terms of the GNU General Public License
# * as published by the Free Software Foundation; either version 2
# * of the License, or (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program; if not, write to the Free Software
# * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
********************************************************************************

    Classes and objects in C

    This file makes it easy to implement classes and objects in C. To define a
    class we need to perform three steps:

    1. Define the class prototype. This is suitable to go in a .h file for
    general use by other code.

    Example:

    C_CLASS(Foo)
        int x;
        int y;

        // This declares a method of a class Foo, called Con returning a
        // Foo object. In other words it is a constructor.
        Foo METHOD_VA(Foo, Con, int x, int y);
        int METHOD(Foo, add);
    END_C_CLASS

    2. Now we need to define some functions for the constructor and methods.
    Note that the constuctor is using ALLOCATE_CLASS to allocate space for the
    class structures.

    Foo Foo_Con(Foo self, int32_t x, int32_t y)
    {
        self->x = x;
        self->y = y;

        return self;
    }

    int32_t Foo_add(Foo this)
    {
        return (this->x + this->y);
    }

    3. Now we need to define the Virtual function table - these are those
    functions and attributes which are defined in this class. Basically
    these are all those things in the class definition above, with real
    function names binding them. (Note that by convention we precede the
    name of the method with the name of the class):

    VIRTUAL(Foo)
        VMETHOD(Con) = Foo_Con;
        VMETHOD(add) = Foo_add;
    END_VIRTUAL

*******************************************************************************/
#ifndef C_CLASS_H
#define C_CLASS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


#define C_CLASS(c_class)                                \
    typedef struct c_class* c_class;                    \
    extern void c_class ## _init(void);                 \
    extern void c_class ## _Alloc(c_class this);        \
    extern int32_t __ ## c_class ## _initialised;       \
    extern struct c_class __ ## c_class;                \
    struct c_class                                      \
    {                                                   \
        c_class __c_class__;

// Used when there are _exactly_ "2" items being passed in the macro
#define METHOD(c_class, name)                           (*name) (c_class this)

// Used when there are _more_ than "2" items being passed in the macro
#define METHOD_VA(c_class, name, ...)                   (*name) (c_class this, __VA_ARGS__)

// Used when there are _exactly_ "2" items being passed in the macro
#define CALL(x, method)                                 x->method(x)

// Used when there are _more_ than "2" items being passed in the macro
#define CALL_VA(x, method, ...)                         x->method(x, __VA_ARGS__)

// Ends a structure definition encapsulated in the C_CLASS macro
#define END_C_CLASS                                     };

/*******************************************************************************
    This is used to set the classes up for use:

    - class_init:
        checks the defined class' template to see if it has already
        been allocated, if not it is allocated within the global context.

    - class_Alloc
        Allocates memory for an instance of the class and fills the object
        with some templatized bare methods and attributes the class objects
        methods and attributes.

*******************************************************************************/
#ifdef __C_CLASS_DEBUG__
    #define VIRTUAL(c_class)                                                \
        struct c_class __ ## c_class;                                       \
        void c_class ## _init(void)                                         \
        {                                                                   \
            if(!__ ## c_class ## _initialised)                              \
            {                                                               \
                c_class ## _Alloc(&__ ## c_class);                          \
                __ ## c_class ## _initialised = 1;                          \
            }                                                               \
        }                                                                   \
        int32_t __ ## c_class ## _initialised = 0;                          \
        void c_class ## _Alloc(c_class this)                                \
        {                                                                   \
            ((Object) this)->__c_class__  = (Object) &__ ## c_class;        \
            this->__c_class__             = &__ ## c_class;                 \
            ((Object) this)->__size       = sizeof(struct c_class);         \
            ((Object) this)->__name__     = #c_class;
#else
    #define VIRTUAL(c_class)                                                \
        struct c_class __ ## c_class;                                       \
        void c_class ## _init(void)                                         \
        {                                                                   \
            if(!__ ## c_class ## _initialised)                              \
            {                                                               \
                c_class ## _Alloc(&__ ## c_class);                          \
                __ ## c_class ## _initialised = 1;                          \
            }                                                               \
        }                                                                   \
        int32_t __ ## c_class ## _initialised = 0;                          \
        void c_class ## _Alloc(c_class this)                                \
        {                                                                   \
            ((Object) this)->__c_class__  = (Object) &__ ## c_class;        \
            ((Object) this)->__size       = sizeof(struct c_class);
#endif

#define SET_DOCSTRING(string)                       ((Object) this)->__doc__ = string

// Ends a function definition encapsulated in the VIRTUAL macro
#define END_VIRTUAL                                 }

#define VMETHOD(method)                             this->method

#define VATTR(attribute)                            this->attribute

#define INIT_C_CLASS(c_class)                       if(!__ ## c_class ## _initialised)      \
                                                    {                                       \
                                                        c_class ## _init();                 \
                                                    }

#define NAMEOF(obj)                                 ((Object) obj)->__name__

#define DOCSTRING(obj)                              ((Object) obj)->__doc__

/*******************************************************************************
    This MACRO is used to construct a new C_Class using a constructor.

    This is done to try and hide the bare (unbound) method names in  order to
    prevent namespace pollution. (Bare methods may be defined as static within
    the implementation file). This macro ensures that class structures are
    initialised properly before calling their constructors.

    We require the following two arguments:

        1. class - the type of class to make
        2. constructor - The constructor method to use

        CONSTRUCT(Foo, const)
        CONSTRUCT_VA(Foo, const, param-1, param-2)

*******************************************************************************/
#ifdef __C_CLASS_DEBUG__
    /* Used when there are _exactly_ "2" items (c_class & constructor) being passed in the macro */
    #define CONSTRUCT(c_class, constructor)                                                         \
          (c_class) (c_class ## _init(),                                                            \
                   __ ## c_class.constructor((c_class) c_class_mem_dup((uint8_t*) &__ ## c_class,   \
                                                                sizeof(struct c_class),             \
                                                                 __location__ "(" #c_class ")")))

    /* Used when there are _more_ than "2" items being passed in the macro */
    #define CONSTRUCT_VA(c_class, constructor, ... )                                                \
          (c_class) (c_class ## _init(),                                                            \
                   __ ## c_class.constructor((c_class) c_class_mem_dup((uint8_t*) &__ ## c_class,   \
                                                                sizeof(struct c_class),             \
                                                                __location__ "(" #c_class ")"),     \
                                                                __VA_ARGS__))
#else
    /* Used when there are _exactly_ "2" items (c_class & constructor) being passed in the macro */
    #define CONSTRUCT(c_class, constructor)                                                         \
      (c_class) (c_class ## _init(),                                                                \
               __ ## c_class.constructor((c_class) c_class_mem_dup((uint8_t*) &__ ## c_class,       \
                                                            sizeof(struct c_class))))

    /* Used when there are _more_ than "2" items being passed in the macro */
    #define CONSTRUCT_VA(c_class, constructor, ...)                                                 \
      (c_class) (c_class ## _init(),                                                                \
               __ ## c_class.constructor((c_class) c_class_mem_dup((uint8_t*) &__ ## c_class,       \
                                                            sizeof(struct c_class)),                \
                                                            __VA_ARGS__))
#endif

typedef struct Object* Object;

// Templatized object methods and attributes.
struct Object {
    // A reference to a c_class instance - this is useful to be able to tell
    // which c_class an object really belongs to:
    Object  __c_class__;
    // And its super c_class:
    int8_t* __name__;
    // Objects may have a doc string associated with them.
    int8_t* __doc__;
    // How large the c_class is:
    int32_t __size;
};

static inline uint8_t* c_class_mem_dup(uint8_t const*  ptr,
                                       size_t const    size)
{
    assert(ptr);

    uint8_t* newp = (uint8_t*) malloc(size);

    assert(newp);

    if(newp)
        memcpy(newp, ptr, size);

    return newp;
}

#ifdef __cplusplus
extern "C" {
#endif

//    extern void* c_class_mem_dup(const void* p, size_t size);

#ifdef __cplusplus
}
#endif



#endif /* C_CLASS_H */

