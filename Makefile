# Le compilateur à utiliser
CC = gcc

# Options de compilation de base
CFLAGS = -Wall -Wextra -std=c11

# Récupère les options GTK via pkg-config
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   := $(shell pkg-config --libs gtk+-3.0)

# Combine les options de compilation (on ajoute GTK_CFLAGS aux CFLAGS)
CFLAGS += $(GTK_CFLAGS)

# Fichiers sources
SRC = example.c 

# Fichiers objets générés à partir des sources
OBJ = $(SRC:.c=.o)

# Nom de l'exécutable
EXEC = example

# Cible par défaut appelée avec juste "make"
all: $(EXEC)

# Règle pour créer l'exécutable en liant avec GTK_LIBS
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(GTK_LIBS)

# Règle générique pour compiler les .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour nettoyer les fichiers temporaires
clean:
	del /Q *.o *.exe 2>nul || rm -f *.o $(EXEC)

# Pour forcer la recompilation de tout
rebuild: clean all
