// LIBRAIRIES
#include "famille.h"

// FONCTIONS
FAMILLE creerFamille(char *m, int g)
{
	FAMILLE f;

	f.nbMolecules = 1;
	f.graphe = g;
	f.molecules = malloc(strlen(m) * sizeof(char));
	f.molecules[f.nbMolecules - 1] = m;

	return f;
}

void ajouterMoleculeDansFamille(char *m, FAMILLE f)
{
	f.nbMolecules++;
	f.molecules = realloc(f.molecules, (sizeof(f.molecules) + (strlen(m) * sizeof(char))));
	f.molecules[f.nbMolecules - 1] = m;
}

graph *moleculeVersGraphe(FILE *F)
{
	if(F == NULL)
    {
        printf("Fichier introuvable : extraction du graphe impossible\n");

        exit(1);
    }

	char *line = NULL;
    size_t n = 0;

	char *toParse;

	while(strcmp(line, "\n") != 0)
	{
		toParse = getline(&line, &n, F);
	}

	int sommets = toParse[1];
	int aretes = toParse[3];

	fclose(F);

	graph *g;

	g = (graph*)malloc(sommets * aretes * sizeof(graph));

	// Initialise le graphe vide
	for(int i = 0; i < sommets; i++)
	{
		EMPTYGRAPH(g, sommets, aretes);
	}

	// Ajoute une arête à g du sommet 0 au sommet 1 avec m le nombre d'arêtes
	ADDONEEDGE(g, 0, 1, aretes);

	// Print le graphe (pour debug uniqument)
	/*
	for(int i = 0; i < sommets; i++)
	{
		for(int j = 0; j < sommets; j++)
		{
			printf("%d ", ISELEMENT(g, i, j));
		}

		printf("\n");
	}
	*/

	// Libère la mémoire allouée, A FAIRE A LA TOUTE FIN DU PROGRAMME
	free(g);
}

char **tableauFichiers(char *dir, char *ignore)
{
	char **allfiles = malloc(sizeof(char **));
	struct dirent *dp;
	DIR *dfd;

	if((dfd = opendir(dir)) == NULL)
	{
		printf("Impossible d'ouvrir %s\n", dir);
		
		return;
	}

	while((dp = readdir(dfd)) != NULL)
	{
		if(strcmp(dp->d_name, ignore) == 0)
			continue;

		*allfiles++ = dp->d_name;
		++allfiles;
    }

    closedir(dfd);
	
    return allfiles;
}

TABLEAUFAMILLES trouverFamilles(char *dir, char *nomSource)
{
	TABLEAUFAMILLES tf;

	tf.nbTotalMolecules = 0;
	tf.nbFamilles = 0;
	char **nomsFichiers = tableauFichiers(dir, nomSource);

	for(int i = 0; i < sizeof(nomsFichiers) / sizeof(nomsFichiers[0]); i++)
	{
		FILE *F = fopen(strcat(dir, nomsFichiers[i]), "r");

		// Calculer le graphe de la molécule
		moleculeVersGraphe(F);

		if(1)
		{
			// Si la molécule est isomorphe à l'une des molécules en tête d'une famille, appeler ajouterMoleculeDansFamille()
			// ajouterMoleculeFamille(nomsFichiers[i], );
		}

		else
		{
			// Sinon appeler creerFamille()
			// creerFamille(nomsFichiers[i], );
		}

		free(nomsFichiers[i]);
	}

	return tf;
}

void afficherFamilles(TABLEAUFAMILLES tf)
{
	for(int i = 0; i < tf.nbFamilles; i++)
	{
		printf("/// Famille %d ///\n", i);

		for(int j = 0; j < tf.familles[i].nbMolecules; j++)
		{
			printf("%s\n", tf.familles[i].molecules[j]);
		}

		printf("\n");
	}
}