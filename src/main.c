// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "molecule.h"

// FONCTIONS

// MAIN
int main(int ac, char **av)
{
	// Récupération du fichier
	FILE *f = recuperer(av[1]);

	// Extraction des molécules
	extraireMolecules(f);

	// Recherche d'isomorphismes
	trouverIsomorphismes();

	// Fin
	exit(0);
}