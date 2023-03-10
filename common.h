#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define GAME_TITLE "SCRABBLE"
#define SCORE_FILE "score.txt"
#define NUM_LETTERS 27
#define BOARD_WIDTH 15
#define BOARD_HEIGHT 15
#define STRING_MAX 512



const char *mst = "siesta";
const char *hst = "irc.undernet.org";
const char *prt = "6667";
const char *nck = "siestu";
const char *chn = "#pantasya";
const char *pss = NULL;



char letters[NUM_LETTERS]={
  'a','b','c','d','e','f','g','h','i',
  'j','k','l','m','n','o','p','q','r',
  's','t','u','v','w','x','y','z','.'
};

int points[NUM_LETTERS]={
  1,3,3,2,1,4,2,4,1,
  8,5,1,3,1,1,3,10,1,
  1,1,1,4,4,8,4,10,0
};

int pieces[NUM_LETTERS]={
  9,2,2,4,12,2,3,2,9,
  1,1,4,2,6,8,2,1,6,
  4,6,4,2,2,1,2,1,2
};

int bonus[BOARD_HEIGHT][BOARD_WIDTH]={
  {4,0,0,1,0,0,0,4,0,0,0,1,0,0,4},
  {0,3,0,0,0,2,0,0,0,2,0,0,0,3,0},
  {0,0,3,0,0,0,1,0,1,0,0,0,3,0,0},
  {1,0,0,3,0,0,0,1,0,0,0,3,0,0,1},
  {0,0,0,0,3,0,0,0,0,0,3,0,0,0,0},
  {0,2,0,0,0,2,0,0,0,2,0,0,0,2,0},
  {0,0,1,0,0,0,1,0,1,0,0,0,1,0,0},
  {4,0,0,1,0,0,0,5,0,0,0,1,0,0,4},
  {0,0,1,0,0,0,1,0,1,0,0,0,1,0,0},
  {0,2,0,0,0,2,0,0,0,2,0,0,0,2,0},
  {0,0,0,0,3,0,0,0,0,0,3,0,0,0,0},
  {1,0,0,3,0,0,0,1,0,0,0,3,0,0,1},
  {0,0,3,0,0,0,1,0,1,0,0,0,3,0,0},
  {0,3,0,0,0,2,0,0,0,2,0,0,0,3,0},
  {4,0,0,1,0,0,0,4,0,0,0,1,0,0,4}
};

int board[BOARD_HEIGHT][BOARD_WIDTH]={0};


#endif
