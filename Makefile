# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

# Directorios
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
REPORTES_DIR = reportes

# Archivos fuente
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Avion.cpp \
          $(SRC_DIR)/Piloto.cpp \
          $(SRC_DIR)/ArbolB.cpp \
          $(SRC_DIR)/ArbolBST.cpp \
          $(SRC_DIR)/TablaHash.cpp \
          $(SRC_DIR)/Grafo.cpp \
          $(SRC_DIR)/MatrizDispersa.cpp \
          $(SRC_DIR)/ListaCircular.cpp

# Archivos objeto
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Ejecutable
TARGET = aeropuerto

# Regla principal
all: directories $(TARGET)

# Crear directorios necesarios
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(REPORTES_DIR)

# Compilar ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilacion exitosa: ./$(TARGET)"

# Compilar archivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Archivos de compilacion eliminados"

# Limpiar todo incluyendo reportes
cleanall: clean
	rm -rf $(REPORTES_DIR)
	@echo "Todo limpio"

# Ejecutar el programa
run: all
	./$(TARGET)

.PHONY: all clean cleanall run directories