# Makefile for test program for linux
# note - requires GNU gcc/g++ 4.9 or later due to <regex> unimplemented before that
CC = g++

# Make sure to remove -g when not debugging!
CFLAGS = -g -std=c++17

INCDIRS = -I/usr/include/SDL2

BIN = main
OBJS = build/main.o build/Sprite.o build/Display.o build/Setting.o build/Element.o build/SettingHandlers.o build/HexColorParser.o \
       build/FontHandler.o build/GuisanWindow.o build/ActionEventHandlers.o \
	   build/ApplySettingsHandlers.o build/WriteTheme.o

# Note - the order here matters - different order may result in errors!
#-lpthread -lstdc++fs
LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lguisan -lstdc++fs

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(INCDIRS) $(LIBDIRS) $(LIBS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCDIRS)
    
clean:
	rm -rf build/*
