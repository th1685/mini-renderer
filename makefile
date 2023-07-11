CC= gcc
DEPS= include/graphics.h include/3dmaths.h include/dynarr.h include/datastructures.h include/texture.h include/loadobj.h
CFLAGS= -Wall -Wextra
LIBS= -lncurses -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

renderer: main.c
	$(CC) -o renderer main.c $(LIBS)
