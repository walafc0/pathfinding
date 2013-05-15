# ./Makefile

CC      = gcc
CFLAGS  = -std=c99 -Wall -Werror -pedantic -I$(INC)
LDFLAGS = `sdl-config --libs --cflags`

INC = ./inc
SRC = ./src
OBJ = ./obj
BIN = ./bin

all: dirs target

DIRS = $(OBJ) $(BIN)
dirs: | $(DIRS)
$(DIRS):
	mkdir -p $(DIRS)

TRGT =	$(BIN)/astar
OBJS =	$(OBJ)/setObstacle.o	\
	$(OBJ)/unsetObstacle.o	\
	$(OBJ)/update.o		\
	$(OBJ)/findPath.o	\
	$(OBJ)/nextStep.o	\
	$(OBJ)/affichage.o	\
	$(OBJ)/main.o
target: $(OBJS)
	$(CC) -o $(TRGT) $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(TRGT)
	rm -f $(OBJ)/*.o
