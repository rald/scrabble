#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

#define CONIO_IMPLEMENTATION
#include "conio.h"

#define SCRABBLE_IMPLEMENTATION
#include "scrabble.h"

#define BAG_IMPLEMENTATION
#include "bag.h"

#define RACK_IMPLEMENTATION
#include "rack.h"



void flush() {
	int c; while((c=getchar())!='\n' && c!=EOF);
}

void PrintBoard() {
	textbackground(BLACK);
	textcolor(WHITE);
	printf("  ");
	for(int i=0;i<BOARD_WIDTH;i++) {
		textbackground(BLACK);
		textcolor(WHITE);
		printf("%c",i+'A');
	}
	printf("\n");

	for(int j=0;j<BOARD_HEIGHT;j++) {
		textbackground(BLACK);
		textcolor(WHITE);
		printf("%2d",j+1);
		for(int i=0;i<BOARD_WIDTH;i++) {
			switch(bonus[j][i]) {
				case 0: textbackground(BLACK); textcolor(WHITE); break;
				case 1: textbackground(CYAN); textcolor(WHITE); break;
				case 2: textbackground(BLUE); textcolor(WHITE); break;
				case 3: textbackground(MAGENTA); textcolor(WHITE); break;
				case 4: textbackground(RED); textcolor(WHITE); break;
				case 5: textbackground(BLACK); textcolor(WHITE); break;
			}
			if(board[j][i]!=0) {
				textbackground(WHITE);
				textcolor(BLACK);
				printf("%c",board[j][i]);
			} else if (i==7 && j==7) {
				printf("*");				
			} else  {
				printf(".");				
			}
		}
		printf("\n");
	}
	printf("\n");
	textbackground(BLACK);
	textcolor(WHITE);
}



int main(void) {

	char **dict=NULL;
	size_t ndict=0;

	char *bag=NULL;
	char *rack[4];
	int score[4];

	srand(time(NULL));

	Scrabble_LoadDict(&dict,&ndict,"csw.txt");

	bag=Bag_New();

	for(int j=0;j<4;j++) {
		score[j]=0;
		rack[j]=Rack_New();
		for(int i=0;i<7;i++) {
			int tile=Bag_PickTile(bag);
			if(tile!=-1) {
				Rack_AddTile(rack[j],tile);
			}
		}
	}



	int f=1;

	clrscr();

	for(;;) {

		int pnt=0;
		char line[STRING_MAX];

		PrintBoard();

		for(int i=0;i<4;i++) {
			printf("%d %3d %s\n",i,score[i],rack[i]);
		}

		char col,dir;
		int row,r;
		char w[RACK_MAX+1];

 		printf("type .h for help\n");

		printf("scrabble > ");

		if(fgets(line,STRING_MAX,stdin)) {		


			rmnl(line);
			trim(line);

			clrscr();

			if(!strcmp(line,".h")) {
				printf(
					".h                                 -> this help\n"
					".w [player] [col][row][dir] [word] -> play a word\n"
					".c [player] [letters]              -> change letters\n"					 
					".s [player]                        -> shuffle letters\n"
					".a [player] [letters]              -> arrange letters\n"
					".q                                 -> quit program\n"
					"player -> 0 to 3\n"
					"col    -> A to O\n"
					"row    -> 1 to 15\n"
					"dir    -> a for across/d for down\n\n"
				);		

				printf("press enter key to continue...");
				getchar();
				clrscr();

				
			} else if(sscanf(line,".w %d %c%d%c %s",&r,&col,&row,&dir,w)==5) {		
				int x=tolower(col)-'a';
				int y=row-1;
				int d=tolower(dir)=='d'?DIRECTION_DOWN:DIRECTION_ACROSS;

				pnt=Scrabble_Move(dict,ndict,bag,rack[r],w,x,y,d,f);

				if(pnt!=-1) {
					printf("plus %d points\n",pnt);
					score[r]+=pnt;
				}

				if(f==1) f=0;
			} else if(sscanf(line,".c %d %s",&r,w)==2) {
				if(strlen(bag)<strlen(w)) {
					printf("bag has only %zu letters\n",strlen(bag));
				} else { 
					int v=1;
					for(size_t i=0;i<strlen(w);i++) {
						if(Rack_IndexOf(rack[r],tolower(w[i]))==-1) {
							v=0;
							printf("invalid letters\n");
							break;
						}
					}
					if(v==1) {
						for(size_t i=0;i<strlen(w);i++) {
							int tile=Bag_PickTile(bag);
							if(tile!=-1) {
								Rack_RemoveTile(rack[r],tolower(w[i]));
								Rack_AddTile(rack[r],tile);
							}
						}
						for(size_t i=0;i<strlen(w);i++) {
							Bag_AddTile(bag,w[i]);
						}										
					}
				}
			} else if(sscanf(line,".s %d",&r)==1) {
				Rack_Shuffle(rack[r]);
			} else if(sscanf(line,".a %d %s",&r,w)==2) {
				Rack_Arrange(rack[r],w);
			} else if(sscanf(line,".s %d",&r)==1) {
			} else {
				printf("invalid command\n");
			}

		}

		printf("\n");

	}

	return 0;
}


