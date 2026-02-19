CFLAGS = -Wall -Wextra -std=c99 -g
SRC = compiler.c parser.c scanner.c symtable.c error.c ast.c psa.c precedence_table.c expr_stack.c semantics.c generator.c
TARGET = compiler 

all: $(TARGET) 

$(TARGET): $(SRC) 
	gcc $(CFLAGS) -o $@ $^ -lm

clean: 
	rm -f $(TARGET)