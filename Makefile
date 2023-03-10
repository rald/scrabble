scrabble: main.c common.h strutil.h scrabble.h bag.h rack.h
	gcc main.c -o scrabble -lm -g -Wall -Wextra -pedantic

clean:
	rm scrabble
