# Compiler to use
CXX = g++

# Detect architecture (Intel or Apple Silicon)
UNAME := $(shell uname -m)
ifeq ($(UNAME), arm64)
  OPENSSL_DIR = /opt/homebrew/opt/openssl@3
  QT_DIR = /opt/homebrew/opt/qt@5
else
  OPENSSL_DIR = /usr/local/opt/openssl@3
  QT_DIR = /usr/local/opt/qt@5
endif

# Build directory
BUILD_DIR = build

# Compiler flags for GUI
CXXFLAGS = -Wall -Wextra -std=c++11 -I$(OPENSSL_DIR)/include $(shell pkg-config --cflags Qt5Widgets)

# Compiler flags for CLI (no Qt)
CXXFLAGS_CLI = -Wall -Wextra -std=c++11 -I$(OPENSSL_DIR)/include

# Linker flags for GUI
LDFLAGS = -L$(OPENSSL_DIR)/lib -lcrypto $(shell pkg-config --libs Qt5Widgets)

# Linker flags for CLI
LDFLAGS_CLI = -L$(OPENSSL_DIR)/lib -lcrypto

# Qt moc compiler
MOC = $(QT_DIR)/bin/moc

# Targets
TARGET_GUI = hash_program
TARGET_CLI = hash_cli

# Source files
SOURCES_GUI = hash_gui.cpp main_gui.cpp hash_core.cpp seeds.cpp
SOURCES_CLI = hash_cli.cpp hash_core.cpp seeds.cpp

# Object files (in build directory)
OBJECTS_GUI = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES_GUI)) $(BUILD_DIR)/moc_hash_gui.o
OBJECTS_CLI = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES_CLI))

# Header files
HEADERS = hash_gui.h hash_core.h seeds.h

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
	$(CXX) $(OBJECTS_CLI) -o $(TARGET_CLI) $(LDFLAGS_CLI)

# Compile source files for GUI
$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files for CLI
$(BUILD_DIR)/hash_cli.o: hash_cli.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS_CLI) -c $< -o $@
$(BUILD_DIR)/hash_core.o: hash_core.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS_CLI) -c $< -o $@
$(BUILD_DIR)/seeds.o: seeds.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS_CLI) -c $< -o $@

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