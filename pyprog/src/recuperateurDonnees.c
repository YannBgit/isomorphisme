// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "custom_getline.h"
// FONCTIONS
FILE *recupererFichier()
{
	// Choix du fichier
	char *Turl[NBR_URL + 1];
	char *finalfilenames[NBR_URL + 1];

	Turl[0] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_lite_3star.sdf.gz";
	Turl[1] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_complete_3star.sdf.gz";
	Turl[2] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_lite.sdf.gz";
	Turl[3] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_complete.sdf.gz";

	finalfilenames[0] = "ChEBI_lite_3star.sdf";
	finalfilenames[1] = "ChEBI_complete_3star.sdf";
	finalfilenames[2] = "ChEBI_lite.sdf";
	finalfilenames[3] = "ChEBI_complete.sdf";
	printf("\nIndiquez quel type de fichier vous souhaiteriez utiliser :\nTapez 0 pour le fichier '3 stars lite'\nTapez 1 pour le fichier '3 stars complet'\nTapez 2 pour le fichier 'lite'\nTapez 3 pour le fichier 'complet'\nTapez 4 pour entrer une url custom\nTapez 5 pour utiliser un fichier existant\n");

	int choix = -1;

	while((choix < 0) || (choix > 5))
	{
		if(scanf("%d", &choix) && (choix >= 0) && (choix < 6))
		{
			if(choix == 4)
			{
				printf("Entrez l'url du fichier désiré\n");

				if(!scanf("%ms", &Turl[4]))
				{
					printf("Erreur : vérifiez la validité de l'url\n");

					exit(1);
				}
			}

			else
			{
				if(choix == 5)
				{
					char *data = "data/";
					char *nomFichier;

					printf("Entrez le nom du fichier présent dans le répertoire data\n");

					if(!scanf("%ms", &nomFichier))
					{
						printf("Erreur : vérifiez la validité du nom du fichier\n");

						exit(1);
					}

					char *chemin = malloc((strlen(data)
					+ strlen(nomFichier))
					* sizeof(char));

					stpcpy(stpcpy(chemin, data), nomFichier);

					FILE *f = fopen(chemin, "r");

					free(chemin);

					return f;
				}
			}

			printf("L'url du fichier choisi est : %s\n", Turl[choix]);
		}

		else
		{
			printf("Entrée invalide : tapez un numéro entre 0 et 5\n");
		}
	}

	// Téléchargement du fichier
	char *url = Turl[choix];
	char *get = "cd data; sudo wget ";
	char *command = malloc((strlen(get)
	+ strlen(url) + 1)
	* sizeof(char));

	stpcpy(stpcpy(command, get), url);

	int status = system(command);

	free(command);

	if(status == -1)
	{
		printf("Erreur de téléchargement du fichier\n");

		exit(1);
	}

	// Décompression du fichier
	status = system("cd data; gzip -d *.gz");

	DIR *d = opendir("data");

	if(d)
	{
		char *data = "data/";
		struct dirent *entree;

		if((entree = readdir(d)) != NULL)
		{
			char *nomFichier = readdir(d)->d_name;

			closedir(d);

			char *chemin = malloc((strlen(data)
			+ strlen(finalfilenames[choix]) + 1)
			* sizeof(char));
			sprintf(chemin, "%s%s", data, finalfilenames[choix]);

			FILE *f = fopen(chemin, "r");

			free(chemin);

			return f;
		}

		else
		{
			printf("Erreur de lecture dans le répertoire data\n");

        	exit(1);
		}
	}

	else
	{
		printf("Impossible d'ouvrir le répertoire data\n");

        exit(1);
	}
}

char *recupererNomFichier(char *dir)
{
	struct dirent *dp;
	DIR *dfd;

	if((dfd = opendir(dir)) == NULL)
	{
		printf("Impossible d'ouvrir %s\n", dir);

		return;
	}
	while((dp = readdir(dfd)) != NULL){
		int n = strlen(dp->d_name);
		if(n >= 4){
			if( strcmp(&(dp->d_name[n-4]), ".sdf") == 0 ){
				closedir(dfd);
				return dp->d_name;
			}
	  }
	}
	printf("Erreur : fichier non trouve\n");
	return NULL;
}

void extraireMolecules(FILE *F)
{
	FILE *dest;
    char filename[50];
    char name[1000];
    //char buf[1000] = "";

    char *line = NULL;
    size_t n = 0;

    char *nom = NULL;
    size_t m = 0;

    if(F == NULL)
    {
        printf("Fichier introuvable : extraction des molécules impossible\n");

        exit(1);
    }

    else
    {
			printf("Création de fichiers individuels pour chaque molécule...\n");

	   	//n = custom_getline(&line, &n, F);
			int i = 0;
	   	while(1)
		 	{
				// Récupération de la ligne
				n = 0;
				if(line) free(line);
	      n = custom_getline(&line, &n, F);
				if(n == -1){ //break on EOF, TODO error handling
					free(line);
					break;
				}

	      // Création du fichier
	      sprintf(filename, "data/%d.sdf", i);
	      dest = fopen(filename, "w");


	      // Tant qu'on n'arrive pas à une prochaine molécule
	      while(strcmp(line, "$$$$\n") != 0)
				{
	        fputs(line, dest);

	        if(strcmp(line, "> <ChEBI Name>\n") == 0)
					{
						m = 0;
						m = custom_getline(&nom, &m, F);
						fputs(nom, dest);
						nom[m-1] = 0; //strip newline from nom
	          sprintf(name, "data/%s.sdf", nom);
	        }
					n = 0;
					if(line) free(line);
	        n = custom_getline(&line, &n, F);
	      }

	      fclose(dest);
				rename(filename, name);
				if(nom) free(nom);
				i+=1;
	    }
			printf("Number of files written : %d\n", i);
	}
  fclose(F);
}
