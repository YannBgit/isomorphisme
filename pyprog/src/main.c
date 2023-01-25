// LIBRAIRIES
#include "recuperateurDonnees.h"
#include "famille.h"
#include "serialize.h"
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
	afficherFamilles(tf);

	FILE *out = fopen("index.txt", "w");

	//Serialisation des familles
	writeTableauFamilles(tf, out);
	fclose(out);
	FILE *in = fopen("index.txt", "r");
	TABLEAUFAMILLES tfcmp = readTableauFamilles(in);
	fclose(in);

	FILE *outcmp = fopen("index_cmp.txt", "w");
	writeTableauFamilles(tfcmp, outcmp);
	fclose(outcmp);
	
	libererMemoire(tf);
	libererMemoire(tfcmp);
	// Fin
	exit(0);
}
