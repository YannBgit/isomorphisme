// LIBRAIRIES
#include "dataGrabber.h"
#include "molecule.h"

// FONCTIONS

// MAIN
int main(int ac, char **av)
{
	// Récupération du fichier
	FILE *f = grab(av[1]);

	// Extraction des molécules
	extractMolecules(f);

	// Recherche d'isomorphismes

	// Fin
	exit(0);
}