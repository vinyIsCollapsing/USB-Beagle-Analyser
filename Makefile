# Variáveis do compilador e flags
CXX         := g++
CXXFLAGS    := -std=c++17 -Wall -Iinclude -Iexternal/csv-parser/single_include


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
CSV_OBJECTS := $(patsubst external/csv-parser/src/%.cpp, $(OBJ_DIR)/%.o, $(CSV_SOURCES))

# Regra padrão: compilar o executável
all: $(TARGET)

$(TARGET): $(OBJECTS) $(CSV_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regra para compilar arquivos .cpp do seu projeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para compilar arquivos .cpp da biblioteca csv-parser
$(OBJ_DIR)/%.o: external/csv-parser/src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Define as regras como "phony" para evitar conflitos com arquivos de mesmo nome
.PHONY: all clean
