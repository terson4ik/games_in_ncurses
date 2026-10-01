CFLAGS   = -Wall -pedantic
LIBS     = -ncurses
OBJFILES = engine.o graphic.o main.o
EXE	     = tetris

engine.o: engine.h tetris_matrixs.h
graphic.o: graphic.h

%.o: common_structs.h
	$(CC) $(CFLAGS) -c $^ -o $@

$(EXE): $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@

run: $(EXE)
	./$(EXE)

build: $(EXE)

clean:
	rm -f *.o $(EXE)
