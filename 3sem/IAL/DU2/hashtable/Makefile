CC=gcc
CFLAGS=-Wall -std=c11 -pedantic 
FILES=hashtable.c test.c test_util.c
TEST=test

.PHONY: test clean run

run: $(TEST)
	@./$(TEST) > current-test.out
	@diff -u --report-identical-files --text  ../ref_hash.out current-test.out
	

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

clean:
	rm -f test
