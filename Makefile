# Copyright 2016 Yahoo Inc.
# Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
# this is from /usr/libexec/java_home -v 1.8
#JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_72.jdk/Contents/Home
JAVA_HOME=`/usr/libexec/java_home -v 1.8`
JAVA_OS=darwin
EXT=dylib
CC=CLANG++
TARGET_ARCH=x86_64-darwin-clang
endif

ifeq ($(UNAME_S),Linux)
JAVA_HOME=/usr/java/default
JAVA_OS=linux
LINUX_ADD=amd64/
EXT=so
CC=gcc
TARGET_ARCH=x86_64-linux-gcc
endif

# include rules that can change the locations
-include custom.rules

SOURCES_DIR=src/main/native
LIBS_DIR=target/native/$(TARGET_ARCH)
OBJECTS_DIR=$(LIBS_DIR)

SOURCES=$(shell find '$(SOURCES_DIR)' -type f -name '*.cpp')
OBJECTS=$(SOURCES:$(SOURCES_DIR)/%.cpp=$(OBJECTS_DIR)/%.o)

JAVA_LIBRARY_PATH=$(JAVA_HOME)/jre/lib/$(LINUX_ADD)server/
JAVA_INCLUDES=-I$(JAVA_HOME)/include/ -I$(JAVA_HOME)/include/$(JAVA_OS)/ -L$(JAVA_LIBRARY_PATH)

CXXFLAGS=$(JAVA_INCLUDES) -I$(SOURCES_DIR)  -g -O0  -shared -fPIC
LFLAGS = -Wall -lpthread 

LIBNAME=libjni_callback.$(EXT)



$(LIBNAME): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -shared -o $(LIBS_DIR)/$@

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp
	@echo $< 
	mkdir -p $(OBJECTS_DIR)
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -rf *.$(EXT) $(OBJECTS_DIR)
