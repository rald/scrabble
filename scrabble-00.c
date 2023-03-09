#ifndef SCRABBLE_H
#define SCRABBLE_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h" 

#define STRUTIL_IMPLEMENTATION
#include "strutil.h" 

#define BAG_IMPLEMENTATION
#include "bag.h" 

#define RACK_IMPLEMENTATION
#include "rack.h" 

typedef enum {
	DIRECTION_ACROSS=0,
	DIRECTION_DOWN,
	DIRECTION_MAX,
} Direction;



void Words_Add(char ***words,size_t *nwords,char *word);

int Scrabble_LoadDict(char ***dict,size_t *ndict,char *filename);

int ValidBounds(int bx,int by,int ex,int ey);
ssize_t Char_IndexOf(char *word,char ch);

int Char_Remove(char *word,char ch);
ssize_t Word_IndexOf(char **words,size_t nwords,char *word);
int ValidLetters(char *l,char *w,int x,int y,Direction d);
int ValidWords(char **dict,size_t ndict,char *w,int x,int y,Direction d,int first);
int Scrabble_Move(char **dict,size_t ndict,char *bag,char *l,char *w,int x,int y,Direction d,int f);



#ifdef SCRABBLE_IMPLEMENTATION



void Words_Add(char ***words,size_t *nwords,char *word) {
	*words=realloc(*words,sizeof(**words)*(*nwords+1));
	(*words)[(*nwords)++]=strdup(word);
}

int Scrabble_LoadDict(char ***dict,size_t *ndict,char *filename) {

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;

	FILE *fin=fopen(filename,"r");
	if(!fin) return 0;

	while((rlen=getline(&line,&llen,fin))!=-1) {
		rmnl(line);
		trim(line);
		Words_Add(dict,ndict,strlwr(line));
	}
	return 1;
}

int ValidBounds(int bx,int by,int ex,int ey) {
	return bx>=0 && ex<BOARD_WIDTH && by>=0 && ey<BOARD_HEIGHT;
}

ssize_t Char_IndexOf(char *word,char ch) {
	for(ssize_t i=0;i<strlen(word);i++) {
		if(word[i]==ch) return i;
	}
	return -1;
}

int Char_Remove(char *word,char ch) {
	int res=0;
	int j=Char_IndexOf(word,ch);
	if(j!=-1) {
		for(ssize_t i=j;i<strlen(word)-1;i++) {
			word[i]=word[i+1];
	  }
	  res=1;
	}
	return res;
}

ssize_t Word_IndexOf(char **words,size_t nwords,char *word) {
	for(ssize_t i=0;i<nwords;i++) {
		if(!strcasecmp(words[i],word)) return i;
	}
	return -1;
}


int ValidLetters(char *l,char *w,int x,int y,Direction d) {

	int n0=0;
	for(int i=0;i<strlen(l);i++) {
		if(l[i]=='.') n0++;
	}
	int n1=0;
	for(int i=0;i<strlen(w);i++) {
		if(w[i]==toupper(w[i])) n1++;
	}
	if(n0<n1) {
		printf("not enough blanks\n");
		return 0;
	}

	for(int i=0;i<strlen(w);i++) {
		if(d==DIRECTION_ACROSS) {
			if(board[y][x+i]==0 && w[i]==tolower(w[i]) && Char_IndexOf(strlwr(l),tolower(w[i]))==-1) {
				printf("letter '%c' not present in rack\n",w[i]);
				return 0;
			}

			if(board[y][x+i]!=0 && tolower(board[y][x+i])!=tolower(w[i])) {
				printf("letter '%c' do not match on board\n",w[i]);
				return 0;
			}
		} else if(d==DIRECTION_DOWN) {
			if(board[y+i][x]==0 && w[i]==tolower(w[i]) && Char_IndexOf(strlwr(l),tolower(w[i]))==-1) {
				printf("letter '%c' not present in rack\n",w[i]);
			}

			if(board[y+i][x]!=0 && tolower(board[y+i][x])!=tolower(w[i])) {
				printf("letter '%c' do not match on board\n",w[i]);
				return 0;
			}
		}
	}
	return 1;
}

int ValidWords(char **dict,size_t ndict,char *w,int x,int y,Direction d,int first) {
	int res=-1;
	int pnt=0;
	int mul=1;
	int p;
	int s=0;
	int b[BOARD_HEIGHT][BOARD_WIDTH]={0};

	for(int j=0;j<BOARD_WIDTH;j++)
		for(int i=0;i<BOARD_WIDTH;i++)
			b[j][i]=board[j][i];



	if(d==DIRECTION_ACROSS) {

		int j,k,l;

		j=x;
		while(j>0 && b[y][j-1]!=0) j--;
		k=x+strlen(w)-1;
		while(k<BOARD_WIDTH-1 && b[y][k+1]!=0) k++;

		l=k-j+1;
		if(l>1) {
			char *word=calloc(l+1,sizeof(*word));
			for(int m=0;m<l;m++) {
				word[m]=b[y][m+j];
			}
			if(Word_IndexOf(dict,ndict,word)==-1) { 
				printf("invalid word \"%s\"\n",word);
			} else { 
				printf("valid word \"%s\" %d points\n",word,p);
			}
			free(word);
			word=NULL;
		}
		
	} else if(d==DIRECTION_DOWN) {

	}

	return res;
}


int Scrabble_Move(char **dict,size_t ndict,char *bag,char *l,char *w,int x,int y,Direction d,int f) {
	int pnt=0;
	if(d==DIRECTION_ACROSS) {
		if(!ValidBounds(x,y,x+strlen(w)-1,y)) {
			printf("invalid bounds\n");
			return -1;
		}
	} else if(d==DIRECTION_DOWN) {
		if(!ValidBounds(x,y,x,y+strlen(w)-1)) {
			printf("invalid bounds\n");
			return -1;
		}
	} else {
		printf("invalid direction\n");
		return -1;
	}

	if(!ValidLetters(l,w,x,y,d)) {
		printf("invalid letters\n");
		return -1;
	}

	if((pnt=ValidWords(dict,ndict,w,x,y,d,f))==-1) {
		printf("invalid word\n");
		return -1;
	}		

	for(int i=0;i<strlen(w);i++) {
		if(d==DIRECTION_ACROSS) {
			if(board[y][x+i]==0) {
				board[y][x+i]=w[i];
				Rack_RemoveTile(l,w[i]==toupper(w[i])?'.':w[i]);
				int c=Bag_PickTile(bag);
				if(c!=-1) Rack_AddTile(l,c);
			} 
		} else if(d==DIRECTION_DOWN) {
			if(board[y+i][x]==0) {
				board[y+i][x]=w[i];
				Rack_RemoveTile(l,w[i]==toupper(w[i])?'.':w[i]);
				int c=Bag_PickTile(bag);
				if(c!=-1) Rack_AddTile(l,c);
			}
		} 
	}

	return pnt;
}



#endif /* SCRABBLE_IMPLEMENTATION */



#endif /* SCRABBLE_H */


