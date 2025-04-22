# Le compilateur à utiliser
CC = gcc

# Options de compilation de base
CFLAGS = -Wall -Wextra -std=c11

# Récupère les options GTK via pkg-config
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   := $(shell pkg-config --libs gtk+-3.0)

# Combine les options de compilation
CFLAGS += $(GTK_CFLAGS)

# Fichiers sources
SRC = login_window.c chat_window.c main.c

# Fichiers objets générés
OBJ = $(SRC:.c=.o)

# Nom de l'exécutable
EXEC = main.exe

# Feuilles de style (copiées à l'installation)
CSS = style.css

# Cible par défaut
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(GTK_LIBS)

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	@rm -f $(EXEC) *.o

# Recompilation totale
rebuild: clean all

# Installer les ressources (ici CSS)
install: all
	@cp $(CSS) ./

# Exécution rapide
run: all
	./$(EXEC)
