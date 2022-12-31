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

graph *molecule_vers_graphe(FILE *F)
{
	int n = 4;  // Nombre de sommets A GET DANS F
	int m = 4;  // Nombre d'arêtes A GET DANS F

	graph *g;

	g = (graph*)malloc(n * m * sizeof(graph));

	// Initialise le graphe vide
	for(int i = 0; i < n; i++)
	{
		EMPTYGRAPH(g, n, m);
	}

	// Ajoute une arête à g du sommet 0 au sommet 1 avec m le nombre d'arêtes
	ADDONEEDGE(g, 0, 1, m);

	// Print le graphe (pour debug uniqument)
	for(int i = 0; i < n; i++)
	{
		int j;
		for (j = 0; j < n; j++)
		{
		printf("%d ", ISELEMENT(g, i, j));
		}
		printf("\n");
	}

	// Libère la mémoire allouée, A FAIRE A LA TOUTE FIN DU PROGRAMME
	free(g);
}

TABLEAUFAMILLES trouverFamilles()
{
	TABLEAUFAMILLES tf;

	tf.nbTotalMolecules = 0;
	tf.nbFamilles = 0;

	// Pour chaque molécule, calculer son graphe
	// Si la molécule est isomorphe à l'une des molécules en tête d'une famille, appeler ajouterMoleculeDansFamille()
	// Sinon appeler creerFamille()
	// Lorsque toutes les molécules ont été classées, retourner la liste de tableaux (de char *)

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