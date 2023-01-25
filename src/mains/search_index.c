#include "famille.h"
#include "serialize.h"
#include "similarity.h"
#include "recuperateurDonnees.h"
#include <stdio.h>
#include <stdlib.h>

#define INDEX_LOC "index.txt"

int main(int argc, char **argv)
{
  while(1)
  {
    printf("\nChoisissez une option en entrant le numéro correspondant :\nTapez 0 pour chercher une molécule par id\nTapez 1 pour quitter le programme\n");
    int choix;
    int check = scanf("%d", &choix);

    if(check != 1)
    {
      printf("Erreur : %d, choix invalide, sortie du programme\n", choix);
    }

    if(choix == 0)
    {
      printf("Entrez l'id de la molécule recherchée : ");
      int id;
      int rc = scanf("%d", &id);

      if(rc != 1)
      {
        printf("Erreur : %d, id ChEBI invalide, retour au menu\n", id);
        continue;
      }

      FILE *index = fopen(INDEX_LOC, "r");
      TABLEAUFAMILLES tf = readTableauFamilles(index);
      fclose(index);

      FILE *in = recupererUneMolecule(id);

      if(in == NULL)
      {
        printf("%s\n", "recupererUneMolecule - Erreur pour obtenir le fichier ChEBI, retour au menu");
        continue;
      }

      GRAPH_NAUTY gn = moleculeVersGraphe(in);
      fclose(in);

      int i = recupererIndiceFamille(gn, tf);

      if(i == -1)
      {
        printf("%s\n", "Famille correspondant à la molécule non trouvée dans l'index, retour au menu\n");
        continue;
      }

      double *similarities = get_similarities(gn, tf.familles[i]);

      if(similarities == NULL)
      {
        printf("%s\n", "get_similarities - Le graphe n'est pas isomorphe avec la famille en entree");
      }

      printf("//////////////////\nSimilarités avec les molécules de CHEBI pour la molécule %d\n", id);

      for (int m = 0; m<tf.familles[i].nbMolecules; ++m)
        printf("%s : %f\n", tf.familles[i].molecules[m].nomMolecule, similarities[m]);

      printf("//////////////////\n");

      free(similarities);
      freeGrapheNauty(gn);
      libererMemoire(tf);
    }

    if(choix == 1)
    {
      printf("Sortie du programme\n");
      exit(0);
    }

    else if(choix > 1)
    {
      printf("Erreur : %d, choix invalide, retour au menu\n", choix);
    }
  }
}