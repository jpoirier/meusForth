/**@file token_info.c
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
#include "types.h"
#include "config.h"

#include "xvm.h"
#include "token_info.h"


// Assumes these line up with the enumerations in xvm_inner.h!!!
TTokenInfo g_xvm_token_tab[MAX_TOKEN_NO] =
{
  1, (int8_t*) "NOOP",      (int8_t*) "( -- ) No operation.",
// 0

  1, (int8_t*) "DROP",      (int8_t*) "( x -- ) Remove x from the top of the data stack.",
  1, (int8_t*) "2DROP",     (int8_t*) "( x1 x2 -- ) Drop x1 and x2 from the top of the data stack.",
  1, (int8_t*) "DUP",       (int8_t*) "( x -- x x ) Duplicate x on the top of the data stack.",
  1, (int8_t*) "2DUP",      (int8_t*) "( x1 x2 -- x1 x2 x1 x2 ) Duplicate x1 and x2 on the top of the data stack.",
  1, (int8_t*) "SWAP",      (int8_t*) "( x1 x2 -- x2 x1 ) Swap x1 and x2 on the data stack.",
  1, (int8_t*) "PICK",      (int8_t*) "( +n -- x ) Put a copy of the n'th stack item on the top of the data stack. Index starts at 0.",
  1, (int8_t*) "2SWAP",     (int8_t*) "( x1 x2 x3 x4 -- x3 x4 x1 x2 ) Swap the top element pairs on the data stack.",
  1, (int8_t*) "OVER",      (int8_t*) "( x1 x2 -- x1 x2 x1 ) Put a copy of the second element on the top of the data stack.",
  1, (int8_t*) "2OVER",     (int8_t*) "( x1 x2 x3 x4 -- x1 x2 x3 x4 x1 x2 ) Put a copy of the second set of elements on the top of the data stack.",
  1, (int8_t*) "ROT",       (int8_t*) "( x1 x2 x3 -- x2 x3 x1 ) Rotate the top three elements of the data stack.",
 // 10

  1, (int8_t*) "2ROT",      (int8_t*) "( x1 x2 x3 x4 x5 x6 -- x3 x4 x5 x6 x1 x2) Rotate the top three element pairs of the data stack.",
  1, (int8_t*) "NIP",       (int8_t*) "( x1 x2 -- x2 ) Remove the second element from the data stack.",
  1, (int8_t*) "TUCK",      (int8_t*) "( x1 x2 -- x2 x1 x2) Put a copy of the top stack element below the second stack element.",
  1, (int8_t*) ">R",        (int8_t*) "(D: x -- ) (R: -- x ) Transfer the top element of the data stack to the return stack.",
  1, (int8_t*) "2>R",       (int8_t*) "(D: x1 x2 -- ) (R: -- x1 x2 ) Transfer the top two elements of the data stack to the return stack.",
  1, (int8_t*) "R>",        (int8_t*) "(R: x -- ) (D: -- x ) Transfer the top element of the return stack to data stack.",
  1, (int8_t*) "2R>",       (int8_t*) "(R: x1 x2 -- ) (D: -- x1 x2 ) Transfer the top two elements of the return stack to data stack.",
  1, (int8_t*) "R@",        (int8_t*) "(R: x -- x ) (D: -- x ) Copy the top of the return stack to data stack.",
  1, (int8_t*) "2R@",       (int8_t*) "(R: x1 x2 -- x1 x2 ) (D: -- x1 x2 ) Copy the top two elements of the return stack to the data stack.",
  1, (int8_t*) "@",         (int8_t*) "( addr -- x ) Reads 'x' from the specified RAM address 'addr' and places it on the top of the data stack.",
// 20

  1, (int8_t*) "!",         (int8_t*) "( x addr -- ) Writes x to the specified RAM address 'addr'.",
  1, (int8_t*) "AND",       (int8_t*) "( x1 x2 -- x1&x2 ) Bitwise AND of the top two elements of the data stack.",
  1, (int8_t*) "OR",        (int8_t*) "( x1 x2 -- x1|x2 ) Bitwise OR of the top two elements of the data stack.",
  1, (int8_t*) "XOR",       (int8_t*) "( x1 x2 -- x1^x2 ) Bitwise XOR of the top two elements of the data stack.",
  1, (int8_t*) "NOT",       (int8_t*) "( x -- ~x ) Bitwise NOT of the top elements of the data stack.",
  1, (int8_t*) "<<",        (int8_t*) "( x u -- x<<u ) Shifts x 'u' bits to the left",
  1, (int8_t*) ">>",        (int8_t*) "( x u -- x>>u ) Shifts x 'u' bits to the right",
  1, (int8_t*) "1+",        (int8_t*) "( x -- x+1 ) Increment x by a value of 1",
  1, (int8_t*) "1-",        (int8_t*) "( x -- x-1 ) Decrement x by a value of 1",
  1, (int8_t*) "+",         (int8_t*) "( x1 x2 -- x1+x2 ) Add x1 to x2.",
// 30

  1, (int8_t*) "-",         (int8_t*) "( x1 x2 -- x1-x2 ) Subtract x2 from x1.",
  1, (int8_t*) "*",         (int8_t*) "( x1 x2 -- x1*x2 ) Multiply x1 by x2.",
  1, (int8_t*) "/",         (int8_t*) "( x1 x2 -- x1/x2 ) Divide x1 by x2.",
  1, (int8_t*) "*/",        (int8_t*) "( x1 x2 x3 -- (x1*x2)/x3 ) Multiply x1 by x2 and divide the result by x3. Uses a 64 bit intermediate value.",

  1, (int8_t*) "MOD",       (int8_t*) "( x1 x2 -- remainder: x1/x2 ) Returns the remainder of x1 divided by x2.",
  1, (int8_t*) "/MOD",      (int8_t*) "( x1 x2 -- remainder: x1/x2 quotient: x1/x2 ) Returns the temainder and quotient of x1 divided by x2.",

  1, (int8_t*) "NEGATE",    (int8_t*) "( x1 -- -x1  ) Returns the two's compilemnt of x1.",
  1, (int8_t*) "ABS",       (int8_t*) "( x1 -- [x1] ) Returns the absolute value of x1.",

  1, (int8_t*) "MIN",       (int8_t*) "( x1 x2 -- x1|x2 ) Returns the smaller value of x1 and x2.",
  1, (int8_t*) "MAX",       (int8_t*) "( x1 x2 -- x1|x2 ) Returns the larger value of x1 and x2.",

  1, (int8_t*) ">",         (int8_t*) "( x1 x2 -- flag ) If x1 is greater-than x2 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) "<",         (int8_t*) "( x1 x2 -- flag ) If x1 is less-than x2 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) ">=",        (int8_t*) "( x1 x2 -- flag ) If x1 is greater-than or equal to x2 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) "<=",        (int8_t*) "( x1 x2 -- flag ) If x1 is less-than or equal to x2 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) "=",         (int8_t*) "( x1 x2 -- flag ) If x1 is equal to x2 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) "!=",        (int8_t*) "( x1 x2 -- flag ) If x1 is not-equal to x2 then flag equals TRUE (-1) else flag equals FALSE (0).",
 // 40

  1, (int8_t*) "=0",        (int8_t*) "( x -- flag ) If x is equal to 0 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) "!=0",       (int8_t*) "( x -- flag ) If x is not-equal to 0 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) ">0",        (int8_t*) "( x -- flag ) If x is greater-than 0 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) "<0",        (int8_t*) "( x -- flag ) If x is less-than 0 then flag equals TRUE (-1) else flag equals FALSE (0).",
  1, (int8_t*) "PC",        (int8_t*) "( -- pc ) Puts the 'Program Counter' on the top of stack.",
  1, (int8_t*) "SP",        (int8_t*) "( -- sp ) Puts the 'Data Stack Pointer' on the top of stack.",
  1, (int8_t*) "RP",        (int8_t*) "( -- rp ) Puts the 'Return Stack Pointer' on the top of stack.",
  1, (int8_t*) "FLUSH",     (int8_t*) "( x1 x2 x3 ... -- ) Flush the data stack of all elements.",
