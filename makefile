CC=gcc
CFLAGS=`pkg-config --cflags --libs cairo` -D_GNU_SOURCE -Wall -Wextra
LDLIBS=-lcairo -ldl

all: contrastinator.so

clean:
	$(RM) contrastinator.so

run: all paper.pdf
	LD_PRELOAD=./contrastinator.so evince paper.pdf

%.so: %.c
	$(CC) $(CFLAGS) -fpic -o $@ -shared $< $(LDLIBS)

.PHONY: all clean run
