test: test.c buf.c
	gcc -Wall -o test test.c buf.c
clean:
	rm test
run:
	./test