#if HAVE_PRINTF
  1, (int8_t*) ".",         (int8_t*) "( x -- ) Print the top element on the data stack.",
  1, (int8_t*) ".\" ...\"", (int8_t*) "( x -- ) Print the string within the quotes. Note, requires a space after the first quote. Ex: .\" Spam And Eggs!\"",
// 50

  1, (int8_t*) "LF",        (int8_t*) "( -- ) Print a line-feed character.",
#endif
  1, (int8_t*) "_LIT0",     (int8_t*) "( -- 0 ) Internal opcode - byte optimized (1 instead of 4) version of the value 0.",
  1, (int8_t*) "_LIT1",     (int8_t*) "( -- 1 ) Internal opcode - byte optimized (1 instead of 4) version of the value 1.",
  1, (int8_t*) "_LIT2",     (int8_t*) "( -- 2 ) Internal opcode - byte optimized (1 instead of 4) version of the value 2.",
  1, (int8_t*) "_LIT3",     (int8_t*) "( -- 3 ) Internal opcode - byte optimized (1 instead of 4) version of the value 3.",
  1, (int8_t*) "_LIT4",     (int8_t*) "( -- 4 ) Internal opcode - byte optimized (1 instead of 4) version of the value 4.",
  1, (int8_t*) "_LIT5",     (int8_t*) "( -- 5 ) Internal opcode - byte optimized (1 instead of 4) version of the value 5.",
  1, (int8_t*) "_LIT6",     (int8_t*) "( -- 6 ) Internal opcode - byte optimized (1 instead of 4) version of the value 6.",
  1, (int8_t*) "_LIT7",     (int8_t*) "( -- 7 ) Internal opcode - byte optimized (1 instead of 4) version of the value 7.",
  1, (int8_t*) "_LIT8",     (int8_t*) "( -- 8 ) Internal opcode - byte optimized (1 instead of 4) version of the value 8.",
 // 60

  1, (int8_t*) "_LIT9",     (int8_t*) "( -- 9 ) Internal opcode - byte optimized (1 instead of 4) version of the value 9.",
  1, (int8_t*) "_LIT10",    (int8_t*) "( -- 10 ) Internal opcode - byte optimized (1 instead of 4) version of the value 10.",
  1, (int8_t*) "_LIT11",    (int8_t*) "( -- 11 ) Internal opcode - byte optimized (1 instead of 4) version of the value 11.",
  1, (int8_t*) "_LIT12",    (int8_t*) "( -- 12 ) Internal opcode - byte optimized (1 instead of 4) version of the value 12.",
  1, (int8_t*) "_LIT13",    (int8_t*) "( -- 13 ) Internal opcode - byte optimized (1 instead of 4) version of the value 13.",
  1, (int8_t*) "_LIT14",    (int8_t*) "( -- 14 ) Internal opcode - byte optimized (1 instead of 4) version of the value 14.",
  1, (int8_t*) "_LIT15",    (int8_t*) "( -- 15 ) Internal opcode - byte optimized (1 instead of 4) version of the value 15.",
  1, (int8_t*) "_LIT-1",    (int8_t*) "( -- -1 ) Internal opcode - byte optimized (1 instead of 4) version of the value -1.",
  1, (int8_t*) "_GOTO",     (int8_t*) "( addr -- ) Internal opcode - jump to addr.",
  1, (int8_t*) "_CALL",     (int8_t*) "( addr -- ) Internal opcode - call addr.",
 // 70

  1, (int8_t*) "_RETURN",   (int8_t*) "( -- ) Internal opcode - return from call.",
  1, (int8_t*) "_JZ",       (int8_t*) "( reladdr x -- ) Internal opcode - jump to reladdr if x == 0.",
  1, (int8_t*) "_JNZ",      (int8_t*) "( reladdr x -- ) Internal opcode - Jump to reladdr if x != 0.",
  1, (int8_t*) "_RETZ",     (int8_t*) "( x -- ) Internal opcode - return if x == 0.",
  1, (int8_t*) "_RETNZ",    (int8_t*) "( x -- ) Internal opcode - return if x != 0.",
  2, (int8_t*) "_LIT",      (int8_t*) "( -- val ) Internal opcode - a 4 byte fixed point literal value.",
  2, (int8_t*) "_SJMP",     (int8_t*) "( -- ) Internal opcode - .",
  2, (int8_t*) "_SJZ",      (int8_t*) "( a -- ) Internal opcode - .",
  2, (int8_t*) "_SJNZ",     (int8_t*) "( a -- ) Internal opcode - .",
  4, (int8_t*) "_LLIT",     (int8_t*) "( -- val ) Internal opcode - .",
