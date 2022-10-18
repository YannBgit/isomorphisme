// LIBRAIRIES
#include <stdbool.h>
#include <stdio.h>

// STRUCTURES
// Structure molecule
typedef struct molecule
{
	char **atomes;		// Tableau des atomes de la molécule
	int **Mincidence;	// Matrice d'incidence
} MOLECULE;

// FONCTIONS
// Compare une molécule m1 à une molécule m2 et renvoie true ou false si il y a isomorphisme entre elles ou non
bool comparerMolecules(MOLECULE m1, MOLECULE m2);

// Cherche les isomorphismes entre les molécules et affiche une liste de listes de noms (char *) de molécules isomorphes
void trouverIsomorphismes();