#include "famille.h"
#include "serialize.h"
#include "similarity.h"
#include "recuperateurDonnees.h"
#include <stdio.h>
#include <stdlib.h>

#define INDEX_LOC "index.txt"

int main(int argc, char **argv){
  if(argc != 2){
    printf("Usage : %s [id ChEBI]\n", argv[0]);
  }

  int id;
  int rc = sscanf(argv[1], "%d", &id);
  if(rc != 1){
    printf("Erreur : %s, id ChEBI invalide\n", argv[1]);
  }

  FILE *index = fopen(INDEX_LOC, "r");
  TABLEAUFAMILLES tf = readTableauFamilles(index);
  fclose(index);

  FILE *in = recupererUneMolecule(id);
  if(in == NULL){
    printf("%s\n", "recupererUneMolecule - Erreur pour obtenir le fichier ChEBI");
    exit(1);
  }
  GRAPH_NAUTY gn = moleculeVersGraphe(in);
  fclose(in);

  int i = recupererIndiceFamille(gn, tf);
  if(i == -1){
    printf("%s\n", "Famille correspondant à la molécule non trouvée dans l'index");
    exit(0);
  }

  double *similarities = get_similarities(gn, tf.familles[i]);
  if(similarities == NULL){
    printf("%s\n", "get_similarities - Le graphe n'est pas isomorphe avec la famille en entree");
  }
  printf("Similarités avec CHEBI:%d\n", id);
  for (int m = 0; m<tf.familles[i].nbMolecules; ++m)
    printf("%s : %f\n", tf.familles[i].molecules[m].nomMolecule, similarities[m]);

  free(similarities);

  freeGrapheNauty(gn);
  libererMemoire(tf);
  return 0;
}
