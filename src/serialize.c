#include "serialize.h"

void writeGraphSimple(GRAPH_NAUTY gn, FILE *out){
  set *row;
  fprintf(out, "sG\n");
  fprintf(out, "%d %d\n", gn.n, gn.m);
  for(int i = 0; i<gn.n; ++i){
    row = GRAPHROW(gn.g,i,gn.nauty_m);
    for(int j = i+1; j<gn.n; ++j){
      if(ISELEMENT(row,j)){
        fprintf(out, "%d %d\n", i, j);
      }
    }
  }
  fprintf(out, "eG\n");
}

void writeMoleculeFamille(MOLECULE_FAMILLE mf, int n, FILE *out){
    fprintf(out, "%s\n", mf.nomMolecule);
    for(int i = 0; i < n; ++i){
        fprintf(out, "%s ", mf.colors[i]);
    }
    fprintf(out, "\n");
}

void writeFamille(FAMILLE f, FILE *out){
  fprintf(out, "sF\n");
  writeGraphSimple(f.graphe, out);
  fprintf(out, "%d\n", f.nbMolecules);
  for(int i = 0; i < f.nbMolecules; ++i){
    writeMoleculeFamille(f.molecules[i], f.graphe.n, out);
  }
  fprintf(out, "eF\n");
}

void writeTableauFamilles(TABLEAUFAMILLES tf, FILE *out){
  fprintf(out, "%d\n", tf.nbFamilles);
  for(int i = 0; i<tf.nbFamilles; ++i){
    writeFamille(tf.familles[i], out);
  }
}

GRAPH_NAUTY readGraphSimple(FILE *in){
  if(in == NULL)
  {
      printf("Fichier introuvable : conversion en graphe impossible\n");

      exit(1);
  }
  char buf_delim[4];
  char *res = fgets(buf_delim, 4, in);
  if(!res || strcmp(buf_delim, "sG\n") != 0){
    printf("readGraphSimple - bad delim\n");
    exit(1);
  }


	//parse header
	int n, m;
  int rc;
  rc = fscanf(in, "%d %d\n", &n, &m);
  if(rc != 2 || rc == EOF){
		printf("readGraphSimple - header : read error\n");
		exit(1);
	}
	//create graph & read edges
  int nauty_m = SETWORDSNEEDED(n);
	graph *g = ALLOCS((nauty_m),(n)*sizeof(graph));
	EMPTYGRAPH(g,nauty_m,n);
	for(int i = 0; i<m; ++i){
		int e1, e2;
    rc = fscanf(in, "%d %d\n", &e1, &e2);
    if(rc != 2 || rc == EOF){
  		printf("readGraphSimple - edge : read error\n");
  		exit(1);
  	}
		ADDONEEDGE(g, e1, e2, nauty_m);
	}
	GRAPH_NAUTY gn = {.n = n,
                    .nauty_m = nauty_m,
			              .m = m,
										.size=(m)*(n)*sizeof(graph),
										.g=g,
										.colors = NULL,
									 };
  res = fgets(buf_delim, 4, in);
  if(!res || strcmp(buf_delim, "eG\n") != 0){
    printf("readGraphSimple - bad delim\n");
    exit(1);
  }
	return gn;
}
MOLECULE_FAMILLE readMoleculeFamille(FILE *in, int n){
  MOLECULE_FAMILLE mf;
  char *nomMolecule = malloc(1024);
  char *res = fgets(nomMolecule, 1024, in);
  int rc;

  if(!res){
    printf("readMoleculeFamille - error reading name\n");
    exit(1);
  }
  int len = strlen(nomMolecule);
  nomMolecule[len-1] = 0; //strip ending new line
  mf.nomMolecule = nomMolecule;

  mf.colors = malloc(n * sizeof(char *));
  for(int i = 0; i < n; ++i){
    char *color = malloc(4);
    int read = fread(color, 3, 1, in);
    if(read != 1){
      printf("readMoleculeFamille - error reading color\n");
      exit(1);
    }
    color[3] = 0;
    rc = fgetc(in); //space character, there's a typo for diamond
    if(rc == EOF){
      printf("readMoleculeFamille - color separator error\n");
      exit(1);
    }
    mf.colors[i] = color;
  }
  rc = fscanf(in, "\n");
  if(rc == EOF){
    printf("readMoleculeFamille - ending new line error\n");
    exit(1);
  }
  return mf;
}
FAMILLE readFamille(FILE *in){
  FAMILLE f;

  char buf_delim[4];
  char *res = fgets(buf_delim, 4, in);
  if(!res || strcmp(buf_delim, "sF\n") != 0){
    printf("readFamille - bad delim\n");
    exit(1);
  }
  GRAPH_NAUTY gn = readGraphSimple(in);
  f.graphe = gn;

  int nbMolecules;
  int rc;
  rc = fscanf(in, "%d\n", &nbMolecules);
  if(rc != 1 || rc == EOF){
		printf("readGraphSimple - header : read error\n");
		exit(1);
	}
  f.nomMolecules = NULL;
  f.nbMolecules = nbMolecules;
  f.molecules = malloc(nbMolecules*sizeof(MOLECULE_FAMILLE));
  for(int i = 0; i < f.nbMolecules; ++i){
    MOLECULE_FAMILLE mf = readMoleculeFamille(in, f.graphe.n);
    f.molecules[i] = mf;
  }
  res = fgets(buf_delim, 4, in);
  if(!res || strcmp(buf_delim, "eF\n") != 0){
    printf("readFamille - bad delim\n");
    exit(1);
  }
  return f;
}

TABLEAUFAMILLES readTableauFamilles(FILE *in){
  TABLEAUFAMILLES tf;
  int nbFamilles;
  int rc;

  rc = fscanf(in, "%d\n", &nbFamilles);
  if(rc != 1 || rc == EOF){
		printf("readTableauFamilles - header : read error\n");
		exit(1);
	}

  tf.nbFamilles = nbFamilles;
  tf.familles = malloc(tf.nbFamilles * sizeof(FAMILLE));
  for(int i = 0; i < nbFamilles; ++i){
    FAMILLE f = readFamille(in);
    tf.familles[i] = f;
  }
  return tf;
}
