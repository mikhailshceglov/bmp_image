CC = gcc
CFLAGS = -Wall -Iinclude
SRC = main.c lib/bmp.c lib/drawing.c lib/operations.c lib/utils.c lib/options.c lib/defence.c
OBJ = $(SRC:.c=.o)
TARGET = cw

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
