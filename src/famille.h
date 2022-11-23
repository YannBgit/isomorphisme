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
	char **molecules;	// Tableau des noms des molécules de la famille
} FAMILLE;

// Structure tableauFamilles
typedef struct tableauFamilles
{
	int nbTotalMolecules;	// Nombre total de molécules contenues dans toutes les familles
	int nbFamilles;			// Nombre de familles contenues dans le tableau de familles
	FAMILLE *familles;		// Tableau des familles
} TABLEAUFAMILLES;

// FONCTIONS
// Crée une famille avec la molécule m comme premier membre et g le graphe de référence
FAMILLE creerFamille(char *m, int g);

// Ajoute la molécule m à la famille f
void ajouterMoleculeDansFamille(char *m, FAMILLE f);

// Cherche les isomorphismes entre les molécules et renvoie le tableau des familles de molécules isomorphes
TABLEAUFAMILLES trouverFamilles();

// Affiche les noms des molécules triées par famille
void afficherFamilles(TABLEAUFAMILLES tf);