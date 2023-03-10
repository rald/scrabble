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
	printf("\n  ");
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
		textbackground(BLACK);
		textcolor(WHITE);
		printf("\n");
	}
	textbackground(BLACK);
	textcolor(WHITE);
	printf("\n");
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
 	printf("type .h for help\n\n");

	for(;;) {

		int pnt=0;
		char line[STRING_MAX];
	
		char col,dir;
		int row,r;
		char w[STRING_MAX];

		printf("scrabble > ");

		if(fgets(line,STRING_MAX,stdin)) {		

			rmnl(line);
			trim(line);

			if(!strcmp(line,".h")) {

				printf(
					".h -> this help\n"
					".w [player] [col][row][dir] [word] -> play a word\n"
					".c [player] [letters] -> change letters\n"					 
					".s [player] -> shuffle letters\n"
					".a [player] [letters] -> arrange letters\n"
					".b -> print board\n"
					".r -> print racks\n"
					".d [word] -> define word\n"
					".l -> clear screen\n"
					".q -> quit program\n"
					"player -> 0 to 3\n"
					"col -> A to O\n"
					"row -> 1 to 15\n"
					"dir -> a for across/d for down\n"
				);		

			} else if(!strcmp(line,".q")) {
				break;
			} else if(!strcmp(line,".l")) {
				clrscr();
			} else if(!strcmp(line,".b")) {
				PrintBoard();
			} else if(!strcmp(line,".r")) {
				printf("\n");
				for(int i=0;i<4;i++) {
					printf("%d %3d %s\n",i,score[i],rack[i]);
				}
				printf("\n");
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
				Rack_Change(bag,rack[r],w);
			} else if(sscanf(line,".c %d",&r)==1) {
				Rack_Change(bag,rack[r],rack[r]);
			} else if(sscanf(line,".s %d",&r)==1) {
				Rack_Shuffle(rack[r]);
			} else if(sscanf(line,".a %d %s",&r,w)==2) {
				Rack_Arrange(rack[r],w);
			} else if(sscanf(line,".d %s",w)==1) {
				FILE *fin=fopen("cswd.txt","r");
				while(fgets(line,STRING_MAX,fin)) {
					int l=strchr(line,'\t')-line;
					char h[STRING_MAX];
					h[0]='\0';
					strncpy(h,line,l);
					h[l]='\0';
					if(!strcasecmp(w,h)) {
						printf("%s",line);
					}
				}
				fclose(fin);
			} else {
				printf("invalid command\n");
			}

		}

	}

	printf("bye\n");

	return 0;
}


