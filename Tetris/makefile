IN = main.c logic.c rendering.c tetrominos.c
OUT = tetris

LIBS = -lSDL2 # -fsanitize=address
WINLIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows

CC = gcc
WINCC = x86_64-w64-mingw32-gcc

# Linux:

all:
	@$(CC) $(IN) -o $(OUT) $(LIBS)

run:
	@$(CC) $(IN) -o $(OUT) $(LIBS) && ./$(OUT)

clean:
	@rm $(OUT)

# Windows:

winall:
	@$(WINCC) $(IN) -o $(OUT).exe $(WINLIBS)

winclean:
	@rm $(OUT).exe