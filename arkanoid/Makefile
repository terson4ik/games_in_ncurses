CFLAGS = -Wall -pedantic
LIBS = -lncurses
NAME_GAME = arka
OBJMODULES = tui.o arkanoid.o main.o

tui.o: tui.h tui_conf.h arkanoid_conf.h
arkanoid.o: arkanoid.h arkanoid_conf.h

%.o: %.c default_structs.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME_GAME): $(OBJMODULES)
	$(CC) $(CFLAGS) $(OBJMODULES) $(LIBS) -o $@

run: $(NAME_GAME)
	./$(NAME_GAME)

clean:
	rm -f *.o $(NAME_GAME)