CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm
FILES_REC=exa.c ../rec/btree.c ../btree.c ../test_util.c ../test.c ../character.c
FILES_ITER=exa.c ../iter/btree.c ../iter/stack.c ../btree.c ../test_util.c ../test.c ../character.c

.PHONY: test clean

all:
	@echo "------------------------ iter ---------------------------------"
	@diff -u --report-identical-files --text ../../ref_exa_iter.out
	@echo "------------------------- rec --------------------------------"
	diff -u --report-identical-files --text ../../ref_exa_rec.out
test: $(FILES_REC)
	$(CC) -DEXA=1 $(CFLAGS) -o $@_rec $(FILES_REC)
	$(CC) -DEXA=1 $(CFLAGS) -o $@_iter $(FILES_ITER)

clean:
	rm -f test_rec
	rm -f test_iter
