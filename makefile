# Nom de l'exécutable
TARGET = metro

# Dossiers
SRC_DIR = src
OBJ_DIR = obj

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -I$(SRC_DIR)

# Liste des sources (trouve tous les .c dans src/)
SRC = $(wildcard $(SRC_DIR)/*.c)

# Liste des objets (place les .o dans le dossier obj/)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Règle par défaut
all: $(OBJ_DIR) $(TARGET)

# Création du dossier obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Liaison de l'exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des fichiers .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

re: clean all

.PHONY: all clean re