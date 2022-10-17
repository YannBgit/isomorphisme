// LIBRAIRIES
#include "dataGrabber.h"

// FONCTIONS
void grab(char *url)
{
	char *get = "cd data; sudo wget ";
	char *command = malloc((strlen(get)
	+ strlen(url))
	* sizeof(char));

	stpcpy(stpcpy(command, get), url);

	int status = system(command);

	free(command);

	if(status == -1)
	{
		printf("Erreur de téléchargement du fichier\n");

		exit(1);
	}

	status = system("cd data; gzip -d *.gz");
}

void extractMolecules(FILE *f)
{
	
}