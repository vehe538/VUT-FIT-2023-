CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm   
FILES=btree.c ../btree.c ../test_util.c ../test.c ../character.c

.PHONY: test clean

all: 
	@./test > current-test.out
	@echo "--------------------------------------------------------------------"
	@diff -u --report-identical-files --text ../../ref_rec.out current-test.out
	@echo "--------------------------------------------------------------------"

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

clean:
	rm -f test
