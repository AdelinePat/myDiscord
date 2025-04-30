# Le compilateur à utiliser
CC = gcc

# Options de compilation de base
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lws2_32

# Récupère les options GTK via pkg-config
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   := $(shell pkg-config --libs gtk+-3.0)

PQ_CFLAGS := -I $(shell pg_config --includedir)
PQ_LIBS := $(shell pkg-config --libs libpq)

JSON_CFLAGS := -I $(shell pkg-config --cflags libcjson)
JSON_LIBS := $(shell pkg-config --libs libcjson)
LDFLAGS += $(JSON_LIBS)

# Combine les options de compilation
CFLAGS += $(GTK_CFLAGS)
CFLAGS += $(PQ_CFLAGS)

# préciser les dossiers utilisés
# SERVER_DIR = ./server 
# CLIENT_DIR = ./client

SERVER_SRC = ./server/source/server.c ./server/source/hoster.c ./controller/source/utils.c
CLIENT_SRC = ./client/source/client.c ./client/source/connector.c ./controller/source/utils.c ./client/source/front/client_front.c ./client/source/front/login_window.c ./client/source/front/register_window.c ./client/source/front/chat_window.c 

# SERVER_OBJ = $(SERVER_DIR)/server.o
# Fichiers sources
# SRC = ./source/register_window.c ./source/login_window.c ./source/chat_window.c ./client_main.c

# Fichiers objets générés
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
# OBJ = $(SRC:.c=.o)

# Nom de l'exécutable
SERVER_EXEC = server.exe
CLIENT_EXEC = client.exe

# Feuilles de style (copiées à l'installation)
CSS = ./client/source/front/style.css

# Cible par défaut
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Création de l'exécutable serveur
$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) -o $(SERVER_EXEC) $(LDFLAGS) $(GTK_LIBS) $(PQ_LIBS)

# Création de l'exécutable client
$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) -o $(CLIENT_EXEC) $(LDFLAGS) $(GTK_LIBS) $(PQ_LIBS)

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	@rm -f $(SERVER_OBJ) $(SERVER_EXEC)
	@rm -f $(CLIENT_OBJ) $(CLIENT_EXEC)

# Recompilation totale
rebuild: clean all

# Installer les ressources (ici CSS)
install: all
	@cp $(CSS) ./ 

# Exécution rapide
run: all
	./$(SERVER_EXEC)
	
