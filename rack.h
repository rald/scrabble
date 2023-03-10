#ifndef RACK_H
#define RACK_H



#define RACK_MAX 7



#ifdef RACK_IMPLEMENTATION



char *Rack_New() {
	char *rack=calloc(RACK_MAX+1,sizeof(*rack));
	return rack;
}



void Rack_Free(char **rack) {
	free(*rack);
	(*rack)=NULL;
}



ssize_t Rack_IndexOf(char *rack,char letter) {
	for(size_t i=0;i<strlen(rack);i++) {
		if(rack[i]==letter) {
			return i;
		}
	}	
	return -1;
}



int Rack_AddTile(char *rack,char letter) {
	if(strlen(rack)<RACK_MAX) {
		strcat(rack,(char[2]){tolower(letter),'\0'});
		return 1;
	}
	return 0;
}



int Rack_RemoveTile(char *rack,char letter) {
	int res=-1;
	ssize_t i=0,j=Rack_IndexOf(rack,letter);
	if(j!=-1) {
		res=rack[j];
		for(i=j;i<(ssize_t)strlen(rack)-1;i++) {
			rack[i]=rack[i+1];
		}
		rack[i]='\0';
	}	
	return res;
}


void Rack_Shuffle(char *rack) {
	for(size_t i=strlen(rack)-1;i>0;i--) {
		int j=rand()%(i+1);
		int t=rack[i];
		rack[i]=rack[j];
		rack[j]=t;
	}
}

void Rack_Arrange(char *rack,char *w) {
	char r0[RACK_MAX+1]={0};
	char r1[RACK_MAX+1]={0};
	int v=1;
	strcpy(r0,rack);
	for(size_t i=0;i<strlen(w);i++) {
		if(Rack_RemoveTile(r0,w[i])!=-1) {
			Rack_AddTile(r1,w[i]);
		} else { 
			v=0; 
			printf("invalid arrangement\n");
			break; 
		}
	}
	if(v==1) {
		strcat(r1,r0);
		strcpy(rack,r1);
	}
}


#endif /* RACK_IMPLEMENTATION */



#endif /* RACK_H */


