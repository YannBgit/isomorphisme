// LIBRAIRIES
#include "dataGrabber.h"
#include "molecule.h"

// FONCTIONS

// MAIN
int main(int ac, char **av)
{
	grab(av[1]);

	FILE *f = fopen("nomDuFichierGrab", "r");

	extractMolecules(f);

	return 0;
}