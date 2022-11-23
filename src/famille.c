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

TABLEAUFAMILLES trouverFamilles()
{
	// Pour chaque molécule, calculer son graphe
	// Si la molécule est isomorphe à l'une des molécules en tête d'une famille, appeler ajouterMoleculeDansFamille()
	// Sinon appeler creerFamille()
	// Lorsque toutes les molécules ont été classées, retourner la liste de tableaux (de char *)

	TABLEAUFAMILLES tf;

	tf.nbTotalMolecules = 0;
	tf.nbFamilles = 0;

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