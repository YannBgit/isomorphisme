// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "famille.h"
#include "serialize.h"
// FONCTIONS

// MAIN
int main()
{
	// Récupération du fichier
	FILE *F = recupererFichier();
	char *nomSource = recupererNomFichier("data");

	// Extraction des molécules
	extraireMolecules(F);
	
	// Recherche d'isomorphismes
	TABLEAUFAMILLES tf = classerMolecules("data", nomSource);

	//Serialisation des familles
	FILE *out = fopen("index.txt", "w");
	writeTableauFamilles(tf, out);
	fclose(out);

	libererMemoire(tf);
	// Fin
	exit(0);
}
