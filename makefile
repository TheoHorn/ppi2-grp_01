CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS+= `pkg-config --cflags gtk+-3.0`
LDFLAGS+= -fsanitize=address
LDFLAGS+= `pkg-config --libs gtk+-3.0`

ALL_EXECUTABLES=
ALL_O=parser_csv.o station.o time_distance_calcul.o dijkstra.o search_button.o degree_of_station.o interface_graphique.o

all: create_o $(ALL_EXECUTABLES)
create_o:
	$(ALL_O)
	@echo "Tout les fichiers .o ont été créés"

# --- .o ---
parser_csv.o: src/utils/parser_csv.c src/utils/parser_csv.h
	${CC} ${CFLAGS} -c $< -o src/utils/$@

station.o : src/station.c src/station.h
	$(CC) $(CFLAGS) -c $< -o src/$@

time_distance_calcul.o: src/test/performances src/station.o

	$(CC) $(CFLAGS) -c $< -o src/test/$@

dijkstra.o: src/dijkstra.c src/station.o
	$(CC) $(CFLAGS) -c $< -o src/$@

search_button.o : src/graphics/search_button.c src/station.o
	$(CC) $(CFLAGS) -c $< -o src/graphics/$@

degree_of_station.o: src/test/performances src/station.o
	$(CC) $(CFLAGS) -c $< -o src/test/performances$@

interface_graphique.o : src/graphics/interface_graphique.c src/station.o
	$(CC) $(CFLAGS) -c $< -o src/graphics/$@
#   -----------------



# --- Tests ---
parser_csv_test: $(parser_csv.o) src/test/csv_loading_test.c src/utils/parser_csv.o
	${CC} ${CFLAGS} -o $@ $<

calculate_distance_test:  src/test/calculate_distance.c src/station.o
	${CC} ${CFLAGS} -o $@ $<

#  -----------------
#  --- Performances tests ---
time_distance_calcul: src/test/performances src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/performances/$@

#  -----------------


degree_of_station: src/test/performances src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/performances/$@

interface_graphique: src/graphics/interface_graphique.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/$@


search_button: src/graphics/search_button.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/$@

dijkstra: src/dijkstra.o src/station.o src/parser_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o src/$@



clean:
	find . -name '*.o' -type f -delete
	find . -name '$(ALL_EXECUTABLES)' -type f -delete

.SILENT: clean test


