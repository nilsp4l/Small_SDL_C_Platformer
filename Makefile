CFLAGS= -Wall -Wextra -Wpedantic -std=gnu17 -O3
SDL_FLAGS = -lSDL2_image -lSDL2 -lSDL2main
all: main.o

main.o: src/main.c src/Game.c src/Gameboard.c src/Control.c src/Player.c src/Movement.c src/Enemy.c src/Clock.c src/LevelFactory.c src/Utility.c
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_FLAGS)

clean:
	rm -f main.o
