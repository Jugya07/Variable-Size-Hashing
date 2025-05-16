# Compiler to use
CXX = g++

# Detect architecture (Intel or Apple Silicon)
UNAME := $(shell uname -m)
ifeq ($(UNAME), arm64)
  QT_DIR = /opt/homebrew/opt/qt@5
else
  QT_DIR = /usr/local/opt/qt@5
endif

# Build directory
BUILD_DIR = build

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11 $(shell pkg-config --cflags Qt5Widgets)

# Linker flags
LDFLAGS = $(shell pkg-config --libs Qt5Widgets)

# Qt moc compiler
MOC = $(QT_DIR)/bin/moc

# Targets
TARGET_GUI = hash_program
TARGET_CLI = hash_cli

# Source files
SOURCES = hash_gui.cpp main_gui.cpp hash_core.cpp hash_cli.cpp seeds.cpp primes.cpp

# Object files (in build directory)
OBJECTS_GUI = $(BUILD_DIR)/hash_gui.o $(BUILD_DIR)/main_gui.o $(BUILD_DIR)/hash_core.o $(BUILD_DIR)/seeds.o $(BUILD_DIR)/primes.o $(BUILD_DIR)/moc_hash_gui.o
OBJECTS_CLI = $(BUILD_DIR)/hash_cli.o $(BUILD_DIR)/hash_core.o $(BUILD_DIR)/seeds.o $(BUILD_DIR)/primes.o

# Header files
HEADERS = hash_gui.h hash_core.h seeds.h primes.h

# Default target
all: $(BUILD_DIR) $(TARGET_GUI) $(TARGET_CLI)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link GUI executable
$(TARGET_GUI): $(OBJECTS_GUI)
	$(CXX) $(OBJECTS_GUI) -o $(TARGET_GUI) $(LDFLAGS)

# Link CLI executable
$(TARGET_CLI): $(OBJECTS_CLI)
	$(CXX) $(OBJECTS_CLI) -o $(TARGET_CLI)

# Compile source files
$(BUILD_DIR)/hash_gui.o: hash_gui.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/main_gui.o: main_gui.cpp hash_gui.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/hash_core.o: hash_core.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/hash_cli.o: hash_cli.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/seeds.o: seeds.cpp seeds.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/primes.o: primes.cpp primes.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate moc file for hash_gui.h
$(BUILD_DIR)/moc_hash_gui.cpp: hash_gui.h
	$(MOC) -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB $< -o $@

# Compile moc file
$(BUILD_DIR)/moc_hash_gui.o: $(BUILD_DIR)/moc_hash_gui.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR) $(TARGET_GUI) $(TARGET_CLI)

# Phony targets
.PHONY: all clean