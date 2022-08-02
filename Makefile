test: test.c lib
	gcc -Wall -L./ -o test test.c -lbuf
buf.o: buf.c
	gcc -Wall -c -o buf.o buf.c
clean:
	rm test libbuf.so *.o
run:
	./test
lib: buf.o
	gcc -Wall -shared -o libbuf.so buf.o
