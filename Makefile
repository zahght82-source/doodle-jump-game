# =====================
# SFML location -- installed via MSYS2 UCRT64 (pacman -S mingw-w64-ucrt-x86_64-sfml)
# =====================
SFML_DIR = C:/msys64/ucrt64

# =====================
# Directories
# =====================
SRC_DIR = src
BUILD_DIR = build
MY_INCLUDE_DIR = include

DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release

# =====================
# Compiler
# =====================
# IMPORTANT: must be the UCRT64 g++ that matches the SFML build above,
# not any other MinGW installation on this machine (e.g. mingw13 or the
# 8.1.0 toolchain) -- mismatched compilers cause link errors.
CXX = C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS = -std=c++17 -Wall -MMD -MP
INCFLAGS = -I$(SFML_DIR)/include -I$(MY_INCLUDE_DIR)
LIBFLAGS = -L$(SFML_DIR)/lib -lsfml-graphics -lsfml-window -lsfml-system

# =====================
# File Names (recursive: picks up src/**/*.cpp, not just src/*.cpp)
# =====================
SRC = $(shell find $(SRC_DIR) -name '*.cpp')
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRC))
RELEASE = $(RELEASE_DIR)/game
DEBUG = $(DEBUG_DIR)/game_debug

# =====================
# Make rules
# =====================
all: release

release: $(RELEASE)

debug: $(DEBUG)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

-include $(DEP)

$(RELEASE): $(OBJ)
	@mkdir -p $(RELEASE_DIR)
	$(CXX) $(CXXFLAGS) -O2 -DNDEBUG $^ -o $@ $(LIBFLAGS)
	@cp $(SFML_DIR)/bin/*.dll $(RELEASE_DIR)/ 2>/dev/null || true
	@cp -r assets $(RELEASE_DIR)/
	@cp -r fonts $(RELEASE_DIR)/

$(DEBUG): $(OBJ)
	@mkdir -p $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LIBFLAGS)
	@cp $(SFML_DIR)/bin/*.dll $(DEBUG_DIR)/ 2>/dev/null || true
	@cp -r assets $(DEBUG_DIR)/
	@cp -r fonts $(DEBUG_DIR)/

run: release
	./$(RELEASE)

clean:
	rm -rf $(BUILD_DIR) $(RELEASE_DIR) $(DEBUG_DIR)

.PHONY: all release debug run clean
