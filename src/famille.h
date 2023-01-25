#ifndef FAMILLE_H
#define FAMILLE_H

// LIBRAIRIES
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include "nauty.h"

// STRUCTURES
// Structure famille
typedef struct graph_nauty{
	int n, m, nauty_m;
	size_t size;
	graph *g;
	char **colors; //atom labels are stored as 3 characters in .sdf files
} GRAPH_NAUTY;

typedef struct molecule_famille{
	char *nomMolecule;
	char **colors; //attention, peut être NULL après remplissage dans certains cas, voir colorMapping
} MOLECULE_FAMILLE;

typedef struct famille
{
	GRAPH_NAUTY graphe;			// raphe des molécules de la famille
	int nbMolecules;		// Nombre de molécules contenues dans la famille
	char **nomMolecules;	// Tableau des noms des molécules de la famille
	MOLECULE_FAMILLE *molecules;
} FAMILLE;

// Structure tableauFamilles
typedef struct tableauFamilles
{
	int nbFamilles;			// Nombre de familles contenues dans le tableau de familles
	FAMILLE *familles;		// Tableau des familles
} TABLEAUFAMILLES;

// FONCTIONS
// Prend en argument le chemin vers un répertoire et retourne une liste des noms de tous les fichiers qu'il contient
char **tableauFichiers(int *nbTotalMolecules, char *dir, char *ignore);

// Convertit une molécule donnée en tant que son fichier de stockage et renvoie le graphe de cette molécule
GRAPH_NAUTY moleculeVersGraphe(FILE *F);

// Vérifie si deux graphes sont identiques et renvoie un booléen
bool graphesIdentiques(GRAPH_NAUTY g1, GRAPH_NAUTY g2);

// Donne l'indice de la famille à laquelle correspond le graphe g
int recupererIndiceFamille(GRAPH_NAUTY g, TABLEAUFAMILLES tf);

//Calcule le mapping de couleurs de to_map associé à la famille ref
char **colorMapping(GRAPH_NAUTY to_map, FAMILLE ref);

// Ajoute la molécule m à sa famille d'indice indiceFamille
void ajouterMoleculeDansFamille(TABLEAUFAMILLES tf, int indiceFamille, char *m, GRAPH_NAUTY g);

// Crée une famille avec la molécule m comme premier membre et g le graphe de référence, et l'ajoute au tableau de familles
void nouvelleFamille(TABLEAUFAMILLES *tf, char *m, GRAPH_NAUTY g);

// Cherche les isomorphismes entre les molécules et renvoie le tableau des familles de molécules isomorphes
TABLEAUFAMILLES classerMolecules(char *dir, char *ignore);

// Affiche les noms des molécules triées par famille
void afficherFamilles(TABLEAUFAMILLES tf);

//Libère la mémoire d'un GRAPH_NAUTY
void freeGrapheNauty(GRAPH_NAUTY gn);

//Libère la mémoire des molécules d'une famille (n est le nombre de sommets des molécules)
void freeMoleculeFamille(MOLECULE_FAMILLE mf, int n);

// Libère la mémoire d'une famille
void freeFamille(FAMILLE f);

// Libère toute la mémoire utilisée par le tableau de familles, comprenant les graphes, les tableaux de noms de molécules et les tableaux de familles
void libererMemoire(TABLEAUFAMILLES tf);

#endif /* end of include guard: FAMILLE_H */
