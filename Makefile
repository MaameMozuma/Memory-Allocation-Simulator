CC = gcc
CFLAGS = -w
SRC = process.c utils.c memory_alloc_algo.c main.c
OBJ = $(SRC:.c=.o)
EXEC = memorySimulation

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	rm -f $(OBJ) $(EXEC)
