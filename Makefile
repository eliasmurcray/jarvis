CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -lcurl
SRC_FILES = $(wildcard src/*.cpp)
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_NAME = main

all:
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)
