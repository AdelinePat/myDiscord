CC = gcc
CFLAGS = -Wall -Wextra -g
THREAD = -lpthread

# Dossiers
SRC_DIR = lib

# Sources
SRV_SRC = $(SRC_DIR)/server.c
CLI_SRC = $(SRC_DIR)/client.c

# Exécutables
SRV_EXEC = server.exe
CLI_EXEC = client.exe

all: $(SRV_EXEC) $(CLI_EXEC)

$(SRV_EXEC): $(SRV_SRC)
	$(CC) $(CFLAGS) -o $(SRV_EXEC) $(SRV_SRC) $(THREAD)

$(CLI_EXEC): $(CLI_SRC)
	$(CC) $(CFLAGS) -o $(CLI_EXEC) $(CLI_SRC)

clean:
	del $(SRV_EXEC) $(CLI_EXEC) 2>nul || echo "Déjà clean hihi"

fclean: clean

re: fclean all