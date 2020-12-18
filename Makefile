CFLAGS = -Wall
LFLAFS = -lGL -lGLU -lglut
C_SOURCES = $(wildcard ./*c)
OBJ = $(C_SOURCES:.c=.o)
CC = gcc
P_NAME = game_of_life_c
RM = rm -rf

all: $(P_NAME)

$(P_NAME): $(OBJ)
	$(CC) -o $@ $^ $(LFLAFS) $(CFLAGS)

clean:
	@ $(RM) $(OBJ) $(P_NAME)

.PHONY: all clean
