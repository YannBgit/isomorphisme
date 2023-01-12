// LIBRAIRIES
#include "famille.h"

// FONCTIONS
char **tableauFichiers(int *nbTotalMolecules, char *dir, char *ignore)
{
	nbTotalMolecules = 0;
	char **allfiles = malloc(sizeof(char **));
	struct dirent *dp;
	DIR *dfd;

	if((dfd = opendir(dir)) == NULL)
	{
		printf("Impossible d'ouvrir %s\n", dir);
		
		exit(1);
	}

	while((dp = readdir(dfd)) != NULL)
	{
		if(strcmp(dp->d_name, ignore) == 0)
			continue;

		*allfiles++ = dp->d_name;
		++allfiles;
		nbTotalMolecules++;
    }

    closedir(dfd);
	
    return allfiles;
}

graph moleculeVersGraphe(FILE *F)
{
	if(F == NULL)
    {
        printf("Fichier introuvable : conversion en graphe impossible\n");

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

	graph g;

	g = malloc(sommets * aretes * sizeof(graph));

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
}

bool graphesIdentiques(graph g1, graph g2)
{
	// Comparer les graphes g1 et g2, renvoyer true s'ils sont identiques, false sinon

	return true;
}

int recupererIndiceFamille(graph g, TABLEAUFAMILLES tf)
{
	for(int i = 0; i < tf.nbFamilles; i++)
	{
		if(graphesIdentiques(g, tf.familles[i].graphe))
		{
			return i;
		}
	}

	return -1;
}

void ajouterMoleculeDansFamille(TABLEAUFAMILLES tf, int indiceFamille, char *m)
{
	tf.familles[indiceFamille].nbMolecules++;
	tf.familles[indiceFamille].nomMolecules = realloc(tf.familles[indiceFamille].nomMolecules, sizeof(tf.familles[indiceFamille].nomMolecules) + (strlen(m) * sizeof(char)));
	tf.familles[indiceFamille].nomMolecules[tf.familles[indiceFamille].nbMolecules - 1] = m;
}

void nouvelleFamille(TABLEAUFAMILLES tf, char *m, graph g)
{
	FAMILLE f;

	f.graphe = g;
	f.nbMolecules = 1;
	f.nomMolecules = malloc(strlen(m) * sizeof(char));
	f.nomMolecules[f.nbMolecules - 1] = m;

	tf.nbFamilles++;
	tf.familles = realloc(tf.familles, tf.nbFamilles * sizeof(FAMILLE));
	tf.familles[tf.nbFamilles - 1] = f;
}

TABLEAUFAMILLES classerMolecules(char *dir, char *ignore)
{
	// Création du tableau de familles
	TABLEAUFAMILLES tf;
	tf.nbFamilles = 0;

	// Stockage du nombre total de molécules et du nom des fichiers de toutes les molécules
	int nbTotalMolecules;
	char **nomsFichiers = tableauFichiers(&nbTotalMolecules, dir, ignore);

	// On boucle sur tous les fichiers
	for(int i = 0; i < nbTotalMolecules; i++)
	{
		// Ouverture du fichier de la molécule courante
		FILE *F = fopen(strcat(dir, nomsFichiers[i]), "r");

		// Calcul du graphe de la molécule courante
		graph g = moleculeVersGraphe(F);

		// On cherche l'indice de la famille à laquele appartient la molécule courante
		int indiceFamille = recupererIndiceFamille(g, tf);

		// Si l'indice ne vaut pas -1, on stock la molécule dans sa famille, sinon, on crée une famille
		if(indiceFamille != -1)
		{
			// Ajout de la molécule courante dans sa famille
			ajouterMoleculeDansFamille(tf, indiceFamille, nomsFichiers[i]);
		}

		else
		{
			// Création d'une nouvelle famille
			nouvelleFamille(tf, nomsFichiers[i], g);
		}
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
			printf("%s\n", tf.familles[i].nomMolecules[j]);
		}

		printf("\n");
	}
}

void libererMemoire(TABLEAUFAMILLES tf)
{

}