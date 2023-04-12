CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS+= `pkg-config --cflags gtk+-3.0`
LDFLAGS+= -fsanitize=address
LDFLAGS+= `pkg-config --libs gtk+-3.0`

ALL_EXECUTABLES=parser_csv_test calculate_distance_test france_map search_button_test dijkstra graphics

all: $(ALL_EXECUTABLES)

#Algorithme
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

#Graphics
france_map.o : src/graphics/france_map.c src/graphics/france_map.h src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o $@

france_map: src/graphics/france_map.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/graphics/$@

search_button.o : src/graphics/search_button.c src/graphics/search_button.h src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o $@

search_button: src/graphics/search_button.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/graphics/$@

graphics.o : src/graphics/graphics.c src/graphics/france_map.c src/graphics/france_map.h src/graphics/search_button.c src/graphics/search_button.h src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o src/graphics/$@

graphics: src/graphics/graphics.o src/graphics/search_button.o src/graphics/france_map.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

#Tests
parser_csv_test: src/test/csv_loading_test.c src/parser_csv.c src/parser_csv.h
	${CC} ${CFLAGS} -o $@ $<

calculate_distance_test: src/test/calculate_distance.c src/station.c src/station.h
	${CC} ${CFLAGS} -o $@ $<

search_button_test: src/test/search_button_test.c src/graphics/search_button.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

france_map_test: src/test/france_map_test.c src/graphics/france_map.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


clean:
	find . -name '*.o' -type f -delete
	find . -name '$(ALL_EXECUTABLES)' -type f -delete

.SILENT: clean test


