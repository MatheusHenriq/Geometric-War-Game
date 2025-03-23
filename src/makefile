CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/Cellar/sfml/3.0.0_1/include/
LDFLAGS = -L/opt/homebrew/Cellar/sfml/3.0.0_1/lib/ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

TARGET = assignment2

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

SRC_FILES = main.cpp \
            $(LIB_DIR)/vec2/vec2.cpp \
            $(LIB_DIR)/entityManager/entity_manager.cpp \
            $(LIB_DIR)/entity/entity.cpp \
            $(LIB_DIR)/gameEngine/game_engine.cpp

OBJ_FILES = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@) # Garante que o diretório build exista
	$(CXX) $(CXXFLAGS) -c $< -o $@

xclean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)
