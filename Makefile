all:
	gcc -I src/include -L src/lib -o main main.c Menu.c Grafica.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf