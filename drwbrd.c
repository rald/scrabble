#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "gifenc.h"
#include "scrabble.h" 

#include "font.h" 

uint8_t colors[]={
 26,  28,  44,	
 93,  39,  93,	
177,  62,  83,	
239, 125,  87,	
255, 205, 117,	
167, 240, 112,
 56, 183, 100,	
 37, 113, 121,	
 41,  54, 111,	
 59,	93, 201,	
 65, 166, 246,	
115, 239, 247,	
244, 244, 244,	
148, 176, 194,	
 86, 108, 134,	
 51,  60,  87,	
};

int bcolors[]={3,10,9,5,2,5};

void Draw_Point(ge_GIF *gif,int x,int y,int c) {
	gif->frame[y*gif->w+x]=c;
}

void Draw_Line(ge_GIF *gif,int x0,int y0,int x1,int y1,int color) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;
  for(;;) {
   	Draw_Point(gif,x0,y0,color);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void Draw_Rect(ge_GIF *gif,int x,int y,int rw,int rh,int c) {
	for(int i=0;i<rw;i++) {
		Draw_Point(gif,x+i,y,c);
		Draw_Point(gif,x+i,y+rh-1,c);
	}

	for(int j=1;j<rh-1;j++) {
		Draw_Point(gif,x,y+j,c);
		Draw_Point(gif,x+rw-1,y+j,c);
	}
}

void Fill_Rect(ge_GIF *gif,int x,int y,int rw,int rh,int c) {
	for(int j=0;j<rh;j++) {
		for(int i=0;i<rw;i++) {
			Draw_Point(gif,x+i,y+j,c);
		}
	}	
}

void Draw_Char(ge_GIF *gif,int ch,int x,int y,int s,int t,int c) {
	for(int j=0;j<font_height;j++) {
		for(int i=0;i<font_width;i++) {
			int k=ch*font_width*font_height+j*font_width+i;
			if(font[k]!=t) {
				Fill_Rect(gif,x+i*s,y+j*s,s,s,c);
			}
		} 	
	} 
}

void Draw_Text(ge_GIF *gif,char *u,int x,int y,int s,int t,int c) {
	for(int i=0;i<(int)strlen(u);i++) {
		Draw_Char(gif,u[i],x+i*font_width,y,s,t,c);
	}
}


void Draw_Letter(ge_GIF *gif,int ch,int x,int y) {
	Fill_Rect(gif,x*34+34+1,y*34+34+1,32,32,12);
	if(ch==toupper(ch)) {
		Draw_Char(gif,ch,x*34+34+10,y*34+34+6,2,0,10);
	} else {
		Draw_Char(gif,toupper(ch),x*34+34+10,y*34+34+6,2,0,0);
		char str[256];
		sprintf(str,"%2d",points[toupper(ch)-'A']);
		Draw_Text(gif,str,x*34+34+16,y*34+34+24,1,0,0);
	}
}

void Draw_Word(ge_GIF *gif,char *w,int x,int y,int d) {
	for(int i=0;i<(int)strlen(w);i++) {
		if(toupper(d)=='A') {
			Draw_Letter(gif,w[i],x+i,y);
		} else if(toupper(d)=='D') {
			Draw_Letter(gif,w[i],x,y+i);
		}
	}
}

void Draw_Board(ge_GIF *gif,int x,int y) {
	Fill_Rect(gif,x,y,34*17,34*17,0);
	for(int j=0;j<BOARD_HEIGHT;j++) {
		Draw_Char(gif,j+'A',j*34+34+13+x,13+y,1,0,12);
		Draw_Char(gif,j+'A',j*34+34+13+x,16*34+13+y,1,0,12);
		char str[256];
		sprintf(str,"%2d",j+1);
		Draw_Text(gif,str,13+x,j*34+34+13+y,1,0,12);
		Draw_Text(gif,str,16*34+13+x,j*34+34+13+y,1,0,12);
		for(int i=0;i<BOARD_WIDTH;i++) {
			int k=j*BOARD_WIDTH+i;
			Fill_Rect(gif,i*34+34+1+x,j*34+34+1+y,32,32,bcolors[bonus[k]]);				
		}
	}
	Draw_Char(gif,'*',7*34+34+10+x,7*34+34+10+y,2,0,0);
}

void Draw_Clear(ge_GIF *gif,int color) {
	Fill_Rect(gif,0,0,gif->w,gif->h,color);
}

int main(void) {
	int w = 34*17, h = 34*17;

	/* create a GIF */
	ge_GIF *gif = ge_new_gif(
		"example.gif",  /* file name */
		w, h,           /* canvas size */
		colors,
		4,              /* palette depth == log2(# of colors) */
		15,             /* no transparency */
		0               /* infinite loop */
	);

	char *word="homework";

	Draw_Board(gif,0,0);
	ge_add_frame(gif,100);
	for(int i=0;word[i];i++) {
		Draw_Letter(gif,word[i],i+7,7);
		ge_add_frame(gif,25);
	}
	Draw_Word(gif,word,7,7,'a');
	ge_add_frame(gif,200);
	Draw_Board(gif,0,0);
	ge_add_frame(gif,100);
	Draw_Word(gif,word,7,7,'a');
	ge_add_frame(gif,200);
	Draw_Board(gif,0,0);
	ge_add_frame(gif,100);
	Draw_Word(gif,word,7,7,'a');
	ge_add_frame(gif,200);
	Draw_Board(gif,0,0);
	ge_add_frame(gif,100);
	Draw_Word(gif,word,7,7,'a');
	ge_add_frame(gif,200);
	/* remember to close the GIF */
	ge_close_gif(gif);

	return 0;
}
