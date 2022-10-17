// LIBRAIRIES
#include "dataGrabber.h"
#include "molecule.h"

// FONCTIONS

// MAIN
int main(int ac, char **av)
{
	// Récupération du fichier
	grab(av[1]);
	FILE *f = fopen("/data/*.sdf", "r");

	// Extraction des molécules
	extractMolecules(f);

	// Recherche d'isomorphismes

	// Fin
	//fclose(f);

	exit(0);
}