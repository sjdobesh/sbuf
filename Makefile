test: test.c object
	gcc -Wall -g -o test buf.o test.c
object: buf.c
	gcc -Wall -O2 -fpic -c -o buf.o buf.c
lib: object
	gcc -Wall -O2 -shared -o libbuf.so buf.o
	sudo cp libbuf.so /usr/lib
	sudo cp buf.h /usr/include
	rm *.o libbuf.so
clean:
	rm test libbuf.so *.o
run:
	./test
