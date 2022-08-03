test: test.c object
	gcc -Wall -Wextra -Werror -pedantic -g3 -o test sbuf.o test.c
object: sbuf.c
	gcc -Wall -Wextra -Werror -pedantic -Os -fpic -c -o sbuf.o sbuf.c
lib: object
	gcc -Wall -Wextra -Werror -pedantic -Os -shared -o libsbuf.so sbuf.o
	sudo cp libsbuf.so /usr/lib
	sudo cp sbuf.h /usr/include
	rm *.o libsbuf.so
clean:
	rm test libsbuf.so *.o > /dev/null 2>&1
run:
	./test
