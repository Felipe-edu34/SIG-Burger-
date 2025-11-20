# Diretórios
SRC_DIR = modulos
HDR_DIR = headers
OBJ_DIR = obj
DATA_DIR = dados

# Arquivos
CFILES = $(wildcard *.c) $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(CFILES)))

all: criar_pastas compilar_gch compilar_obj link mover_dat

# 1) Criar as pastas necessárias
criar_pastas:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(DATA_DIR)

# 2) Compilar .h → .gch (pré-compilados)
compilar_gch:
	gcc -Wall -c $(HDR_DIR)/*.h

# 3) Compilar .c → .o e enviar para obj/
compilar_obj:
	gcc -Wall -c $(SRC_DIR)/*.c -I $(HDR_DIR)
	gcc -Wall -c *.c -I $(HDR_DIR)
	mv *.o $(OBJ_DIR)

# 4) Linkar os .o → sig-burger
link:
	gcc -o sig-burger $(OBJ_DIR)/*.o

# 5) Mover todos os .dat para dados/
mover_dat:
	mv -f *.dat $(DATA_DIR) 2>/dev/null || true

clean:
	rm -f $(OBJ_DIR)/*.o sig-burger $(HDR_DIR)/*.gch