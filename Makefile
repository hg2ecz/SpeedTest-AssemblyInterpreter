CC=gcc
CFLAGS=-O2 -Wall -funroll-loops

CFLAGS+=-DDATANUM=100 -DREPEAT=1000000

LDFLAGS=-s

OBJS =fast_interpret-calltable.o fast_interpret-switch.o fast_interpret-direct.o fast_interpret-goto.o
OBJS+=fast_interpret_main.o
TARGET=fast_interpret

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
