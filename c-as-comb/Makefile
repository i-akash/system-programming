OBJECTS = func.o main.o
CC = gcc
CFLAGS = -std=c11  -Wall -Wextra -Werror -c
AS = nasm
ASFLAGS = -f elf64

all: $(OBJECTS)
	gcc  $(OBJECTS) -o main

run: all
	./main

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) main
