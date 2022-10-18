// LIBRAIRIES
#include "molecule.h"

// FONCTIONS
bool comparerMolecules(MOLECULE m1, MOLECULE m2)
{
	// Retourner true si les deux molécules sont isomorphes, sinon retourner false

	if(1)
	{
		return true;
	}

	return false;
}

void trouverIsomorphismes()
{
	// Pour chaque molécule, vérifier si elle n'est pas déjà à la fin d'un tableau du tableau de tableaux "familles", sinon passer à la suivante
	// Si la molécule est isomorphe à l'une des molécules en fin d'un tableau, realloc la taille du tableau, ajouter 1 à la taille du tableau et l'ajouter à la fin
	// Sinon, créer un tableau de taille 1, stocker sa taille (pour pouvoir l'incrémenter) et y ajouter la molécule
	// Lorsque toutes les molécules ont été classées, retourner la liste de tableaux (de char *)

	int nbFamilles = 0;
	char ***familles = (char ***)1;
	int *nbMolecules;

	// Affichage final
	for(int i = 0; i < nbFamilles; i++)
	{
		printf("/// Famille %d ///\n", i);

		for(int j = 0; j < nbMolecules[i]; j++)
		{
			printf("%s\n", familles[i][j]);
		}

		printf("\n");
	}
}