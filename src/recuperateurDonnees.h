// LIBRAIRIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "constantes.h"

// STRUCTURES

// FONCTIONS
// Télécharge le fichier (dans ../data) contenant les molécules avec une url et le décompresse, puis renvoie le pointeur vers ce fichier
FILE *recupererFichier();

// Extrait les molécules du fichier téléchargé et les range chacune dans son propre fichier (dans ../data)
void extraireMolecules(FILE *F);