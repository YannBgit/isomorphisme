#include "famille.h"

void writeGraphSimple(GRAPH_NAUTY gn, FILE *out);
void writeMoleculeFamille(MOLECULE_FAMILLE mf, int n, FILE *out);
void writeFamille(FAMILLE f, FILE *out);
void writeTableauFamilles(TABLEAUFAMILLES tf, FILE *out);

GRAPH_NAUTY readGraphSimple(FILE *in);
MOLECULE_FAMILLE readMoleculeFamille(FILE *in, int n);
FAMILLE readFamille(FILE *in);
TABLEAUFAMILLES readTableauFamilles(FILE *in);
