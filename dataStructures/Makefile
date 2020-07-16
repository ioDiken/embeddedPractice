# DataStructures Makefile

LIBS = gtest
LIBS += pthread # required for gtest

CC = g++
CFLAGS = -Wall -O2
LDFLAGS = -L /usr/local/lib
LDFLAGS += $(foreach name, $(LIBS), -l $(name))

# includes
INCV = ./inc
INCV += /usr/local/include
CFLAGS += $(foreach inc, $(INCV), -iquote $(inc))

# libs
LIB_PATH = ./lib
C_LIB_SRC = $(realpath $(wildcard $(LIB_PATH)/*.c)) # match *.c
CPP_LIB_SRC = $(realpath $(wildcard $(LIB_PATH)/*.cpp)) # match *.cpp

# gtests
SRC_PATH = ./src
CPP_LIB_SRC += $(realpath $(wildcard $(SRC_PATH)/*.cpp)) # match *.cpp

# add to objects
OBJECTS = $(foreach obj,$(basename $(notdir $(C_LIB_SRC) $(CPP_LIB_SRC))), $(BUILD_PATH)/$(obj).o)
# OBJECTS = $(foreach obj,$(basename $(notdir C_LIB_SRC)),$(BUILD_PATH)/$(obj))

BUILD_PATH = ./build
TARGET = ./bin/tests

.DEFAULT: all
.PHONY: all clean $(OBJECTS) $(C_LIB_SRC) $(CPP_LIB_SRC)

all: FUCK $(C_LIB_SRC) $(CPP_LIB_SRC)
	# build main test object
	$(CC) $(CFLAGS) -c ./tests.cpp

	# build main test
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) ./tests.o $(LDFLAGS)

FUCK:
	@echo "C Sources" $(C_LIB_SRC)
	@echo "CPP Sources" $(CPP_LIB_SRC)

$(C_LIB_SRC):
	$(CC) -c $(CFLAGS) $@ -o $(BUILD_PATH)/$(patsubst %.c,%.o,$(notdir $@))

$(CPP_LIB_SRC):
	$(CC) -c $(CFLAGS) $@ -o $(BUILD_PATH)/$(patsubst %.cpp,%.o,$(notdir $@))
