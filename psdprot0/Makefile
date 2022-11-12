# Mon Makefile
#PATH = /usr/include/SDL
TARGET = psdprot 
OBJECTS = main.o
 
CFLAGS = -O3 -Wall -I/usr/include/SDL -L/usr/lib -fno-strict-aliasing -D__LINUX__ 
LIBS = 
CC = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm $(TARGET) $(OBJECTS)

