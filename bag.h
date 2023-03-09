#ifndef BAG_H
#define BAG_H


#include <stdlib.h>
#include <string.h>

#include "common.h"


#define BAG_MAX 100



char *Bag_New();
void Bag_Free(char **bag);
int Bag_AddTile(char *bag,char letter);
int Bag_PickTile(char *bag);



#ifdef BAG_IMPLEMENTATION



char *Bag_New() {
	char *bag=calloc(BAG_MAX+1,sizeof(*bag));
	if(bag) {
		for(int i=0;i<NUM_LETTERS;i++) {
			for(int j=0;j<pieces[i];j++) {
				Bag_AddTile(bag,letters[i]);
			}	
		}
	}
	return bag;
}



void Bag_Free(char **bag) {
	free(*bag);
	(*bag)=NULL;
}



int Bag_AddTile(char *bag,char letter) {
	if(strlen(bag)<BAG_MAX) {
		strcat(bag,(char[2]){tolower(letter),'\0'});
		return 1;
	}
	return 0;
}



int Bag_PickTile(char *bag) {
	int res=-1;
	if(strlen(bag)>0) {
		ssize_t i=0,j=rand()%strlen(bag);
		res=bag[j];		
		for(i=j;i<(ssize_t)strlen(bag)-1;i++) {
			bag[i]=bag[i+1];
		}
		bag[i]='\0';
	}
	return res;
}



#endif



#endif 


