# ./Makefile
 
CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -I ./headers
OBJ = functions/*.o main/*.o
NAME = api
OTHER = `sdl-config --libs --cflags`
 
all:
	make -C functions
	make -C main
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(OTHER)
	make -C functions clean
	make -C main clean
 
clean:
	make -C functions clean
	make -C main clean
