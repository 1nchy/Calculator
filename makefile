DIR=$(shell pwd)
DIR=.
BIN_DIR=$(DIR)/bin
LIB_DIR=$(DIR)/lib
SRC_DIR=$(DIR)/src
INCLUDE_DIR=$(DIR)/include
DEPS_DIR=$(DIR)/deps
OBJ_DIR=$(DIR)/obj
PROGRAM=$(DIR)/calculator

## obj file path
EXTENSION=cpp
SRCS=$(wildcard $(SRC_DIR)/*.$(EXTENSION))
OBJS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o, $(SRCS))
DEPS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(DEPS_DIR)/%.d, $(SRCS))

## include file path
INCLUDE=\
		-I$(INCLUDE_DIR)

## compile option
CC=g++
CFLAGS=-std=c++11

## compile target
.PHONY: all clean rebuild

all:$(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS)

$(DEPS_DIR)/%.d: $(SRC_DIR)/%.$(EXTENSION)
	$(CC) $(CFLAGS) -MM $^ | sed 's,^,$@ $(OBJ_DIR)/,' > $@

include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXTENSION)
	$(CC) $(CFLAGS) -o $@ -c $<

rebuild: clean all

clean:
	rm -rf $(OBJS) $(DEPS)
