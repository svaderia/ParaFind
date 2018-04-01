

compileAll : binary_seach.o myRead.o walk.o
	gcc -g -o exe binary_seach.o myRead.o walk.o

binary_seach.o : binary_seach.c
	gcc -g -c binary_seach.c

myRead.o : myRead.c myRead.h
	gcc -g -c myRead.c

walk.o : walk.c myRead.h
	gcc -g -c walk.c

clean :
	rm *.o exe