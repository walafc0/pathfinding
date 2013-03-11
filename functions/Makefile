# ./functions/Makefile

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -I ../headers
OBJ = findPath.o affichage.o nextStep.o setObstacle.o unsetObstacle.o update.o

all: $(OBJ)

%o: %c
	$(CC) $(CFLAGS) -c $^ -o $@ $(OTHER)

clean: 
	rm -f *.o
