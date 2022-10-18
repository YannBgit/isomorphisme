// LIBRAIRIES
#include "dataGrabber.h"

// FONCTIONS
FILE *grab(char *url)
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

	return fopen("/data/*.sdf", "r");
}

void extractMolecules(FILE *f)
{
	// Lire le fichier f et stocker chaque molécule dans un fichier à son nom (on ne stock pas toutes les molécules dans une liste de structures MOLECULES pour ne pas exploser la mémoire)
}