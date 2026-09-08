CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

OBJECTS = src/main.o src/input.o src/simulation.o

all: scheduler

scheduler: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o scheduler

src/%.o: src/%.c include/scheduler.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) scheduler rate_yrn.out edf_yrn.out

test: scheduler
	sh tests/run_tests.sh

.PHONY: all clean test
