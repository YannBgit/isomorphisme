// LIBRAIRIES
#include <stdio.h>

// STRUCTURES

// FONCTIONS
// Télécharge le fichier (dans ./data) contenant les molécules avec une url
void grab(char *url);

// Extrait les molécules du fichier téléchargé et les range chacune dans leur propre fichier (dans ./data)
void extractMolecules(FILE *f);