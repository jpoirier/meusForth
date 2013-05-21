@echo off
cls

echo ~                                                                          
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo !!!!!!!!!!  !!!!!!!!!!
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo `              

set INC_XVM=./xVM         
set INC_MEUSFORTH=./meusForth                                                    


REM SET CL_EXE="C:/Program Files/Microsoft Visual Studio 8/VC/bin/cl"
REM SET LIB_EXE="C:/Program Files/Microsoft Visual Studio 8/VC/bin/lib"
REM SET LIB_EXE=%ICPP_COMPILER10%\IA32\Lib";"%ICPP_COMPILER10%\EM64T\Lib
REM CALL "C:/Program Files/Intel/Compiler/C++/10.1.011/IA32/Bin/ICLVars.bat"
REM CALL "C:/Program Files/Microsoft Visual Studio 8/Common7/Tools/vsvars32.bat"
REM call "C:/Program Files/Microsoft Visual Studio 8/VC/Bin/Vcvars32.bat"
REM /DDEBUG
REM /DMSVC

REM /O2     = maximize speed
REM /Ox     = maximum optimizations
REM /Og     = enable global optimization
REM /I      = <dir> add to include search path
REM /Wall   = enable all warnings
REM /c      = compile only, no link
REM /EHa    = enable C++ EH (w/ SEH exceptions)

REM SET OPTIONS = /Qstd=c99 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS /DWIN32 /D_CONSOLE /Dconsole
SET OPTIONS = -D"_CRT_SECURE_NO_DEPRECATE" -D"_CRT_SECURE_NO_WARNINGS" -D"WIN32" -D"_CONSOLE" -D"CONSOLE_MODE" D"ICC_WIN"
SET PATH=C:\Users\Projects\xVM\tcc-0924;%PATH%

re2c -b -s -o ./meusForth/lexer.c ./meusForth/lexer.re

tcc %OPTIONS% -I%INC_XVM% -I%INC_MEUSFORTH% -o meusForth_tcc_bld.exe ./meusForth/code.c ./meusForth/compiler.c ./meusForth/debug.c ./meusForth/directives.c ./meusForth/files.c ./meusForth/lexer.c ./meusForth/macro.c ./meusForth/main.c ./meusForth/parser.c ./meusForth/stack.c ./meusForth/token_info.c ./meusForth/utilities.c ./meusForth/voc.c ./xVM/xvm.c ./xVM/xvm_inner.c

REM del *.obj





