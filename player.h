#ifndef PLAYER_H
#define PLAYER_H


typedef struct Player Player;

struct Player {
	char *nick;
	int score;
	char *rack;
}; 



Player *Player_New(char *nick,int score,char *rack);



#ifdef PLAYER_IMPLEMENTATION



Player *Player_New(char *nick,int score,char *rack) {
	Player *player=malloc(sizeof(*player));
	if(player) {
		player->nick=strdup(nick);
		player->score=score;
		player->rack=strdup(rack);
	}
	return player;
}



#endif /* PLAYER_IMPLEMENTATION */



#endif /* PLAYER_H */

 
