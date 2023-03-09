// Ed's Scrabble AI  started May 23, 2001
// engine completed May 29, 2001

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAXWORDS 39000
#define HORIZ 0
#define VERT 1

using namespace std;

char words[MAXWORDS][50];

char board[15][15];
char baseletters[30];
int numwords=0;

int compoints=0;

int bestx, besty, bestdir;
char bestword[30];
int bestpoints;

//                    A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z
int pointvals[26] = { 1, 2, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10,1, 1, 1, 1, 4, 4, 8, 4, 10 };
int numleft[26] =   {

int score(char word[], int x, int y, int dir);
int ismadeof(char *word, char *letters);
int contains(char *word, char *letters);
int pointval(char c) {
  return pointvals[(unsigned char)tolower(c)-97];
}
int wpointval(char *str) {
  int len=strlen(str);
  int c=0, val=0;
  for (c=0; c<len; c++) {
    val+=pointval(str[c]);
  }
  return val;
}

void init_board() {
// Initialize board

sprintf(board[0],  "4  1   4   1  4");
sprintf(board[1],  " 3   2   2   3 ");
sprintf(board[2],  "  3   1 1   3  ");
sprintf(board[3],  "1  3   1   3  1");
sprintf(board[4],  "    3     3    ");
sprintf(board[5],  " 2   2   2   2 ");
sprintf(board[6],  "  1   1 1   1  ");
sprintf(board[7],  "4  1   3   1  4");
sprintf(board[8],  "  1   1 1   1  ");
sprintf(board[9],  " 2   2   2   2 ");
sprintf(board[10], "    3     3    ");
sprintf(board[11], "1  3   1   3   ");
sprintf(board[12], "  3   1 1   3  ");
sprintf(board[13], " 3   2   2   3 ");
sprintf(board[14], "4  1   4   1  4");
}

void display_board() {
  int c=0, d=0;
  clrscr();
  for (c=0; c<15; c++) {
    for (d=0; d<15; d++) {
      if (board[c][d]>52)
        ScreenPutChar(board[c][d], 4, c*2, d);
      else ScreenPutChar(board[c][d], 7, c*2, d);
    }
    putchar('\n');
  }
  printf("%d words in internal dictionary\n", numwords);
  printf("Computer Score: %d\n", compoints);
  printf("Baseletters: %s\n", baseletters);
}

int exists(int x, int y, int dir, char l, int lx, int ly) {
  int p, dicpos=0;
  int points=0;
  char c;
  char lastc;
  fprintf(stderr, "  Finding: ");
  switch(dir) {
    case HORIZ:
      for (p=x; p<15; p++) {
        if (p==lx&&
            y==ly)
          c=l;
        else c=board[p][y];
        if (c>52) {
          fprintf(stderr, "%c", c);
          for (;dicpos<numwords;dicpos++) {
            if (words[dicpos][p-x]==c) {
              points+=pointval(c);
              break;
            }
            if (p>x&&
                words[dicpos][p-x-1]!=lastc) {
              fprintf(stderr, " not found.\n");
              return 0;
            }
          }
          if (words[dicpos][p-x]!=c) {
            fprintf(stderr, " not found.\n");
            return 0;
          }
          lastc=c;
        }
        else break;
      }
      if (words[dicpos][p-x]) {
        fprintf(stderr, " not found.\n");
//        getch();
        return 0;
      }
      else {
        for (p=x; p<15; p++) {
          if (p==lx&&
              y==ly)
            c=l;
          else c=board[p][y];
          if (c<=52) break;
          if (words[dicpos][p-x]!=c) {
            fprintf(stderr, " not found.\n");
            return 0;
          }
        }
        fprintf(stderr, " found %d.\n", dicpos);
//        getch();
        return points;
      }
    break;
    case VERT:
      for (p=y; p<15; p++) {
        if (x==lx&&
            p==ly)
          c=l;
        else c=board[x][p];
        if (c>52) {
          fprintf(stderr, "%c", c);
          for (;dicpos<numwords;dicpos++) {
            if (words[dicpos][p-y]==c) {
              points+=pointval(c);
              break;
            }
            if (p>y&&
                words[dicpos][p-y-1]!=lastc) {
              fprintf(stderr, " not found.\n");
              return 0;
            }
          }
          if (words[dicpos][p-y]!=c) {
            fprintf(stderr, " not found.\n");
            return 0;
          }
          lastc=c;
        }
        else break;
      }
      if (words[dicpos][p-y]) {
        fprintf(stderr, " not found.\n");
//      getch();
        return 0;
      }
      else {
        for (p=y; p<15; p++) {
          if (p==ly&&
              x==lx)
            c=l;
          else c=board[x][p];
          if (c<=52) break;
          if (words[dicpos][p-y]!=c) {
            fprintf(stderr, " not found.\n");
            return 0;
          }
        }
        fprintf(stderr, " found %d.\n", dicpos);
//      getch();
        return points;
      }
    break;
  }
  return points;
}

void dobestmove() {
int d=0;
int wordpoints=0;
int wordmult=1;

if (bestpoints<=0) {
  printf("Computer exchanges all 7 tiles...\n");
  return;
}
if (bestdir==HORIZ) {
  for (d=bestx; d<bestx+strlen(bestword); d++) {
/*    switch(board[d][besty]) {
      case '1':
        wordpoints+=(2*pointval(bestword[d-bestx]));
      break;
      case '2':
        wordpoints+=(3*pointval(bestword[d-bestx]));
      break;
      case '3':
        wordpoints+=pointval(bestword[d-bestx]);
        wordmult*=2;
      break;
      case '4':
        wordpoints+=pointval(bestword[d-bestx]);
        wordmult*=3;
      break;
      default:
        wordpoints+=pointval(bestword[d-bestx]);
      break;
    }*/
    board[d][besty]=bestword[d-bestx];
  }
//  wordpoints*=wordmult;
  compoints+=bestpoints;
}
if (bestdir==VERT) {
  for (d=besty; d<besty+strlen(bestword); d++) {
/*    switch(board[bestx][d]) {
      case '1':
        wordpoints+=(2*pointval(bestword[d-besty]));
      break;
      case '2':
        wordpoints+=(3*pointval(bestword[d-besty]));
      break;
      case '3':
        wordpoints+=pointval(bestword[d-besty]);
        wordmult*=2;
      break;
      case '4':
        wordpoints+=pointval(bestword[d-besty]);
        wordmult*=3;
      break;
      default:
        wordpoints+=pointval(bestword[d-besty]);
      break;
    }*/
    board[bestx][d]=bestword[d-besty];
  }
//  wordpoints*=wordmult;
  compoints+=bestpoints;
}
bestpoints=0;
}

// returns position of wordfrag within word, only if word contains
// wordfrag, and all other letters are members of letters, otherwise
// returns -1
int containsboth(char word[], char letters[], char wordfrag[]) {
  int c=0, d=0, q=0;
  char temp[15];
  char *fpos;
  int fragpos=-1, llen=0;
  //if string is same length as wordfrag, game's up
  if (strlen(word)==strlen(wordfrag))
    return -1;

  //figure out if string contains wordfrag
  if ((fpos=strstr(word, wordfrag))==NULL)
    return -1;

  llen=strlen(letters);
  fragpos=fpos-word;

  strcpy(temp, letters);
  for (c=0; c<strlen(word); c++) {
    if (c==fragpos)
      c=fragpos+strlen(wordfrag);
    if (c>=strlen(word)) break;
    for (d=0; d<llen; d++) {
      if (word[c]==temp[d]) {
        temp[d]='*';
        break;
      }
    }
    if (d==llen) {
      for (q=0; q<llen; q++) {
        if (temp[q]==' ') {
          temp[d]='*';
          break;
        }
      }
      if (q==llen)
        return -1;
    }
  }

  return fragpos;
}

void findbest(char wordfrag[], int x, int y, int dir) {
int c=0, d=0;
int pos=0;
int currentpoints=0;

for (c=0; c<numwords; c++) {
  if ((pos=containsboth(words[c], baseletters, wordfrag))>-1) {
    if (dir==HORIZ)
      currentpoints=score(words[c], x-pos, y, dir);
    else
      currentpoints=score(words[c], x, y-pos, dir);
    if (currentpoints>bestpoints) {
      strcpy(bestword, words[c]);
      if (dir==HORIZ) {
        bestx=x-pos;
        besty=y;
      }
      else {
        bestx=x;
        besty=y-pos;
      }
      bestdir=dir;
      bestpoints=currentpoints;
    }
  }
}
}

void bestbuildoff(int x, int y) {
char wordfrag[15];
int c=0, len=0;
int maxlen=0;
wordfrag[0]='\0';
//Get best horizontal buildoff

//Find pre-existing horizontal word fragment
//Only do search if space immediately to left is non-letter
if (board[x-1][y]<=52) {
  //find wordfragment
  for (c=x; c<15; c++) {
    if (board[c][y]>52) {
      wordfrag[len]=board[c][y];
      wordfrag[len+1]='\0';
      len++;
    }
    else break;
  }
  //Find best move here
  findbest(wordfrag, x, y, HORIZ);
}


len=0;
wordfrag[0]='\0';
//Get best vertical buildoff

//Find pre-existing vertical word fragment
//Only do search if space immediately above is non-letter
if (board[x][y-1]<=52) {
  //find wordfragment
  for (c=y; c<15; c++) {
    if (board[x][c]>52) {
      wordfrag[len]=board[x][c];
      wordfrag[len+1]='\0';
      len++;
    }
    else break;
  }
  //Find best move here
  findbest(wordfrag, x, y, VERT);
}

}

void bestfirstmove() {
// find all words made up of those letters
int currentpoints=0;
int c=0, d=0;
int len=0;
for (c=0; c<numwords; c++) {
  if (contains(words[c], baseletters)) {
    // figure out best positioning tactic
    len=strlen(words[c]);
    for (d=8-len; d<8; d++) {
      currentpoints=score(words[c], d, 7, HORIZ);
      if (currentpoints>bestpoints) {
        strcpy(bestword, words[c]);
        bestx=d;
        besty=7;
        bestdir=HORIZ;
        bestpoints=currentpoints;
      }
    }
  }
}
dobestmove();
}

void bestmove() {
  int x=0, y=0;
  bestpoints=0;
  for (y=0; y<15; y++) {
    for (x=0; x<15; x++) {
      if (board[x][y]>52) {
        bestbuildoff(x, y);
      }
    }
  }
  dobestmove();
}

int score(char word[], int x, int y, int dir) {
int wordpoints=0, wordmult=1;
int otherpoints=0;
int d=0, c=0;
int begin, end;
int spoints=0;

fprintf(stderr, "Scoring %s...\n", word);

if (dir==HORIZ) {
  if (board[x-1][y]>52||
      board[x+strlen(word)][y]>52)
    return -1;
  for (d=x; d<x+strlen(word); d++) {
    if (board[d][y]>52&&
        board[d][y]!=word[d-x])
      return -1;
    if (d>14)
      return -1;
    switch(board[d][y]) {
      case '1':
        wordpoints+=(2*pointval(word[d-x]));
      break;
      case '2':
        wordpoints+=(3*pointval(word[d-x]));
      break;
      case '3':
        wordpoints+=pointval(word[d-x]);
        wordmult*=2;
      break;
      case '4':
        wordpoints+=pointval(word[d-x]);
        wordmult*=3;
      break;
      default:
        wordpoints+=pointval(word[d-x]);
      break;
    }
    //Now find any words created perpendicularly
    //First find the beginning of possible word
    if (board[d][y]<=52) {
      for (begin=y; begin>0; begin--) {
        if (board[d][begin-1]<=52)
          break;
      }
      for (end=y; end<14; end++) {
        if (board[d][end+1]<=52)
          break;
      }
      //check the word in dictionary if...
      if (begin!=end) {
        if (spoints=exists(d, begin, VERT, word[d-x], d, y))
          otherpoints+=spoints;
        else return -1;
      }
    }
  }
  wordpoints*=wordmult;
}
if (dir==VERT) {
  if (board[x][y-1]>52||
      board[x][y+strlen(word)]>52)
    return -1;
  for (d=y; d<y+strlen(word); d++) {
    if (board[x][d]>52&&
        board[x][d]!=word[d-y])
      return -1;
    if (d>14)
      return -1;
    switch(board[x][d]) {
      case '1':
        wordpoints+=(2*pointval(word[d-y]));
      break;
      case '2':
        wordpoints+=(3*pointval(word[d-y]));
      break;
      case '3':
        wordpoints+=pointval(word[d-y]);
        wordmult*=2;
      break;
      case '4':
        wordpoints+=pointval(word[d-y]);
        wordmult*=3;
      break;
      default:
        wordpoints+=pointval(word[d-y]);
      break;
    }
    //Now find any words created perpendicularly
    //First find the beginning of possible word
    if (board[x][d]<=52) {
      for (begin=x; begin>0; begin--) {
        if (board[begin-1][d]<=52)
          break;
      }
      for (end=x; end<14; end++) {
        if (board[end+1][d]<=52)
          break;
      }
      //check the word in dictionary
      if (begin!=end) {
        if (spoints=exists(begin, d, HORIZ, word[d-y], x, d))
          otherpoints+=spoints;
        else return -1;
      }
    }
  }
  wordpoints*=wordmult;
}

return wordpoints+otherpoints;
}

void load_dictionary() {
// load word database
FILE *input=fopen("word.txt", "r");

while(!feof(input)) {
  fscanf(input, "%s", words[numwords]);
  numwords++;
}
printf("%d words in dictionary.\nType ! to exit.\n", numwords);
}

int main() {
int c=0;
int wds=0;
int x, y;
char d;
char wd[15];
char str[30];

load_dictionary();
init_board();

for (c=0; c<7; c++) {
  baseletters[c]=rand()%26+97;
}

bestfirstmove();

display_board();

while(1) {

cout << "(X, Y): \n";
cin >> x >> y;
cout << "Direction: \n";
cin >> d;
cout << "Word: \n";
cin >> wd;

switch(d) {
  case 'h':
    for (c=x; c<x+strlen(wd); c++) {
      board[c][y]=wd[c-x];
    }
  break;
  case 'v':
    for (c=y; c<y+strlen(wd); c++) {
      board[x][c]=wd[c-y];
    }
  break;
}

display_board();

cout << "(X, Y): \n";
cin >> x >> y;
cout << "Direction: \n";
cin >> d;
cout << "Word: \n";
cin >> wd;

switch(d) {
  case 'h':
    for (c=x; c<x+strlen(wd); c++) {
      board[c][y]=wd[c-x];
    }
  break;
  case 'v':
    for (c=y; c<y+strlen(wd); c++) {
      board[x][c]=wd[c-y];
    }
  break;
}

display_board();

cin >> baseletters;

/*for (c=0; c<7; c++) {
  baseletters[c]=rand()%26+97;
}*/

bestmove();

display_board();

}


/*while(baseletters[0]!='!') {
scanf("%s", baseletters);

// find all words made up of those letters
/*
wds=0;
for (c=0; c<numwords; c++) {
  if (contains(words[c], baseletters)) {
    wds++;
    printf("\n%s\t%d", words[c], wpointval(words[c]));
  }
}
printf("\n%d words possible.\n", wds);
}*/

return 1;

}

int ismadeof(char *word, char *letters) {
  int c=0, d=0, len;
  char temp[30];

  if ((len=strlen(word))!=strlen(letters))
    return 0;

  strcpy(temp, letters);

  for (c=0; c<len; c++) {
    for (d=0; d<len; d++) {
      if (word[c]==temp[d]) {
        temp[d]=' ';
        break;
      }
    }
    if (d==len) return 0;
  }
  return 1;
}

int contains(char *word, char *letters) {
  int c=0, d=0, wlen, llen, q=0;
  char temp[30];

  wlen=strlen(word);
  llen=strlen(letters);

  strcpy(temp, letters);

  for (c=0; c<wlen; c++) {
    for (d=0; d<llen; d++) {
      if (word[c]==temp[d]) {
        temp[d]='*';
        break;
      }
    }
    if (d==llen) {
      for (q=0; q<llen; q++) {
        if (temp[q]==' ') {
          temp[q]='*';
          break;
        }
      }
      if (q==llen)
        return 0;
    }
  }
  return 1;
}

