// LIBRAIRIES
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// STRUCTURES
// Structure famille
typedef struct famille
{
	int nbMolecules;	// Nombre de molécules contenues dans la famille
	int graphe;			// Ceci est un placeholder, mettre ici la structure du graphe des molécules de la famille
	char **noms;		// Tableau des noms des molécules de la famille
} FAMILLE;

// FONCTIONS
// Crée une famille avec la molécule m comme premier membre et g le graphe de référence
FAMILLE creerFamille(char *m, int g);

// Ajoute la molécule m à la famille f
// Appeler avec un pointeur vers f (utiliser "&")
void ajouterMoleculeDansFamille(FAMILLE *f, char *m);

// Cherche les isomorphismes entre les molécules et renvoie le tableau des familles de molécules isomorphes
// Appeler avec un pointeur vers Tfamilles et nbFamilles (utiliser "&")
void trouverFamilles(FAMILLE **Tfamilles, int *nbFamilles);

// Affiche les noms des molécules triées par famille
void afficherFamilles(FAMILLE *Tfamilles, int nbFamilles);