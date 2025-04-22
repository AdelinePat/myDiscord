# Makefile pour compiler un projet C avec GTK+3 sous Windows
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
# Use pkg-config to get the correct GTK+ flags
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS := $(shell pkg-config --libs gtk+-3.0)


SRC = example.c
OBJ = $(SRC:.c=.o)
EXEC = example.exe

CFLAGS += $(GTK_CFLAGS)
LDFLAGS = -mconsole 
# LDFLAGS = -mconsole -Wl,-e,mainCRTStartup
all: $(EXEC)
# Rule to link object files into an executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS) $(GTK_LIBS)
# Rule to compile the source into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
# Clean up object files and the executable
clean:
	rm -f *.o $(EXEC)
# Rebuild the project by cleaning and then building
rebuild: clean all
