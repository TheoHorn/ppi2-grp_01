CC=clang
CFLAGS=-Wall -Wextra -pedantic
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls

ALL_EXECUTABLES=parser_csv

all: $(ALL_EXECUTABLES)

parser_csv: src/test/csv_loading_test.c src/parser_csv.c src/parser_csv.h
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o ${ALL_EXECUTABLES}

