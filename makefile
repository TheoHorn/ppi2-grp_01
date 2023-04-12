CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS+= `pkg-config --cflags gtk+-3.0`
LDFLAGS+= -fsanitize=address
LDFLAGS+= `pkg-config --libs gtk+-3.0`


ALL_EXECUTABLES=parser_csv_test calculate_distance_test time_distance_calcul_test degree_of_station_test interface_graphique search_button dijkstra_test france_map search_button_test dijkstra graphics
ALL_O=parser_csv.o station.o car.o dijkstra.o search_button.o interface_graphique.o

all: $(ALL_EXECUTABLES)

create_o: $(ALL_O)

# --- .o ---
parser_csv.o: src/utils/parser_csv.c src/utils/parser_csv.h
	${CC} $(CFLAGS) -c $< -o src/utils/$@

station.o : src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o src/$@

car.o : src/car.c src/car.h
	$(CC) $(CFLAGS) -c $< -o src/$@

dijkstra.o: src/dijkstra.c src/station.o
	$(CC) $(CFLAGS) -c $< -o src/$@

search_button.o : src/graphics/search_button.c src/station.o
	$(CC) $(CFLAGS) -c $< -o src/graphics/$@

interface_graphique.o : src/graphics/interface_graphique.c src/station.o
	$(CC) $(CFLAGS) -c $< -o src/graphics/$@
#   -----------------

# --- Tests ---
parser_csv_test: $(parser_csv.o) src/test/csv_loading_test.c src/utils/parser_csv.o
	$(MAKE) parser_csv.o
	${CC} $(CFLAGS) -o $@ $<

calculate_distance_test:  src/test/calculate_distance.c src/station.o
	$(MAKE) station.o
	${CC} $(CFLAGS) -o $@ $<

dijkstra_test: src/test/dijkstra_test.c src/dijkstra.o src/station.o src/utils/parser_csv.o
	$(MAKE) station.o parser_csv.o dijkstra.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

#  --- Performances tests ---

time_distance_calcul_test: src/test/performances/time_distance_calcul.c src/station.o src/utils/parser_csv.o
	$(MAKE) station.o parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

degree_of_station_test: src/test/performances/degree_of_station.c src/station.o src/utils/parser_csv.o
	$(MAKE) station.o parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

#  -----------------

#  --- Graphics ---
interface_graphique: src/graphics/interface_graphique.o src/station.o src/utils/parser_csv.o
	$(MAKE) station.o parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

#search_button: src/graphics/search_button.o src/station.o src/utils/parser_csv.o
#	$(MAKE) station.o parser_csv.o
#	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

#to be modified
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


#  --- Graphics Tests ---
search_button_test: src/test/search_button_test.c src/graphics/search_button.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

france_map_test: src/test/france_map_test.c src/graphics/france_map.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
#   -------



clean:
	find . -name '*.o' -type f -delete
	@echo "Tout les fichiers .o ont été supprimés"
	rm -f $(ALL_EXECUTABLES)
	@echo "Tout les fichiers exécutables ont été supprimés"

.SILENT: clean test


