test: test.c lib
	gcc -Wall -Og -L./ -o test test.c -lbuf
object: buf.c
	gcc -Wall -c -o buf.o buf.c
lib: object
	gcc -Wall -shared -o libbuf.so buf.o
build: test.c lib
	gcc -Wall -O2 -L./ -o test test.c -lbuf
clean:
	rm test libbuf.so *.o
run:
	./test
