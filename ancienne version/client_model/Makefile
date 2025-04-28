# Le compilateur à utiliser
CC = gcc

# Options de compilation de base
CFLAGS = -Wall -Wextra -std=c11

# 💡 Add this line:
LDFLAGS = -lws2_32

# Fichiers sources
SRC = ./client_connection.c

# Fichiers objets générés
OBJ = $(SRC:.c=.o)

# Nom de l'exécutable
EXEC = client_main.exe

# Cible par défaut
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	@rm -f $(EXEC) $(OBJ)

# Recompilation totale
rebuild: clean all

# Exécution rapide
run: all
	./$(EXEC)
