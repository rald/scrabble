#include <stdio.h>
#include <stdlib.h>
#include <time.h>



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
	printf("   ");
	for(int i=0;i<BOARD_WIDTH;i++) {
		printf("%c",i+'A');
	}
	printf("\n\n");

	for(int j=0;j<BOARD_HEIGHT;j++) {
		printf("%2d ",j+1);
		for(int i=0;i<BOARD_WIDTH;i++) {
			if(board[j][i]!=0) {
				printf("%c",board[j][i]);
			} else if (i==7 && j==7) {
				printf("*");				
			} else  {
				switch(bonus[j][i]) {
					case 0: printf("."); break;
					case 1: printf("1"); break;
					case 2: printf("2"); break;
					case 3: printf("3"); break;
					case 4: printf("4"); break;
				}
			}
		}
		printf("\n");
	}
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

	for(;;) {

		int pnt=0;
		char *line=NULL;
		size_t llen=0;
		ssize_t rlen=0;

		PrintBoard();

		for(int i=0;i<4;i++) {
			printf("%d %3d %s\n",i,score[i],rack[i]);
		}

		char col,dir;
		int row,r;
		char w[RACK_MAX+1];

		if((rlen=getline(&line,&llen,stdin))!=-1) {		

			rmnl(line);
			trim(line);

			if(sscanf(line,".w %d %c%d%c %s",&r,&col,&row,&dir,w)==5) {		
				int x=tolower(col)-'a';
				int y=row-1;
				int d=tolower(dir)=='d'?DIRECTION_DOWN:DIRECTION_ACROSS;

				pnt=Scrabble_Move(dict,ndict,bag,rack[r],w,x,y,d,f);

				if(pnt!=-1) score[r]+=pnt;

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
			} else {
				printf("invalid command\n");
			}

		}

	}

	return 0;
}


