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

ssize_t Word_IndexOf(char **words,size_t nwords,char *word) {
	for(ssize_t i=0;i<(ssize_t)nwords;i++) {
		if(!strcasecmp(words[i],word)) return i;
	}
	return -1;
}


int ValidLetters(char *l,char *w,int x,int y,Direction d) {

	int n0=0;
	for(size_t i=0;i<strlen(l);i++) {
		if(l[i]=='.') n0++;
	}
	int n1=0;
	for(size_t i=0;i<strlen(w);i++) {
		if(w[i]==toupper(w[i])) n1++;
	}
	if(n0<n1) {
		printf("not enough blanks\n");
		return 0;
	}

	for(size_t i=0;i<strlen(w);i++) {
		if(d==DIRECTION_ACROSS) {
			if(board[y][x+i]==0 && w[i]==tolower(w[i]) && Rack_IndexOf(strlwr(l),tolower(w[i]))==-1) {
				printf("letter '%c' not present in rack\n",w[i]);
				return 0;
			}

			if(board[y][x+i]!=0 && tolower(board[y][x+i])!=tolower(w[i])) {
				printf("letter '%c' do not match on board\n",w[i]);
				return 0;
			}
		} else if(d==DIRECTION_DOWN) {
			if(board[y+i][x]==0 && w[i]==tolower(w[i]) && Rack_IndexOf(strlwr(l),tolower(w[i]))==-1) {
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

	int b[BOARD_HEIGHT][BOARD_WIDTH]={0};
	int a[BOARD_HEIGHT][BOARD_WIDTH]={0};

	int v=0;
	int pnt=0;


	for(int j=0;j<BOARD_WIDTH;j++)
		for(int i=0;i<BOARD_WIDTH;i++) {
			b[j][i]=board[j][i];
			a[j][i]=board[j][i]!=0?1:0;
		}



	if(d==DIRECTION_ACROSS) {

		int j,k,l;

		for(size_t i=0;i<strlen(w);i++) {
			if(b[y][i+x]==0) {
				b[y][i+x]=w[i];
			} else if(tolower(b[y][i+x])!=tolower(w[i])) {
				printf("letters dont match\n");
				return -1;
			}
		}

		j=x;
		while(j>0 && b[y][j-1]!=0) j--;
		k=x;
		while(k<BOARD_WIDTH-1 && b[y][k+1]!=0) k++;

		l=k-j+1;
		if(l>1) {
			int p=0;
			int mul=1;
			char *word=calloc(l+1,sizeof(*word));
			for(int m=0;m<l;m++) {
				word[m]=b[y][m+j];
				if(first && y==7 && m+j==7) v=1;
	 			if(a[y][m+j]==1) v=1; 
	 			if(word[m]==tolower(word[m])) {
	 				int pp=points[tolower(word[m])-'a'];
	 				if(a[y][m+j]==1) { 
	 					p+=pp;
					} else {
						switch(bonus[y][m+j]) {
							case 0: p+=pp; break;
							case 1: p+=pp*2; break;
							case 2: p+=pp*3; break;
							case 3: p+=pp; mul*=2; break;
							case 4: p+=pp; mul*=3; break;
							case 5: p+=pp; if(first) mul*=2; break;
						}
					}
	 			}
			}
			p*=mul;
			if(Word_IndexOf(dict,ndict,word)==-1) { 
				printf("invalid word \"%s\"\n",word);
				return -1;
			} else {
				pnt+=p; 
				printf("%s %d\n",word,p);
			}
			free(word);
			word=NULL;
		}

		for(size_t i=0;i<strlen(w);i++) {
			if(board[y][i+x]==0) {
				j=y;
				while(j>0 && b[j-1][i+x]!=0) j--;
				k=y;
				while(k<BOARD_WIDTH-1 && b[k+1][i+x]!=0) k++;
				l=k-j+1;
				if(l>1) {
					int p=0;
					int mul=1;
					char *word=calloc(l+1,sizeof(*word));
					for(int m=0;m<l;m++) {
						word[m]=b[m+j][i+x];
						if(first && i+x==7 && m+j==7) v=1;
			 			if(a[m+j][i+x]==1) v=1; 
			 			if(word[m]==tolower(word[m])) {
			 				int pp=points[tolower(word[m])-'a'];
			 				if(a[m+j][i+x]==1) { 
			 					p+=pp;
							} else {
								switch(bonus[m+j][i+x]) {
									case 0: p+=pp; break;
									case 1: p+=pp*2; break;
									case 2: p+=pp*3; break;
									case 3: p+=pp; mul*=2; break;
									case 4: p+=pp; mul*=3; break;
									case 5: p+=pp; if(first) mul*=2; break;
								}
							}
			 			}
					}
					p*=mul;
					if(Word_IndexOf(dict,ndict,word)==-1) { 
						printf("invalid word \"%s\"\n",word);
						return -1;
					} else {
						pnt+=p; 
						printf("%s %d\n",word,p);
					}
					free(word);
					word=NULL;
				}			
			}
		}
	
		
	} else if(d==DIRECTION_DOWN) {

		int j,k,l;

		for(size_t i=0;i<strlen(w);i++) {
			if(b[i+y][x]==0) {
				b[i+y][x]=w[i];
			} else if(tolower(b[i+y][x])!=tolower(w[i])) {
				printf("letters dont match\n");
				return -1;
			}
		}

		j=y;
		while(j>0 && b[j-1][x]!=0) j--;
		k=y;
		while(k<BOARD_WIDTH-1 && b[k+1][x]!=0) k++;

		l=k-j+1;
		if(l>1) {
			int p=0;
			int mul=1;
			char *word=calloc(l+1,sizeof(*word));
			for(int m=0;m<l;m++) {
				word[m]=b[m+j][x];
				if(first && x==7 && m+j==7) v=1;
	 			if(a[m+j][x]==1) v=1; 
	 			if(word[m]==tolower(word[m])) {
	 				int pp=points[tolower(word[m])-'a'];
	 				if(a[m+j][x]==1) { 
	 					p+=pp;
					} else {
						switch(bonus[m+j][x]) {
							case 0: p+=pp; break;
							case 1: p+=pp*2; break;
							case 2: p+=pp*3; break;
							case 3: p+=pp; mul*=2; break;
							case 4: p+=pp; mul*=3; break;
							case 5: p+=pp; if(first) mul*=2; break;
						}
					}
	 			}
			}
			p*=mul;
			if(Word_IndexOf(dict,ndict,word)==-1) { 
				printf("invalid word \"%s\"\n",word);
				return -1;
			} else {
				pnt+=p; 
				printf("%s %d\n",word,p);
			}
			free(word);
			word=NULL;
		}


		
		for(size_t i=0;i<strlen(w);i++) {
			if(board[i+y][x]==0) {
				j=x;
				while(j>0 && b[i+y][j-1]!=0) j--;
				k=x;
				while(k<BOARD_WIDTH-1 && b[i+y][k+1]!=0) k++;
				l=k-j+1;
				if(l>1) {
					int p=0;
					int mul=1;
					char *word=calloc(l+1,sizeof(*word));
					for(int m=0;m<l;m++) {
						word[m]=b[i+y][m+j];
						if(first && i+y==7 && m+j==7) v=1;
			 			if(a[i+y][m+j]==1) v=1; 
			 			if(word[m]==tolower(word[m])) {
			 				int pp=points[tolower(word[m])-'a'];
			 				if(a[i+y][m+j]==1) { 
			 					p+=pp;
							} else {
								switch(bonus[i+y][m+j]) {
									case 0: p+=pp; break;
									case 1: p+=pp*2; break;
									case 2: p+=pp*3; break;
									case 3: p+=pp; mul*=2; break;
									case 4: p+=pp; mul*=3; break;
									case 5: p+=pp; if(first) mul*=2; break;
								}
							}
			 			}
					}
					p*=mul;
					if(Word_IndexOf(dict,ndict,word)==-1) { 
						printf("invalid word \"%s\"\n",word);
						return -1;
					} else { 
						pnt+=p;
						printf("%s %d\n",word,p);
					}
					free(word);
					word=NULL;
				}
			}
		}

	}

	if(v==0) {
		printf("not valid move\n");
		return -1; 
	}

	return pnt;
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

	for(size_t i=0;i<strlen(w);i++) {
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


