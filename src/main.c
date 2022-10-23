// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "molecule.h"

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
	trouverIsomorphismes();

	// Fin
	exit(0);
}