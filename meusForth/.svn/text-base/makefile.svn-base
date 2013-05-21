
# meusForth

OPTIONS +=  -DREADLINE_LIBRARY

all:
	$(RM) meusForth$(EXE_EXT)
	$(RM) ../release/meusForth$(EXE_EXT)

	re2c -b -s -i -o lexer.c lexer.re

	$(CC) -c $(INCLUDE) $(OPTIONS) code.c
	$(CC) -c $(INCLUDE) $(OPTIONS) compiler.c
	$(CC) -c $(INCLUDE) $(OPTIONS) debug.c
	$(CC) -c $(INCLUDE) $(OPTIONS) directives.c
	$(CC) -c $(INCLUDE) $(OPTIONS) ffi.c
	$(CC) -c $(INCLUDE) $(OPTIONS) files.c
	$(CC) -c $(INCLUDE) $(OPTIONS) hash.c
	$(CC) -c $(INCLUDE) $(OPTIONS) lexer.c
	$(CC) -c $(INCLUDE) $(OPTIONS) macro.c
	$(CC) -c $(INCLUDE) $(OPTIONS) main.c
	$(CC) -c $(INCLUDE) $(OPTIONS) parser.c
	$(CC) -c $(INCLUDE) $(OPTIONS) stack.c
	$(CC) -c $(INCLUDE) $(OPTIONS) token_info.c
	$(CC) -c $(INCLUDE) $(OPTIONS) utilities.c
	$(CC) -c $(INCLUDE) $(OPTIONS) variable.c
	$(CC) -c $(INCLUDE) $(OPTIONS) voc.c


	$(CC) -c $(INCLUDE) $(OPTIONS) ../xVM/xvm.c
	$(CC) -c $(INCLUDE) $(OPTIONS) ../xVM/xvm_inner.c

	$(RM) xvm

	$(CC) $(OPTIONS) $(INCLUDE) code.o compiler.o debug.o directives.o files.o ffi.o hash.o lexer.o macro.o main.o parser.o stack.o token_info.o utilities.o variable.o voc.o xvm.o xvm_inner.o $(HISTORY_LIB) $(READLINE_LIB) $(TERMCAP_LIB) -o meusForth$(EXE_EXT)
	$(COPY) ./meusForth$(EXE_EXT) ../release  

preprocess:
	$(CC) -P $(INCLUDE) $(OPTIONS) stack.c

clean:
	$(RM) $(LIBFILE) $(OBJS) meusForth$(EXE_EXT) ../release/meusForth$(EXE_EXT)



	


