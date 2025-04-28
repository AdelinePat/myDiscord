# Le compilateur à utiliser
CC = gcc

# Options de compilation de base
CFLAGS = -Wall -Wextra -std=c11

# Récupère les options GTK via pkg-config
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   := $(shell pkg-config --libs gtk+-3.0)

# Récupère les options PostgreSQL
PQ_CFLAGS := -I$(shell pg_config --includedir)
PQ_LIBS := $(shell pkg-config --libs libpq)

# Combine toutes les options de compilation
CFLAGS += $(GTK_CFLAGS) $(PQ_CFLAGS)

# Fichiers sources du client principal
CLIENT_SRC = ./source/register_window.c ./source/login_window.c ./source/chat_window.c ./client_main.c

# Fichiers spécifiques serveur et clients secondaires
SERVER_SRC = ./server.c
CLIENT1_SRC = ./client_1.c
CLIENT2_SRC = ./client_2.c

# Fichiers objets
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT1_OBJ = $(CLIENT1_SRC:.c=.o)
CLIENT2_OBJ = $(CLIENT2_SRC:.c=.o)

# Exécutables
CLIENT_EXEC = client_main.exe
SERVER_EXEC = server.exe
CLIENT1_EXEC = client1.exe
CLIENT2_EXEC = client2.exe

# Feuilles de style (copiées à l'installation)
CSS = ./style.css

# Cible par défaut
all: $(CLIENT_EXEC) $(SERVER_EXEC) $(CLIENT1_EXEC) $(CLIENT2_EXEC)

# Compilation des exécutables
$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $^ -o $@ $(GTK_LIBS) $(PQ_LIBS)

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $^ -o $@

$(CLIENT1_EXEC): $(CLIENT1_OBJ)
	$(CC) $^ -o $@

$(CLIENT2_EXEC): $(CLIENT2_OBJ)
	$(CC) $^ -o $@

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	@rm -f $(CLIENT_EXEC) $(SERVER_EXEC) $(CLIENT1_EXEC) $(CLIENT2_EXEC) source/*.o *.o

# Recompilation totale
rebuild: clean all

# Installer les ressources (ici CSS)
install: all
	@cp $(CSS) ./

# Exécution rapide du client principal
run_client:
	./$(CLIENT_EXEC)

run_server:
	./$(SERVER_EXEC)

run_client1:
	./$(CLIENT1_EXEC)

run_client2:
	./$(CLIENT2_EXEC)

