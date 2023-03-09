#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(void) {

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;

	while((rlen=getline(&line,&llen,stdin))!=-1) {
		printf("%s\n",strchr(line,' ')+1);

		free(line);
		line=NULL;
		llen=0;
		rlen=0;
	}	

	free(line);
	line=NULL;
	llen=0;
	rlen=0;


}
