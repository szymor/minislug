# Makefile

TARGET = minislug 
OBJECTS = main.o anims.o animspr.o blkanim.o blkbkg.o boss.o dust.o fire.o font.o frame.o game.o gif.o interface.o loader.o menu.o monsters00.o monsters10.o monsters20.o monsters30.o monsters40.o monsters50.o mst.o preca.o psd.o scroll.o sfx.o sprites.o sprcache.o sprrz.o transit2d.o 

CFLAGS = -m32 -O3 -Wall -s -I/usr/include/SDL -L/usr/lib -L. -fno-strict-aliasing -DNDEBUG
LIBS = -lSDL -s ymlib/libymlib.a

CC = gcc
LINKER = g++

all: $(TARGET)

$(TARGET): $(OBJECTS) ymlib/libymlib.a
	$(LINKER) $(CFLAGS) -o $@ $^ $(LIBS) 

.cc.o:
	$(CC) $(CFLAGS) -o $< 

clean:
	-rm -f $(TARGET) $(OBJECTS)
	-make -C ymlib -f Makefile_a clean

ymlib/libymlib.a:
	make -C ymlib -f Makefile_a

