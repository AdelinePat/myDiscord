# Le compilateur à utiliser
CC = gcc

# Options de compilation de base
CFLAGS = -Wall -Wextra -std=c11

# Récupère les options GTK via pkg-config
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   := $(shell pkg-config --libs gtk+-3.0)

# Nom du fichier source et de l'exécutable
SRC = example.c
EXEC = coucou.exe

# Cible par défaut
all: $(EXEC)

# Règle de compilation
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $< -o $@ $(GTK_LIBS)

# Nettoyage
clean:
	@rm -f $(EXEC)

# Exécution rapide
run: allcouc
	./$(EXEC)
