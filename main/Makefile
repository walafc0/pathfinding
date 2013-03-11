# ./main/Makefile

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -I ../headers
OBJ = main.o

all: $(OBJ)

%o: %c
	$(CC) $(CFLAGS) -c $^ -o $@ $(OTHER)

clean:
	rm -f *.o
