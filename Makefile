###############################################################################
# Tyler Perkins
# 7-21-21
# Makefile
#

CC = g++

FLAGS  = -pipe

CFLAGS  = -Wall
CFLAGS += -Ofast
CFLAGS += -std=c++17
#CFLAGS += -g
#CFLAGS += -pg

#required for rss-cli
LIBRARIES = -lcurl
#required for sdl2
LIBRARIES += `pkg-config --cflags --libs sdl2`
LIBRARIES += `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf

SRC = $(shell find ./src -name '*.cpp')
OBJ = $(subst .cpp,.o,$(SRC))
BIN = ./bin

PREFIX    = /usr/local
MANPREFIX = $(PREFIX)/share/man
DATA      = /usr/share/dashboard
DATA_IMG  = $(DATA)/img
DATA_FONT = $(DATA)/font

TARGET = dashboard.out

MAKEFLAGS += --jobs=4

DEFINITIONS  = -DDATA='"$(DATA)"'
DEFINITIONS += -DDATA_IMG='"$(DATA_IMG)"'
DEFINITIONS += -DDATA_FONT='"$(DATA_FONT)"'

all : $(OBJ)
	@echo LD $@
	@$(CC) $(FLAGS) $(CFLAGS) -o $(BIN)/$(TARGET) $(OBJ) $(LIBRARIES)

.cpp.o :
	@echo CC $<
	@$(CC) $(FLAGS) $(CFLAGS) $(LIBRARIES) $(DEFINITIONS) -c $< -o $@

src/config.hpp :
	cp src/config.def.hpp src/config.hpp


$(OBJ): src/config.hpp

install : all
	mkdir -p $(PREFIX)/bin
	cp -f $(BIN)/$(TARGET) $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/$(TARGET)
	#install resources
	mkdir -p $(DATA) $(DATA_IMG) $(DATA_FONT)
	cp -fr ./img  $(DATA)
	cp -fr ./font $(DATA)
	chmod -R 745  $(DATA)

uninstall :
	rm -rf $(PREFIX)/bin/$(TARGET)
	rm -rf $(DATA)

clean :
	find . -type f -name '*.o' -delete
	rm -rf $(BIN)/*

clean-config :
	rm -rf src/config.hpp
