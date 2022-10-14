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
bool researchIsomorphism(MOLECULE m1, MOLECULE m2);