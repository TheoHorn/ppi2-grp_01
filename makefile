CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=parser_csv_test calculate_distance_test dijkstra

all: $(ALL_EXECUTABLES)

parser_csv_test: src/test/csv_loading_test.c src/parser_csv.c src/parser_csv.h
	${CC} ${CFLAGS} -o $@ $<

calculate_distance_test: src/test/calculate_distance.c src/station.c src/station.h
	${CC} ${CFLAGS} -o $@ $<


time_distance_calcul: src/performances/time_distance_calcul.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/performances/$@

time_distance_calcul.o: src/performances/time_distance_calcul.c src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o $@

degree_of_station: src/performances/degree_of_station.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/performances/$@

degree_of_station.o: src/performances/degree_of_station.c src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o $@

dijkstra: src/dijkstra.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/$@

dijkstra.o: src/dijkstra.c src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o $@

station.o : src/station.c src/station.h
	
clean:
	rm -f *.o ${ALL_EXECUTABLES}

.SILENT: clean test


