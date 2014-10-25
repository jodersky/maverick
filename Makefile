TARGET=controller

CC=gcc
CFLAGS=-std=gnu99 -Wall -O2

LD=gcc
LDFLAGS=-O2

SOURCES=$(wildcard *.c)
OBJECTS=$(addsuffix .o, $(basename $(SOURCES)))


all: $(TARGET) $(TARGET)-static

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ `sdl2-config --libs`

$(TARGET)-static: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ `sdl2-config --static-libs`

%.o: %.c
	$(CC) $(CFLAGS) `sdl2-config --cflags` -o $@ -c $<

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TARGET)-static