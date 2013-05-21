building readline libraries
---------------------------

> ./configure --enable-static -disable-shared 

In the make file change/add

CC=icc 
AR=xiar 
CFLAGS =-std=c99 
CFLAGS += -O3

> make all


Dec 19, 2008
------------
- Fixed a HEX keyword conflict in the lexer.re file
- Fixed a bug where the values 0 and 1 were being eaten by the imported
  function handler. The value was seen as the return count of the function
  being imported - removing 0-9 from the function name identifier fixed the problem.
- Excluded internal opcodes from being printed out
- Change class identifier to c_class and class.h to c_class.h
- Added stack checking When doing floating point operations
- Added MOD, /MOD, NEGATE, ABS, MIN, and MAX words
- Removed an extraneous _Alloc declaration from the VIRTUAL macro in c_class.h
  that was causing a warning about an external declaration in the primary
  source file.




Dec 18, 2008
------------
- Added a REMOVE directive to remove an individual word from the dictionary
- Some clean-up...the EOL problem noted on Nov 28th seems to be fixed
- Removed the printing from the compiler_error function for now. Just call
  the desired printf before calling the error function
- Major clean-up of the get_line function

Nov 28, 2008
------------
- The FFI doesn't seem to work when used in a file unless the last line is an EOL. Why?
- Exiting in the middle of an import operation causes an exception. Why?
- I had to recompile the d.dylib for it to work

General
-------
- FFI:
    [IMPORT|import]
        [0|1] function_name [0-9]
    [IMPORTEND|endimport]

- run: make -f [Makefile.osx|Makefile.win|Makefile.nix]

---------------------- the old way ---------------------------------------------
- Running make: First "re2c" is used to build lexer.c from lexer.re. Then the
  "C" files are compiled which creates the xvm executable.
- To build, run "make all" from the console directory.
--------------------------------------------------------------------------------


Aug 25th, 2008
--------------
Stopped in the files.c file at 128 - 133 lines. The handling of the files.

Sept 9th, 2008
--------------
Currently working on the "#include and include" commands.





todo
----
- Add commands buffer and the handling of the up key to display previously used commands
- Fix the clearing of the buffer when bad token is parsed
- Fixme: is this a 16 or 32 bit stack machine; the data and return stack are both arrays of
  uint32_t types and there's no checking that the input is larger than the valid size nor is
  there any checking for overflow or shift counts. If the compiled byte code allows 4 byte
  literal values then it shouldn't matter!
- show_stack

            Sept 12, 2008
            -------------
- in the d_macro_name function replace the is_number function
- in the d_macro_value function replace the is_number function
- fix the token value printed from within debug_tok

- try and find a more efficient way to handle name within the name_push function

- macro definitions for all integer and float types
- check that the current macro definition handler works
- add the checksum to the compa process

- what's the difference between compiler_reset() and compiler_init() and they're called?
- Fix all the string handling code!!!

            Sept 14, 2008
            -------------
- I think I have some of the string handling under control, at least for name_push and name_pop

- the d_macro_value function doesn't seem to distinguish between scalar and floating point numbers


MISC
----
To add a token
    - add token to xvm_inner.h
    - add token and code to xvm_inner.c (same order as in xvm_inner.h)
            * be sure to increment the enum and array size values
    - add code and call to lexer.re
    - update token_info.c (same order as in xvm_inner.h)

