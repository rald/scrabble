scrabble: main.c common.h ini.h irc.h strutil.h
	gcc main.c -o scrabble -lm -g -Wall -Wextra -pedantic

clean:
	rm scrabble
