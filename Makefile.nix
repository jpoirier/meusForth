#
#
# Makefile.nix
#
#

OS_PLATFORM = NIX
HW_PLATFORM = PC

DIR_LIB = ../readline
TERMCAP_LIB = -ltermcap
READLINE_LIB = ../readline/libreadline.a
HISTORY_LIB = ../readline/libhistory.a


ifeq "$(gcc)" "true"
	PLATFORM_COMPILER = GCC
	OPTIONS = -std=c99 -DHAVE_VA_COPY -DPC_PLATFORM -DNIX
	CC = gcc
	CXX = g++
	AR = ar
else
	PLATFORM_COMPILER = INTEL
	OPTIONS = -std=c99 -align -Wpointer-arith -Wuninitialized -DPC_PLATFORM -DNIX
	CC = icc
	CXX = icpc
	AR = xiar
endif

OPTIONS += -Wall -DCONSOLE_MODE

COPY = cp
AROPTIONS = -rcs

EXE_EXT =
LIB_EXT = a

INCLUDE =

export

include ./makefile.common