// 80

  3, (int8_t*) "_LGOTO",    (int8_t*) "( -- ) Internal opcode - .",
  3, (int8_t*) "_LJMP",     (int8_t*) "( -- ) Internal opcode - .",
  3, (int8_t*) "_LCALL",    (int8_t*) "( -- ) Internal opcode - .",
  3, (int8_t*) "_LJZ",      (int8_t*) "( a -- ) Internal opcode - .",
  3, (int8_t*) "_LJNZ",     (int8_t*) "( a -- ) Internal opcode - .",
// 85
#if HAVE_FP_NUMBERS
  4, (int8_t*) "_LLITFP",   (int8_t*) "( -- val ) Internal opcode - a 4 byte floating point literal value.",
  1, (int8_t*) "F1+",       (int8_t*) "( x1 -- x1+1 ) Floating point increment.",
  1, (int8_t*) "F+",        (int8_t*) "( x1 x2 -- x1+x2 ) Floating point addition.",
  1, (int8_t*) "F-",        (int8_t*) "( x1 x2 -- x1-x2 ) Floating point subtraction.",
  1, (int8_t*) "F*",        (int8_t*) "( x1 x2 -- x1*x2 ) Floating point multiplication.",
  1, (int8_t*) "F/",        (int8_t*) "( x1 x2 -- x1/x2 ) Floating point division.",
  1, (int8_t*) "F*/",       (int8_t*) "( x1 x2 x3 -- x1*x2/x3 ) Floating point multiplication and division. Uses a 64 bit intermediate value.",
