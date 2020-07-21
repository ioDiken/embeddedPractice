# dataStructures Makefile
# TODO:
## g++ create output directory if DNE
## change from literal string ./tests.* to variable 

# libs
LIBS = gtest
LIBS += pthread # required for gtest

# avoid any issues w/ shell inheritance
SHELL = /bin/sh

# includes
INCV = ./inc
INCV += /usr/local/include

CC = g++
CFLAGS = -Wall -O2 -std=c++11 -g
CFLAGS += $(foreach inc, $(INCV), -iquote $(inc))
LDFLAGS = -L /usr/local/lib
LDFLAGS += $(foreach name, $(LIBS), -l $(name))

# libs
LIB_PATH = ./lib
CPP_LIB_SRC = $(realpath $(wildcard $(LIB_PATH)/*.cpp)) # match *.cpp

# gtest fixtures
SRC_PATH = ./src
CPP_LIB_SRC += $(realpath $(wildcard $(SRC_PATH)/*.cpp)) # match *.cpp

BUILD_PATH = ./build
TARGET = ./bin/tests
GTEST_MAIN = ./tests.cpp

# create list of objects
OBJECTS = $(foreach obj,$(basename $(notdir $(CPP_LIB_SRC))), $(BUILD_PATH)/$(obj).o)

# dirs to be created & also cleaned
CREATE_DIRS = $(dir $(TARGET))
CREATE_DIRS += $(BUILD_PATH)

.PHONY: clean $(TARGET) $(CREATE_DIRS) $(OBJECTS) $(CPP_LIB_SRC)

$(TARGET): $(CREATE_DIRS) $(CPP_LIB_SRC)
	# build main test object
	$(CC) $(CFLAGS) -c $(GTEST_MAIN) -o $(BUILD_PATH)/$(patsubst %.cpp,%.o,$(notdir $(GTEST_MAIN)))

	# build main test
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) tests.o $(LDFLAGS)

# create dirs
$(CREATE_DIRS):
	mkdir -p $@

$(CPP_LIB_SRC):
	$(CC) $(CFLAGS) -c $@ -o $(BUILD_PATH)/$(patsubst %.cpp,%.o,$(notdir $@))

clean:
	rm -r $(CREATE_DIRS)