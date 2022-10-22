// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "molecule.h"

// FONCTIONS

// MAIN
int main()
{
	// Récupération du fichier
	// FILE *f = fopen("/data/*.sdf", "r"); // Ouvrir un fichier sans réseau
	FILE *f = recuperer();

	// Extraction des molécules
	extraireMolecules(f);

	// Recherche d'isomorphismes
	trouverIsomorphismes();

	// Fin
	exit(0);
}