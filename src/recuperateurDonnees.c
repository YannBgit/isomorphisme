// LIBRAIRIES
#include "recuperateurDonnees.h"

// FONCTIONS
FILE *recuperer()
{
	// Choix du fichier
	char *Turl[NBR_URL + 1];

	Turl[0] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_lite_3star.sdf.gz";
	Turl[1] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_complete_3star.sdf.gz";
	Turl[2] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_lite.sdf.gz";
	Turl[3] = "https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_complete.sdf.gz";


	printf("\nIndiquez quel type de fichier vous souhaiteriez utiliser :\nTapez 0 pour le fichier '3 stars lite'\nTapez 1 pour le fichier '3 stars complet'\nTapez 2 pour le fichier 'lite'\nTapez 3 pour le fichier 'complet'\nTapez 4 pour entrer une url custom\nTapez 5 pour utiliser un fichier existant\n");

	int choix = -1;

	while((choix < 0) || (choix > 5))
	{
		if(scanf("%d", &choix) && (choix >=0) && (choix < 6))
		{
			if(choix == 4)
			{
				printf("Entrez l'url du fichier désiré\n");

				if(!scanf("%ms", &Turl[4]))
				{
					printf("Erreur : vérifiez la validité de l'url\n");
				}
			}

			else
			{
				if(choix == 5)
				{
					char *data = "/data/";
					char *nomFichier;

					printf("Entrez le nom du fichier existant dans le répertoire /data\n");

					if(!scanf("%ms", &nomFichier))
					{
						printf("Erreur : entrez un nom de fichier valide\n");
					}

					char *chemin = malloc((strlen(data)
					+ strlen(nomFichier))
					* sizeof(char));

					stpcpy(stpcpy(chemin, data), nomFichier);

					return fopen(chemin, "r");
				}
			}

			printf("L'url du fichier choisi est : %s\n", Turl[choix]);
		}

		else
		{
			printf("Entrée invalide : tapez un numéro entre 0 et 4\n");
		}
	}

	// Téléchargement du fichier
	char *url = Turl[choix];
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

	// Décompression du fichier
	status = system("cd data; gzip -d *.gz");

	return fopen("/data/*.sdf", "r");
}

void extraireMolecules(FILE *f)
{
	// Lire le fichier f et stocker chaque molécule dans un fichier à son nom 
    //(on ne stock pas toutes les molécules dans une liste de structures MOLECULES pour ne pas exploser la mémoire)

    FILE *dest;
    char filename[50];
    char name[1000];
    char buf[1000] = "";
    int i = 0;

    char *line = NULL;
    size_t n = 0;

    char *nom = NULL;
    size_t m = 0;
   
    if(f == NULL)
    {
        printf("Impossible d'ouvrir le fichier d'entrée\n");

        exit(1);
    }

    else
    {
        n = getline(&line, &n, f);

        while(fgets(buf,1000,f) != NULL)
		{
            //Créé le fichier
            sprintf(filename, "data/%d.sdf", i);
            dest = fopen(filename, "w");

            //récupere la ligne
            n = getline(&line, &n, f);

            //tant qu'on n'arrive pas a une prochaine molecule
            while(strcmp(line, "$$$$\n") != 0)
			{
                fputs(line, dest);

                if(strcmp(line, "> <ChEBI Name>\n") == 0)
				{ //renommer le fichier
                    m = getline(&nom, &m, f);
                    printf("NOM : %s\n", nom);
                    sprintf(name,"data/%s.sdf", nom);
                    rename(filename, name);
                }

                n = getline(&line, &n, f);
            }

            i = i+1;
            fclose(dest);
        }
   }

   fclose(f);
}