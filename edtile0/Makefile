# Makefile
TARGET = EdTile1 
OBJECTS = main.o font.o infobox.o keyboard.o mouse.o plane.o psd.o map.o cmd.o file.o visu.o undo.o

#CFLAGS = -O3 -Wall -Wno-write-strings -g -I/usr/include/SDL -L/usr/lib		# debug
CFLAGS = -O3 -Wall -Wno-write-strings -s -I/usr/include/SDL -L/usr/lib  
LIBS = -lSDL
CPP = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(CFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm $(TARGET) $(OBJECTS)

