# Variáveis do compilador e flags
CXX         := g++
CXXFLAGS    := -std=c++17 -Wall -Iinclude

# Diretórios
SRC_DIR     := src
OBJ_DIR     := obj
BIN_DIR     := bin

# Nome do executável
TARGET      := $(BIN_DIR)/usb_analyzer

# Encontrar todos os arquivos fonte em src/
SOURCES     := $(wildcard $(SRC_DIR)/*.cpp)
# Gerar os nomes dos arquivos objeto correspondentes
OBJECTS     := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Regra padrão: compilar o executável
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regra para compilar arquivos .cpp para .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Define as regras como "phony" para evitar conflitos com arquivos de mesmo nome
.PHONY: all clean
