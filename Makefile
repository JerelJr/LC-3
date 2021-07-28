SRC_FILES = src/lc3.c src/binops.c src/inbuffer.c src/memio.c

all: lc3 debug clean

lc3: $(SRC_FILES)
	gcc $(SRC_FILES) -o lc3

debug: $(SRC_FILES)
	gcc -g  $(SRC_FILES) -o lc3-debug

clean: 
	rm -f *~ *#
