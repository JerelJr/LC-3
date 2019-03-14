all: lc3 debug clean

lc3: lc3.c
	gcc lc3.c -o lc3

debug: lc3.c
	gcc -g lc3.c -o lc3-debug

clean: 
	rm -f *~ *#
