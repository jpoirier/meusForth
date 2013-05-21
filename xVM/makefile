
# Console

ifeq "$(gcc)" "true"
	PLATFORM_COMPILER = GCC
	OPTIONS = -std=c99
	CC = gcc
	CXX = g++
	AR = ar
else
	PLATFORM_COMPILER = INTEL
	OPTIONS = -std=c99 -align -Wpointer-arith -Wuninitialized -restrict 
	CC = icc
	CXX = icpc
	AR = xiar
endif

ifeq "$(debug)" "true"
    OPTIONS += -g -DDEBUG
else
    OPTIONS += -DNDEBUG
endif

ifeq "$(noopt)" "true"
    # Nothing here
else
    OPTIONS += -O3
endif

OPTIONS += -Wall

INCLUDE = -I.

build_file:
	$(CC) -c $(INCLUDE) $(OPTIONS) $(file)

all:
	$(RM) xvm.a

	$(CC) -c $(INCLUDE) $(OPTIONS) xvm.c
	$(CC) -c $(INCLUDE) $(OPTIONS) xvm_inner.c

#	$(CC) $(OPTIONS) $(INCLUDE) xvm.c xvm_inner.c  -o xvm_console

	$(AR) -rcs xvm.a xvm.o xvm_inner.o 

clean:
	$(RM) *.o
	$(RM) xvm.a


	


