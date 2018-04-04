run: exe
	./exe ${t} > ans.out

compileAll : binary_seach.o myRead.o walk.o
	gcc -g -fopenmp -o exe binary_seach.o myRead.o walk.o

binary_seach.o : binary_seach.c binary.h
	gcc -g -fopenmp -c binary_seach.c

myRead.o : myRead.c myRead.h
	gcc -g -fopenmp -c myRead.c

walk.o : walk.c myRead.h binary.h
	gcc -g -fopenmp -c walk.c

clean :
	rm *.o exe

testcase: 
	python test/maketestcase.py ${d}

diff:
	diff test/answer.out ans.out
