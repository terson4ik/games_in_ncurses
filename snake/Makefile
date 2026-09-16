OBJMODULES = snake.o tui.o main.o
CFLAGS = -Wall -ansi -pedantic 
LIBS = -lncurses
GAME_NAME = snake

snake.o: snake.h
tui.o: tui.h

%.o: %.c common_types.h
	$(CC) $(CFLAGS) -c $< $(LIBS) -o $@

$(GAME_NAME): $(OBJMODULES)
	$(CC) $(CFLAGS) $(OBJMODULES) $(LIBS) -o $@

run: $(GAME_NAME)
	./$(GAME_NAME)

clean:
	rm -f *.o $(GAME_NAME)
