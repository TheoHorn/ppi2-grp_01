CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS+= `pkg-config --cflags gtk+-3.0`
LDFLAGS+= -fsanitize=address
LDFLAGS+= `pkg-config --libs gtk+-3.0`


ALL_EXECUTABLES=parser_csv_test calculate_distance_test dijkstra_test time_distance_calcul_test degree_of_station_test france_map search_button graphics search_button_test france_map_test
ALL_O= parser_csv.o station.o car.o dijkstra.o search_button.o france_map.o graphics.o

all: $(ALL_EXECUTABLES)

create_o: $(ALL_O)

# ---- Lancement du programm ----
program: src/main.c
	$(MAKE) needed.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
# -----------

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

france_map.o : src/graphics/france_map.c src/station.o
	$(CC) $(CFLAGS) -c $< -o src/graphics/$@

graphics.o : src/graphics/graphics.c src/graphics/france_map.o src/graphics/search_button.o src/station.o src/utils/parser_csv.o
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
france_map: src/graphics/france_map.o src/station.o src/utils/parser_csv.o
	$(MAKE) station.o parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

search_button: src/graphics/search_button.o src/station.o src/utils/parser_csv.o
	$(MAKE) station.o parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

graphics: src/graphics/graphics.o src/graphics/search_button.o src/graphics/france_map.o src/station.o src/utils/parser_csv.o
	$(MAKE) station.o parser_csv.o france_map.o search_button.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


#  --- Graphics Tests ---
search_button_test: src/test/search_button_test.c src/graphics/search_button.o src/station.o src/utils/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

france_map_test: src/test/france_map_test.c src/graphics/france_map.o src/station.o src/utils/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
#   -------



clean:
	find . -name '*.o' -type f -delete
	@echo "Tout les fichiers .o ont été supprimés"
	rm -f $(ALL_EXECUTABLES)
	@echo "Tout les fichiers exécutables ont été supprimés"

.SILENT: clean test


