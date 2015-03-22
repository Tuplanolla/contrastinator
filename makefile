CC=gcc
CFLAGS=`pkg-config --cflags cairo gtk+-2.0` -D_GNU_SOURCE -Wall -Wextra
LDLIBS=`pkg-config --libs cairo gtk+-2.0` -ldl

all: contrastinator.so

clean:
	$(RM) contrastinator.so

run: all paper.pdf
	LD_PRELOAD=./contrastinator.so evince paper.pdf

%.so: %.c
	$(CC) $(CFLAGS) -fpic -o $@ -shared $< $(LDLIBS)

.PHONY: all clean run
