CC=gcc
CFLAGS=-O2 -Wall -funroll-loops

CFLAGS+=-DDATANUM=100 -DREPEAT=1000000

LDFLAGS=-s

OBJS =fast_interpret-calltable.o fast_interpret-switch.o fast_interpret-direct.o
OBJS+=fast_interpret-goto.o fast_interpret-goto_jit.o fast_interpret-goto_jit2.o
OBJS+=fast_interpret-goto_jit3.o fast_interpret-goto_jit4.o
OBJS+=fast_interpret_main.o
TARGET=fast_interpret

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
