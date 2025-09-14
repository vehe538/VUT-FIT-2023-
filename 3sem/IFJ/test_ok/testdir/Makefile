# Makefile for IFJ project
# Authors:
# - Milan Babuljak (xbabulm00), 2024
# - Jan Rous (xrousja00), 2024
# - Juraj Mesík (xmesikj00), 2024
# - Marek Chalupka (xchalu18), 2024

CFLAGS=-std=c11
.PHONY: run clean

all:
	cc $(CFLAGS) -o ifjcompiler codegen.c error.c expr_stack.c main.c parser.c prec_table.c psa.c symtable.c tokeniser.c

run: all
	./ifjcompiler

clean:
	rm ifjcompiler
