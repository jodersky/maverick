TARGET=maverick

CC=gcc
CFLAGS=-std=gnu99 -Wall -O2

LD=gcc
LDFLAGS=-O2

SOURCEDIRS=src
INCLUDEDIRS=\
 include\
 ext/mavlink/include

SOURCES=$(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.c))
OBJECTS=$(addsuffix .o, $(basename $(SOURCES)))


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ `sdl2-config --libs`

#$(TARGET)-static: $(OBJECTS)
#	$(LD) $(LDFLAGS) -o $@ $^ `sdl2-config --static-libs`

%.o: %.c
	$(CC) $(CFLAGS) `sdl2-config --cflags` $(addprefix -I, $(INCLUDEDIRS)) -o $@ -c $<

clean:
	@for dir in $(SOURCEDIRS); do \
		rm -f $$dir/*.o; \
	done
	rm -f $(TARGET)
	rm -f $(TARGET)-static