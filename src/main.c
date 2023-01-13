// LIBRAIRIES
#include "recuperateurDonnees.h"
#include <stdlib.h>
// FONCTIONS

// MAIN
int main()
{
	// Récupération du fichier
	// FILE *F = fopen("data/ChEBI_lite_3star.sdf", "r"); // Ouvrir le fichier lite 3 stars sans réseau
	FILE *F = recupererFichier();
	char *nomSource = recupererNomFichier("data");

	// Extraction des molécules
	extraireMolecules(F);

	char command[200];
	sprintf(command, "rm data/%s", nomSource);
	system(command);
	// Recherche d'isomorphismes
<<<<<<< HEAD
	//TABLEAUFAMILLES tf = trouverFamilles("data", nomSource);
=======
	TABLEAUFAMILLES tf = classerMolecules("data", nomSource);
>>>>>>> 27f6e918f81e2a86be58dafb56677ee00b7dafb6

	// Affichage des familles
	//afficherFamilles(tf);

	libererMemoire(tf);

	// Fin
	exit(0);
}
