// LIBRAIRIES
#include <stdbool.h>

// STRUCTURES
// Structure molecule
typedef struct molecule
{
	char **atomes;		// Tableau des atomes de la molécule
	int **Mincidence;	// Matrice d'incidence
} MOLECULE;

// FONCTIONS
// Compare une molécule m1 à une molécule m2 et renvoi true ou false si il y a isomorphisme ou non
bool compareMolecules(MOLECULE m1, MOLECULE m2);

// Cherche tous les isomorphismes entre toutes les molécules, renvoie une liste de listes de noms (char *) de molécules isomorphes
char ***searchAllIsomorphisms();