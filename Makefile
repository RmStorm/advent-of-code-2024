CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g3
LDFLAGS = 

all: day1 day2

day1: src/day1.c src/utils.c
	# $@ = target day1
	# $^ = dependencies src/day1.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

day2: src/day2.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

clean:
	rm -rf build/*
