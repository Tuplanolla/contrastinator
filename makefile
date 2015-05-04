CC=gcc
GTK=gtk+-3.0
CFLAGS=`pkg-config --cflags cairo $(GTK)` -D_GNU_SOURCE -Wall -Wextra
LDLIBS=`pkg-config --libs cairo $(GTK)` -ldl

all: contrastinator.so

clean:
	$(RM) contrastinator.so

help:
	pkg-config --exists gtk+-3.0 && echo gtk+-3.0 || \
		pkg-config --exists gtk+-2.0 && echo gtk+-2.0

run: all paper.pdf
	LD_PRELOAD=./contrastinator.so evince paper.pdf

%.so: %.c
	$(CC) $(CFLAGS) -fpic -o $@ -shared $< $(LDLIBS)

.PHONY: all clean help run
