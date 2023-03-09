#ifndef PLAYER_H
#define PLAYER_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "common.h"



typedef struct Player Player;

struct Player {
	char *nick;
	size_t score;
};



int cmpByScoreDesc(const void *l,const void *r);
Player *Player_New(char *nick,size_t score);
void Players_Free(Player ***players,size_t *nplayers);
void Player_Add(Player ***players,size_t *nplayers,Player *player);
void Players_Load(Player ***players,size_t *nplayers,char *fn);
void Players_Save(Player **players,size_t nplayers,char *fn);
ssize_t Player_Find(Player **players,size_t nplayers,char *nick);



#ifdef PLAYER_IMPLEMENTATION



int cmpByScoreDesc(const void *l,const void *r) {
	Player a=**(Player**)l;
	Player b=**(Player**)r;
	if(a.score<b.score) return 1;
	if(a.score>b.score) return -1;
	return 0;
}



Player *Player_New(char *nick,size_t score) {
	Player *player=malloc(sizeof(*player));
	if(player) {
		player->nick=strdup(nick);
		player->score=score;
	}
	return player;
}



void Players_Free(Player ***players,size_t *nplayers) {
	for(size_t i=0;i<*nplayers;i++) {
		free((*players)[i]->nick);
		(*players)[i]->nick=NULL;
		(*players)[i]->score=0;
	}
	free(*players);
	*players=NULL;
	*nplayers=0;	
}



void Player_Add(Player ***players,size_t *nplayers,Player *player) {
	*players=realloc(*players,sizeof(**players)*(*nplayers+1));
	(*players)[(*nplayers)++]=player;
}



void Players_Load(Player ***players,size_t *nplayers,char *fn) {
	char nick[STRING_MAX];
	size_t score=0;
	FILE *fin=fopen(fn,"r");
	if(fin) {
		while(fscanf(fin,"%s %zu",nick,&score)==2) {
			Player_Add(players,nplayers,Player_New(nick,score));
		}
	fclose(fin);
	}
}



void Players_Save(Player **players,size_t nplayers,char *fn) {
	FILE *fout=fopen(fn,"w");
	if(fout) {
		for(size_t i=0;i<nplayers;i++) {
			fprintf(fout,"%s %zu",players[i]->nick,players[i]->score);
		}
		fclose(fout);
	}
}



ssize_t Player_Find(Player **players,size_t nplayers,char *nick) {
	for(size_t i=0;i<nplayers;i++) {
		if(!strcasecmp(nick,players[i]->nick)) {
			return i;
		}
	}
	return -1;
}



#endif


#endif


