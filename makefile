CC=clang
CFLAGS=-Wall -Wextra -pedantic
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls

ALL_EXECUTABLES=parser_csv

all: $(ALL_EXECUTABLES)

test_parser_csv: src/test/csv_loading_test.c

clean:
	rm -f *.o ${ALL_EXECUTABLES}

