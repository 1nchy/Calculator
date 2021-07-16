DIR=$(shell pwd)
DIR=.
BIN_DIR=$(DIR)/bin
LIB_DIR=$(DIR)/lib
SRC_DIR=$(DIR)/src
INCLUDE_DIR=$(DIR)/include
DEPS_DIR=$(DIR)/deps
OBJ_DIR=$(DIR)/obj
PROGRAM=$(DIR)/calculator

## compile option
CC=g++
CFLAGS=-std=c++11
EXTENSION=cpp
INCLUDE=-I$(INCLUDE_DIR)

## file path
SRCS=$(wildcard $(SRC_DIR)/*.$(EXTENSION))
OBJS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o, $(SRCS))
DEPS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(DEPS_DIR)/%.d, $(SRCS))

## compile target
.PHONY: all clean rebuild

all:$(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS)

$(DEPS_DIR)/%.d: $(SRC_DIR)/%.$(EXTENSION)
	@mkdir -p $(DEPS_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MM $^ | sed 1's,^,$@ $(OBJ_DIR)/,' > $@

include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXTENSION)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

rebuild: clean all

clean:
	rm -rf $(OBJS) $(DEPS)
