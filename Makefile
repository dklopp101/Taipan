launch_gedit:
	@ gedit Makefile asm.c asm.h symbol.c symbol.h token.c token.h error.c error.h

build_asm_core:
	gcc -o hyasm asm.c error.c token.c symbol.c ../hvm.opcodes.c

build_tester:
	gcc -o tester tests/main.c asm.c error.c token.c symbol.c ../hvm.opcodes.c
