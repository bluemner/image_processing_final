OUTPUT_DIR= bin
COMPILER = g++
FLAGS =-W -ggdb -std=c++11
SOURCE_DIR = src/
HEADERS_DIR = src/

all: driver

driver: $(SOURCE_DIR)driver.cpp | make_dir
	$(COMPILER) $(FLAGS) $(HEADERS_DIR)watershed.hpp $(HEADERS_DIR)pgm.hpp $(SOURCE_DIR)driver.cpp -o $(OUTPUT_DIR)/driver.o

make_dir:
	mkdir -p $(OUTPUT_DIR) | mkdir -p data