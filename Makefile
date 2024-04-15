CC = gcc
CFLAGS = -w
SRC = process.c utils.c memory_alloc_algo.c main.c
OBJ = $(SRC:.c=.o)
EXEC = memorySimulation

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
