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

day3: src/day3.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^ -lpcre

day4: src/day4.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

day5: src/day5.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

day6: src/day6.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

day7: src/day7.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^ -lgmp

day8: src/day8.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

day9: src/day9.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^ -lgmp

day10: src/day10.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

day11: src/day11.c
	$(CC) $(CFLAGS) -o build/$@ $^

day12: src/day12.c src/utils.c
	$(CC) $(CFLAGS) -o build/$@ $^

clean:
	rm -rf build/*
