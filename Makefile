CC=arm-linux-gnueabihf-gcc
CFLAGS=-Wall -g -std=gnu99

EXECUTABLE = main
all:  main

main: test.c gpio.c gpio.h
	$(CC) $(CFLAGS) -o test test.c gpio.c


.PHONY: clean

clean:
	find . -type f | xargs -n 5 touch
	rm -f *.o *~ $(EXECUTABLE)
