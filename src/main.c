// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "famille.h"

// FONCTIONS

// MAIN
int main()
{
	// Récupération du fichier
	// FILE *f = fopen("data/ChEBI_lite_3star.sdf", "r"); // Ouvrir le fichier lite 3 stars sans réseau
	FILE *f = recuperer();

	// Extraction des molécules
	extraireMolecules(f);

	// Recherche d'isomorphismes
	int nbFamilles;
	FAMILLE *Tfamilles;
	trouverFamilles(&Tfamilles, &nbFamilles);

	// Affichage des familles
	afficherFamilles(Tfamilles, nbFamilles);

	// Fin
	exit(0);
}