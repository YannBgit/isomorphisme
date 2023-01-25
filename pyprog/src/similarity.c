#include "similarity.h"
#include <stdlib.h>

double* get_similarities(GRAPH_NAUTY gn, FAMILLE f) {
    int shared;
    double* similarities = malloc(sizeof(double) * f.nbMolecules);
    char **mapping = colorMapping(gn, f);
    if(mapping == NULL){
      return NULL;
    }
    for(int i = 0; i < f.nbMolecules; i++) {
      shared = 0;
      for(int k=0;k<gn.n;k++){
        if(strcmp(mapping[k], f.molecules[i].colors[k]) == 0) shared++;
      }
      similarities[i] = (double)shared/gn.n;
    }
    return similarities;
}
