CFLAGS = -Wall -pedantic
LIBS = -lncurses
OBJFILES = main.o cars_engine.o graphics_render.o
GAME_NAME = cars

%.o: %.c common_structs.h cars_settings.h
	$(CC) $(CFLAGS) -c $< -o $@

$(GAME_NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

run: $(GAME_NAME)
	./$(GAME_NAME)

clear:
	rm -f *.o $(GAME_NAME) 