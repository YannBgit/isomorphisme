// LIBRAIRIES
#include "famille.h"
#include "custom_getline.h"
#include <errno.h>
// FONCTIONS
char **tableauFichiers(int *nbTotalMolecules, char *dir, char *ignore)
{
	printf("Enter tableauFichiers\n");
	*nbTotalMolecules = 0;
	struct dirent *dp;
	DIR *dfd;

	if((dfd = opendir(dir)) == NULL)
	{
		printf("Impossible d'ouvrir %s\n", dir);

		exit(1);
	}

	while((dp = readdir(dfd)) != NULL)
	{
		if(strcmp(dp->d_name, ignore) == 0 || strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;
		(*nbTotalMolecules)++;
  }
  closedir(dfd);
	dfd = NULL;
	char **allfiles = malloc((*nbTotalMolecules) * sizeof(char *));
		if((dfd = opendir(dir)) == NULL)
		{
			printf("Impossible d'ouvrir %s\n", dir);

			exit(1);
		}
		int k = 0;
		while((dp = readdir(dfd)) != NULL)
		{
			if(strcmp(dp->d_name, ignore) == 0 || strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
				continue;
			printf("tableauFichiers : adding file %s\n", dp->d_name);
			allfiles[k] = malloc(strlen(dp->d_name)+1);
			if(allfiles[k] == NULL){
				printf("tableauFichiers : error while malloc'ing\n");
			}
			memcpy(allfiles[k], dp->d_name, strlen(dp->d_name)+1);
			printf("%s\n", allfiles[k]);
			printf("copied");

			printf("%d\n", k);
			k++;
	  }

	    closedir(dfd);

    return allfiles;
		printf("Exit tableauFichiers\n");
}

GRAPH_NAUTY moleculeVersGraphe(FILE *F)
{
	if(F == NULL)
    {
        printf("Fichier introuvable : conversion en graphe impossible\n");

        exit(1);
    }

	char *line = NULL;
	size_t l = 0;
	//skip 3 lines to get to molecule header
	for(int i = 0; i<3; ++i){
		line = NULL;
		l = 0;
		if(custom_getline(&line, &l, F) == -1){
			printf("moleculeVersGraphe - skip 3 : read error\n");
			exit(1);
		}
		if(line) free(line);
	}

	//parse header
	char *endptr;
	line = NULL;
	l = 0;
	if(custom_getline(&line, &l, F) == -1){
		printf("moleculeVersGraphe - header : read error\n");
		exit(1);
	}
	int n, m;
	char sn[4], sm[4];
	strncpy(sn, line, 3);
	sn[3] = 0;
	strncpy(sm, &line[3], 3);
	sm[3] = 0;

	errno = 0;
	n = strtol(sn, &endptr, 10);
	if (errno != 0 || endptr == sn) {
    printf("moleculeVersGraphe : error reading n\n");
    exit(1);
  }
	errno = 0;
	m = strtol(sm, &endptr, 10);
	if (errno != 0 || endptr == sm) {
    printf("moleculeVersGraphe : error reading m\n");
    exit(1);
  }
	if(line) free(line);
	//skip atom lines    TODO : take in account atoms
	for(int i = 0; i<n; ++i){
		line = NULL;
		l = 0;
		if(custom_getline(&line, &l, F) == -1){
			printf("moleculeVersGraphe - skip atoms : read error\n");
			exit(1);
		}
		if(line) free(line);
	}
	//create graph & read edges
	graph *g = ALLOCS((m),(n)*sizeof(graph));
	EMPTYGRAPH(g,m,n);
	for(int i = 0; i<m; ++i){
		line = NULL;
		l = 0;
		if(custom_getline(&line, &l, F) == -1){
			printf("moleculeVersGraphe - edge #%d : read error\n", i);
			exit(1);
		}

		int e1, e2;
		char se1[4], se2[4];
		strncpy(se1, line, 3);
		se1[3] = 0;
		strncpy(se2, &line[3], 3);
		se2[3] = 0;

		errno = 0;
		e1 = strtol(se1, &endptr, 10);
		if (errno != 0 || endptr == se1) {
	    printf("moleculeVersGraphe - edge #%d : error reading e1\n", i);
	    exit(1);
	  }
		errno = 0;
		e2 = strtol(se2, &endptr, 10);
		if (errno != 0 || endptr == se2) {
	    printf("moleculeVersGraphe - edge #%d : error reading e2\n", i);
	    exit(1);
	  }
		ADDONEEDGE(g, e1-1, e2-1, m);
		if(line) free(line);
	}
	GRAPH_NAUTY gn = {.n = n, .m = m, .size=(m)*(n)*sizeof(graph), .g=g};
	return gn;
}

bool graphesIdentiques(GRAPH_NAUTY g1, GRAPH_NAUTY g2)
{
	if(g1.n != g2.n || g1.m != g2.m)
		return false;
	// Comparer les graphes g1 et g2, renvoyer true s'ils sont identiques, false sinon
	DEFAULTOPTIONS_GRAPH(options);
	options.getcanon = TRUE;
	options.defaultptn = TRUE;

	//ignored parameters
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLOC1(int,lab,lab_sz,g1.n,"malloc");
	DYNALLOC1(int,ptn,ptn_sz,g1.n,"malloc");
	DYNALLOC1(int,orbits,orbits_sz,g1.n,"malloc");
	statsblk stats;
	//store canonically labeled graphs
	DYNALLSTAT(graph,cg1,cg1_sz);
	DYNALLSTAT(graph,cg2,cg2_sz);
	DYNALLOC2(graph,cg1,cg1_sz,g1.m,g1.n,"malloc");
	DYNALLOC2(graph,cg2,cg2_sz,g2.m,g2.n,"malloc");

	densenauty(g1.g,lab,ptn,orbits,&options,&stats,g1.m,g1.n,cg1);
	densenauty(g2.g,lab,ptn,orbits,&options,&stats,g2.m,g2.n,cg2);

	bool is_isomorphic = true;
	for (int k = 0; k < g1.m*(size_t)g1.n; ++k)
		if (cg1[k] != cg2[k])
			is_isomorphic = false;
	return is_isomorphic;
}

int recupererIndiceFamille(GRAPH_NAUTY g, TABLEAUFAMILLES tf)
{
	for(int i = 0; i < tf.nbFamilles; i++)
	{
		if(graphesIdentiques(g, tf.familles[i].graphe))
		{
			return i;
		}
	}

	return -1;
}

void ajouterMoleculeDansFamille(TABLEAUFAMILLES tf, int indiceFamille, char *m)
{
	tf.familles[indiceFamille].nbMolecules++;
	tf.familles[indiceFamille].nomMolecules = realloc(tf.familles[indiceFamille].nomMolecules, tf.familles[indiceFamille].nbMolecules*sizeof(char*)) ;
	tf.familles[indiceFamille].nomMolecules[tf.familles[indiceFamille].nbMolecules - 1] = malloc((strlen(m)+1)*sizeof(char));
	strcpy(tf.familles[indiceFamille].nomMolecules[tf.familles[indiceFamille].nbMolecules - 1], m);
}

void nouvelleFamille(TABLEAUFAMILLES *tf, char *m, GRAPH_NAUTY g)
{
	FAMILLE f;

	f.graphe = g;
	f.nbMolecules = 1;
	f.nomMolecules = malloc(sizeof(char *));
	f.nomMolecules[f.nbMolecules - 1] = malloc((strlen(m)+1)*sizeof(char));
	strcpy(f.nomMolecules[f.nbMolecules - 1], m);

	tf->nbFamilles++;
	tf->familles = realloc(tf->familles, tf->nbFamilles * sizeof(FAMILLE));
	tf->familles[tf->nbFamilles - 1] = f;
}

TABLEAUFAMILLES classerMolecules(char *dir, char *ignore)
{
	printf("Enter classerMolecules\n");
	// Création du tableau de familles
	TABLEAUFAMILLES tf;
	tf.nbFamilles = 0;
	tf.familles = NULL;

	// Stockage du nombre total de molécules et du nom des fichiers de toutes les molécules
	int nbTotalMolecules = 0;
	char **nomsFichiers = tableauFichiers(&nbTotalMolecules, dir, ignore);

	// On boucle sur tous les fichiers
	for(int i = 0; i < nbTotalMolecules; i++)
	{
		// Ouverture du fichier de la molécule courante
		printf("File %d : %s\n", i, nomsFichiers[i]);
		char *filename = malloc(strlen(dir) + 1 + strlen(nomsFichiers[i]) + 1);
		sprintf(filename, "%s/%s", dir, nomsFichiers[i]);
		FILE *F = fopen(filename, "r");


		// Calcul du graphe de la molécule courante
		GRAPH_NAUTY g = moleculeVersGraphe(F);
		printf("n = %d vertices, m=%d edges\n", g.n, g.m);
		fclose(F);

		// On cherche l'indice de la famille à laquele appartient la molécule courante
		int indiceFamille = recupererIndiceFamille(g, tf);
		printf("indiceFamille=%d\n", indiceFamille);
		// Si l'indice ne vaut pas -1, on stock la molécule dans sa famille, sinon, on crée une famille
		if(indiceFamille != -1)
		{
			// Ajout de la molécule courante dans sa famille
			ajouterMoleculeDansFamille(tf, indiceFamille, nomsFichiers[i]);
		}

		else
		{
			// Création d'une nouvelle famille
			nouvelleFamille(&tf, nomsFichiers[i], g);
		}
		free(filename);
	}

	return tf;
}

void afficherFamilles(TABLEAUFAMILLES tf)
{
	for(int i = 0; i < tf.nbFamilles; i++)
	{
		printf("/// Famille %d ///\n", i);

		for(int j = 0; j < tf.familles[i].nbMolecules; j++)
		{
			printf("%s\n", tf.familles[i].nomMolecules[j]);
		}

		printf("\n");
	}
}

void libererMemoire(TABLEAUFAMILLES tf)
{

}
