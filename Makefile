CC = gcc
SRC = memory_struct.c memory_allocation_algorithms.c main.c
OBJ = $(SRC:.c=.o)
EXEC = memorySimulation

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)