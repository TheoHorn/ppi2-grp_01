CC=clang
CFLAGS=-Wall -Wextra -pedantic
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls

ALL_EXECUTABLES=parser_csv_test calculate_distance_test

all: $(ALL_EXECUTABLES)

parser_csv_test: src/test/csv_loading_test.c src/parser_csv.c src/parser_csv.h
	${CC} ${CFLAGS} -o $@ $<

calculate_distance_test: src/test/calculate_distance.c src/station.c src/station.h
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o ${ALL_EXECUTABLES}

