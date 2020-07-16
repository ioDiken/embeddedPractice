# dataStructures Makefile
# TODO:
## g++ create output directory if DNE
## change from literal string ./tests.* to variable 

# libs
LIBS = gtest
LIBS += pthread # required for gtest

# includes
INCV = ./inc
INCV += /usr/local/include

CC = g++
CFLAGS = -Wall -O2 -std=c++11 
CFLAGS += $(foreach inc, $(INCV), -iquote $(inc))
LDFLAGS = -L /usr/local/lib
LDFLAGS += $(foreach name, $(LIBS), -l $(name))

# libs pt2
LIB_PATH = ./lib
C_LIB_SRC = $(realpath $(wildcard $(LIB_PATH)/*.c)) # match *.c
CPP_LIB_SRC = $(realpath $(wildcard $(LIB_PATH)/*.cpp)) # match *.cpp

# gtests
SRC_PATH = ./src
CPP_LIB_SRC += $(realpath $(wildcard $(SRC_PATH)/*.cpp)) # match *.cpp

# create list of objects
OBJECTS = $(foreach obj,$(basename $(notdir $(C_LIB_SRC) $(CPP_LIB_SRC))), $(BUILD_PATH)/$(obj).o)

BUILD_PATH = ./build
TARGET = ./bin/tests

.DEFAULT: all
.PHONY: all clean $(OBJECTS) $(C_LIB_SRC) $(CPP_LIB_SRC)

all: $(C_LIB_SRC) $(CPP_LIB_SRC)
	# build main test object
	$(CC) $(CFLAGS) -c ./tests.cpp

	# build main test
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) ./tests.o $(LDFLAGS)

$(C_LIB_SRC):
	$(CC) $(CFLAGS) -c $@ -o $(BUILD_PATH)/$(patsubst %.c,%.o,$(notdir $@))

$(CPP_LIB_SRC):
	$(CC) $(CFLAGS) -c $@ -o $(BUILD_PATH)/$(patsubst %.cpp,%.o,$(notdir $@))
