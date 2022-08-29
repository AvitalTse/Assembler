Assembler:Main.o PreAssembler.o AssemblerFirst.o SharedFunctions.o Header.o AssemblerSecond.o ErrorsMain.o ErrorsFunctions.o
	gcc -g -ansi -Wall -pedantic Main.o PreAssembler.o AssemblerFirst.o SharedFunctions.o Header.o AssemblerSecond.o ErrorsMain.o ErrorsFunctions.o -o Assembler -lm
AssemblerFirst.o: AssemblerFirst.c AssemblerFirst.h ConstTab.h ConstVal.h 
	gcc -c -g -ansi -Wall AssemblerFirst.c -o AssemblerFirst.o
AssemblerSecond.o:AssemblerSecond.c AssemblerSecond.h  ConstTab.h ConstVal.h 
	gcc -c -g -ansi -Wall  AssemblerSecond.c -o AssemblerSecond.o -lm
SharedFunctions.o: SharedFunctions.c AssemblerFirst.h 
	gcc -c -g -ansi -Wall SharedFunctions.c -o SharedFunctions.o
Main.o: Main.c PreAssembler.h ConstVal.h  AssemblerSecond.h
	gcc -c -g -ansi -Wall Main.c -o Main.o
PreAssembler.o:PreAssembler.c PreAssembler.h ConstVal.h
	gcc -c -g -ansi -Wall PreAssembler.c -o PreAssembler.o
Header.o:Header.c ConstVal.h ConstTab.h
	gcc -c -g -ansi -Wall Header.c -o Header.o 
ErrorsMain.o:ErrorsMain.c Errors.h 
	gcc -c -g -ansi -Wall ErrorsMain.c -o ErrorsMain.o
ErrorsFunctions.o:ErrorsFunctions.c Errors.h 
	gcc -c -g -ansi -Wall ErrorsFunctions.c -o ErrorsFunctions.o 
