CLEANCMD=rm -rf fuzz fuzz.exp fuzz.lib fuzz.pdb fuzz-abc abc-rt.o.o fuzz.o main.o

OBJS=fuzz.c api.c
CFLAGS=-g -O0 -fsanitize=address -fno-omit-frame-pointer -fprofile-arcs -ftest-coverage

all: fuzz

fuzz: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@


.PHONY: clean

clean:
	$(CLEANCMD)
