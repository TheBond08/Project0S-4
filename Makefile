cc = gcc
CFLAGS = -Wall -Wextra

ALL = main.c processes.c

OBJ = $(ALL:.c=.o)

TARGET = program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)