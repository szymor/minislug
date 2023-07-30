# Makefile

TARGET = minislug

CC ?= gcc
CXX ?= g++
SYSROOT ?= $(shell $(CC) --print-sysroot)
PKGCONF ?= $(SYSROOT)/../../bin/pkg-config

OBJECTS = main.o anims.o animspr.o blkanim.o blkbkg.o boss.o dust.o fire.o font.o frame.o game.o gif.o interface.o loader.o menu.o monsters00.o monsters10.o monsters20.o monsters30.o monsters40.o monsters50.o mst.o preca.o psd.o scroll.o sfx.o sprites.o sprcache.o sprrz.o transit2d.o

CFLAGS = -O3 -Wall -s `$(PKGCONF) --cflags sdl` -L. -fno-strict-aliasing -DNDEBUG
LIBS = `$(PKGCONF) --libs sdl` -s ymlib/libymlib.a

all: $(TARGET)

$(TARGET): $(OBJECTS) ymlib/libymlib.a
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

.cc.o:
	$(CC) $(CFLAGS) -o $< 

clean:
	-rm -f $(TARGET) $(OBJECTS)
	-make -C ymlib -f Makefile_a clean

ymlib/libymlib.a:
	make CC=$(CC) CXX=$(CXX) -C ymlib -f Makefile_a

