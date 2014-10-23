TARGET=controller

all: $(TARGET) $(TARGET)-static


$(TARGET): $(TARGET).o
	gcc -Wall -o $@ $< `sdl2-config --libs`

$(TARGET)-static: $(TARGET).o
	gcc -Wall -o $@ $< `sdl2-config --static-libs`


%.o: %.c
	gcc -Wall `sdl2-config --cflags` -o $@ -c $<

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TARGET)-static