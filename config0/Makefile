# Mon Makefile
#PATH = /usr/include/SDL
TARGET = config 
OBJECTS = main.o font.o 

CFLAGS = -O3 -Wall -g -I/usr/include/SDL -L/usr/lib -fno-strict-aliasing -s 
LIBS = -lSDL
#LIBS = -lSDL -lSDL_image
CC = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm $(TARGET) $(OBJECTS)

