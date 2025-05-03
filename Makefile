# Le compilateur à utiliser
CC = gcc

# Options de compilation de base
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lws2_32  # pour les plateformes Windows

# CFLAGS pour l'inclusion des headers
CFLAGS += -I/mingw64/include


# LDFLAGS pour la liaison des bibliothèques
LDFLAGS += -L/mingw64/lib -lssl -lcrypto  # Nous n'ajoutons plus `-lcrypt`

# Récupère les options GTK via pkg-config
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   := $(shell pkg-config --libs gtk+-3.0)

# Options de PostgreSQL
PQ_CFLAGS := -I $(shell pg_config --includedir)
PQ_LIBS := $(shell pkg-config --libs libpq)

# Options de JSON
JSON_CFLAGS := -I $(shell pkg-config --cflags libcjson)
JSON_LIBS := $(shell pkg-config --libs libcjson)

# On ajoute les bibliothèques JSON
LDFLAGS += $(JSON_LIBS)
LDFLAGS += $(PQ_LIBS) 

# Combine les options de compilation
CFLAGS += $(GTK_CFLAGS)
CFLAGS += $(PQ_CFLAGS)

# Définir les sources
SERVER_SRC = ./server/source/server.c \
./server/source/hoster.c \
./controller/source/utils.c \
./server/source/database_communication.c \
./server/source/db_connection.c \
./server/source/db_chat_content.c \
./server/source/register_user.c \
./server/source/save_message.c

CLIENT_SRC = ./client/source/client.c \
./client/source/connector.c \
./controller/source/utils.c \
./client/source/front/client_front.c \
./client/source/front/hash_login.c \
./client/source/front/login_window.c \
./client/source/front/register_window.c \
./client/source/front/session.c \
./client/source/front/chat_window.c

# Fichiers objets générés
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

# Nom des exécutables
SERVER_EXEC = server.exe
CLIENT_EXEC = client.exe

# Cible par défaut
all: $(SERVER_EXEC) $(CLIENT_EXEC)
	@if [ -f $(SERVER_EXEC) ] && [ -f $(CLIENT_EXEC) ]; then \
		echo -e "\n\n\033[1;32mCongrat'! The build is completed!\033[0m Deleting all .o files..."; \
		rm -f $(SERVER_OBJ) $(CLIENT_OBJ); \
	else \
		echo -e "\n\n\033[1;31mBuild failed. Keeping .o files for inspection.\033[0m"; \
	fi

# Création de l'exécutable serveur
$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) -o $(SERVER_EXEC) $(LDFLAGS) $(GTK_LIBS) $(PQ_LIBS)
	@if [ -f $(SERVER_EXEC) ]; then \
		echo -e "\n\n\033[0;34mBuild server OK.\033[0m\n"; \
	fi

# Création de l'exécutable client
$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) -o $(CLIENT_EXEC) $(LDFLAGS) $(GTK_LIBS) $(PQ_LIBS)
	@if [ -f $(CLIENT_EXEC) ]; then \
		echo -e "\n\n\033[0;34mBuild client OK.\033[0m\n"; \
	fi

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	@rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_EXEC) $(CLIENT_EXEC)

# Recompilation totale
rebuild: clean all

# Installer les ressources (ici CSS)
install: all
	@cp $(CSS) ./ 

# Exécution rapide
run: all
	./$(SERVER_EXEC)
