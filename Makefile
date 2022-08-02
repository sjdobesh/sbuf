test: test.c object
	gcc -Wall  -o test buf.o test.c
object: buf.c
	gcc -Wall -fpic -c -o buf.o buf.c
lib: object
	gcc -Wall -shared -o libbuf.so buf.o
	sudo cp libbuf.so /usr/lib
	sudo cp buf.h /usr/include
	rm *.o libbuf.so
build: test.c lib
	gcc -Wall -O2 -L./ -o test test.c -lbuf
clean:
	rm test libbuf.so *.o
run:
	./test
