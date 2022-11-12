# Makefile

TARGET = minislug 
OBJECTS = main.o anims.o animspr.o blkanim.o blkbkg.o boss.o dust.o fire.o font.o frame.o game.o gif.o interface.o loader.o menu.o monsters00.o monsters10.o monsters20.o monsters30.o monsters40.o monsters50.o mst.o preca.o psd.o scroll.o sfx.o sprites.o sprcache.o sprrz.o transit2d.o 

CFLAGS = -O3 -Wall -s -I/usr/include/SDL -L/usr/lib -L. -fno-strict-aliasing -DNDEBUG 
LIBS = -lSDL -s libymlib.a
#LIBS = -lSDL
#LIBS = -lSDL -lSDL_image

CC = gcc
LINKER = g++

all: $(TARGET)

#$(TARGET): $(OBJECTS)
#	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 

$(TARGET): $(OBJECTS)
	$(LINKER) $(CFLAGS) -o $@ $^ $(LIBS) 

.cc.o:
	$(CC) $(CFLAGS) -o $< 

clean:
	rm $(TARGET) $(OBJECTS)

