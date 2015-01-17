UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CC = gcc
endif
ifeq ($(UNAME), FreeBSD)
CC = gcc48
endif

CFLAGS  =  -std=c99 -Wall -Werror -pedantic -I$(INC)
LDFLAGS = `sdl-config --libs --cflags` -lpthread
DEFINES = -D LOCAL -D DEBUG -D DISPLAY

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
OBJS =	$(OBJ)/findpath.o	\
	$(OBJ)/nextstep.o	\
	$(OBJ)/debug.o		\
	$(OBJ)/local.o		\
	$(OBJ)/display.o	\
	$(OBJ)/main.o
target: $(OBJS)
	$(CC) -o $(TRGT) $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(DEFINES) -o $@ -c $<

clean:
	rm -f $(TRGT)
	rm -f $(OBJ)/*.o
