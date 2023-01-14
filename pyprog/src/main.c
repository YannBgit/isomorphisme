// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "famille.h"
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
	// Recherche d'isomorphismes

	TABLEAUFAMILLES tf = classerMolecules("data", nomSource);

	// Affichage des familles
	//afficherFamilles(tf);

	libererMemoire(tf);

	// Fin
	exit(0);
}
