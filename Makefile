test: test.c lib
	gcc -Wall -L./ -o test test.c -lbuf
object: buf.c
	gcc -Wall -c -o buf.o buf.c
clean:
	rm test libbuf.so *.o
lib: object
	gcc -Wall -shared -o libbuf.so buf.o
run:
	./test