// 92
#endif
#if HAVE_FFI
  3, (int8_t*) "_FFICALL",  (int8_t*) "( x -- ) Internal opcode - .",
#endif
#if HAVE_NXT
    // lejos.nxt.LCD
    1, (int8_t*) "DRAWSTRING",       (int8_t*) "( addr x2 x3 x4 -- ) .",
    1, (int8_t*) "DRAWINT",       (int8_t*) "( x1 x2 x3 x4 -- ) .",
    1, (int8_t*) "DRAWINT",       (int8_t*) "( x1 x2 x3 x4 -- x5 ) .",
    1, (int8_t*) "REFRESH",       (int8_t*) "( -- ) .",
    1, (int8_t*) "CLEAR",       (int8_t*) "( -- ) .",
    1, (int8_t*) "SETDISPLAY",       (int8_t*) "( x -- ) .",
    1, (int8_t*) "GETDISPLAY",       (int8_t*) "( -- addr ) .",
    1, (int8_t*) "SETAUTOREFRESH",       (int8_t*) "( x -- ) .",

    1, (int8_t*) "BITBLT",       (int8_t*) "( addr x1 x2 x3 x4 addr x5 x6 x7 x8 x9 x10 x11-- ) .",
    1, (int8_t*) "GETSYSTEMFONT",       (int8_t*) "( -- addr ) .",

    //lejos.nxt.Battery
    1, (int8_t*) "GETVOLTAGEMILLIVOLT",       (int8_t*) "( -- x ) .",

    //lejos.nxt.Button
    1, (int8_t*) "GETBUTTONS",       (int8_t*) "( -- x ) .",

    //lejos.nxt.MotorPort
    1, (int8_t*) "GETTACHOCOUNTBYID",       (int8_t*) "( x1 -- x2 ) .",
    1, (int8_t*) "CONTROLMOTORBYID",       (int8_t*) "( x1 x2 x3 -- ) .",
    1, (int8_t*) "RESETTACHOCOUNTBYID",       (int8_t*) "( x -- ) .",

    //lejos.nxt.SensorPort
    1, (int8_t*) "I2CENABLEBYID",       (int8_t*) "( x -- ) .",
    1, (int8_t*) "I2CBUSYBYID",       (int8_t*) "( x1 -- x2 ) .",
    1, (int8_t*) "I2CSTARTBYID",       (int8_t*) "( x1 x2 x3 x4 addr x5 x6 -- x7 ) .",
    1, (int8_t*) "I2CDISABLEBYID",       (int8_t*) "( x -- ) .",

    //lejos.nxt.Sound
    1, (int8_t*) "PLAYFREQ",       (int8_t*) "( x1 x2 x3 -- ) .",
    1, (int8_t*) "PLAYSAMPLE",       (int8_t*) "( x1 x2 x3 x4 x5 -- ) .",
    1, (int8_t*) "GETTIME",       (int8_t*) "( -- x ) .",

    //lejos.nxt.comm.Bluetooth
    1, (int8_t*) "BTSEND",       (int8_t*) "( addr x -- ) .",
    1, (int8_t*) "BTRECEIVE",       (int8_t*) "( addr -- ) .",
    1, (int8_t*) "BTGETBC4CMDMODE",       (int8_t*) "( -- x ) .",
    1, (int8_t*) "BTSETARMCMDMODE",       (int8_t*) "( x -- ) .",


    1, (int8_t*) "BTSTARTADCONVERTER",       (int8_t*) "( -- x ) .",
    1, (int8_t*) "BTSETRESETLOW",       (int8_t*) "( -- x ) .",
    1, (int8_t*) "BTSETRESETHIGH",       (int8_t*) "( -- x ) .",

    1, (int8_t*) "BTWRITE",       (int8_t*) "( addr x1 x2 -- x3) .",
    1, (int8_t*) "BTREAD",       (int8_t*) "( addr x1 x2 -- x3) .",
    1, (int8_t*) "BTPENDING",       (int8_t*) "( -- x ) .",

    //lejos.nxt.comm.USB
    1, (int8_t*) "USBREAD",       (int8_t*) "( addr x1 x2 -- x3) .",
    1, (int8_t*) "USBWRITE",       (int8_t*) "( addr x1 x2 -- x3) .",
    1, (int8_t*) "USBRESET",       (int8_t*) "( -- ) .",

    //lejos.nxt.Flash
    1, (int8_t*) "WRITEPAGE",       (int8_t*) "( x1 x2 -- ) .",
    1, (int8_t*) "READPAGE",       (int8_t*) "( x1 x2 -- ) .",
    1, (int8_t*) "EXEC",       (int8_t*) "( x1 x2 -- ) .",
    //lejos.nxt.DebugInterface
    1, (int8_t*) "EXECUTEPROGRAM",       (int8_t*) "( x -- ) .",
    1, (int8_t*) "SETDEBUG",       (int8_t*) "( -- ) .",
    1, (int8_t*) "PEEKWORD",       (int8_t*) "( x1 -- x2 ) .",
    1, (int8_t*) "EVENTOPTIONS",       (int8_t*) "( x1 x2 -- x ) .",
    1, (int8_t*) "SUSPENDTHREAD",       (int8_t*) "( addr -- ) .",
    1, (int8_t*) "RESUMETHREAD",        (int8_t*) "( addr -- ) .",
    //System
    1, (int8_t*) "GETPROGRAMEXECUTIONSCOUNT",        (int8_t*) "( -- x ) .",
    1, (int8_t*) "GETFIRMWAREREVISION",        (int8_t*) "( -- x ) .",
    1, (int8_t*) "GETFIRMWAREMAJORVERSION",        (int8_t*) "( -- x ) .",
    1, (int8_t*) "GETFIRMWAREMINORVERSION",        (int8_t*) "( -- x ) .",

    //USB
    1, (int8_t*) "USBSETSERIALNO",        (int8_t*) "( addr -- ) .",
    1, (int8_t*) "USBSETNAME",        (int8_t*) "( addr -- ) .",
    1, (int8_t*) "USBSTATUS",        (int8_t*) "( -- x ) .",
    1, (int8_t*) "USBENABLE",        (int8_t*) "( x --  ) .",
    1, (int8_t*) "USBDISABLE",        (int8_t*) "( --  ) .",
#endif
  0, 0, 0
};


// end of file
