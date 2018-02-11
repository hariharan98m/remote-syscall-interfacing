foo: foo.o addnum.o
	gcc -o foo foo.c addnum.c
foo.o: foo.c
	gcc -c foo.c -o foo.o

addnum.o: addnum.c
	gcc -c addnum.c -o addnum.o
