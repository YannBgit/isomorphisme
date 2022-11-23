// LIBRAIRIES
#include "famille.h"

// FONCTIONS
FAMILLE creerFamille(char *m, int g)
{
	FAMILLE f;

	f.nbMolecules = 1;
	f.graphe = g;
	f.noms = malloc(strlen(m) * sizeof(char));
	f.noms[f.nbMolecules - 1] = m;

	return f;
}

void ajouterMoleculeDansFamille(FAMILLE *f, char *m)
{
	f->nbMolecules++;
	f->noms = realloc(f->noms, (sizeof(f->noms) + (strlen(m) * sizeof(char))));
	f->noms[f->nbMolecules - 1] = m;
}

void trouverFamilles(FAMILLE **Tfamilles, int *nbFamilles)
{
	// Pour chaque molécule, si la molécule est isomorphe à l'une des molécules en tête d'une famille, appeler ajouterMoleculeDansFamille()
	// Sinon appeler creerFamille()
	// Lorsque toutes les molécules ont été classées, retourner la liste de tableaux (de char *)

	nbFamilles = 0;
}

void afficherFamilles(FAMILLE *Tfamilles, int nbFamilles)
{
	for(int i = 0; i < nbFamilles; i++)
	{
		printf("/// Famille %d ///\n", i);

		for(int j = 0; j < Tfamilles[i].nbMolecules; j++)
		{
			printf("%s\n", Tfamilles[i].noms[j]);
		}

		printf("\n");
	}
}