CFLAGS = -Wall -ansi -pedantic
LIBS = -lncurses
NAME_GAME = arka
OBJMODULES = tui.o arkanoid.o main.o

tui.o: tui.h
arkanoid.o: arkanoid.h

%.o: %.c default_defs.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME_GAME): $(OBJMODULES)
	$(CC) $(CFLAGS) $(OBJMODULES) $(LIBS) -o $@

run: $(NAME_GAME)
	./$(NAME_GAME)

clean:
	rm -f *.o $(NAME_GAME)