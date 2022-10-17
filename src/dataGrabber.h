// LIBRAIRIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STRUCTURES

// FONCTIONS
// Télécharge le fichier (dans ../data) contenant les molécules avec une url et renvoie un pointeur vers le fichier décompressé
FILE *grab(char *url);

// Extrait les molécules du fichier téléchargé et les range chacune dans son propre fichier (dans ../data) et ferme le fichier d'extraction
void extractMolecules(FILE *f);