// LIBRAIRIES
#include "famille.h"
#include "custom_getline.h"
#include <errno.h>
// FONCTIONS
char **tableauFichiers(int *nbTotalMolecules, char *dir, char *ignore)
{
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
			//printf("tableauFichiers : adding file %s\n", dp->d_name);
			allfiles[k] = malloc(strlen(dp->d_name)+1);
			if(allfiles[k] == NULL){
				printf("tableauFichiers : error while malloc'ing\n");
			}
			memcpy(allfiles[k], dp->d_name, strlen(dp->d_name)+1);
			//printf("%s\n", allfiles[k]);
			//printf("copied");

			//printf("%d\n", k);
			k++;
	  }

	  closedir(dfd);

    return allfiles;
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
	char **colors = malloc(n*sizeof(char*));
	for(int i = 0; i<n; ++i){
		colors[i] = malloc(4);
		line = NULL;
		l = 0;
		if(custom_getline(&line, &l, F) == -1){
			printf("moleculeVersGraphe - skip atoms : read error\n");
			exit(1);
		}
		memcpy(colors[i], &line[31], 3);
		colors[i][3] = 0;
		//printf("%s\n", colors[i]);
		//printf("atom %d : color %s\n", i, colors[i]);
		if(line) free(line);
	}
	//create graph & read edges
	int nauty_m = SETWORDSNEEDED(n);
	graph *g = ALLOCS((nauty_m),(n)*sizeof(graph));
	EMPTYGRAPH(g,nauty_m,n);
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
		ADDONEEDGE(g, e1-1, e2-1, nauty_m);
		if(line) free(line);
	}
	GRAPH_NAUTY gn = {.n = n,
										.m = m,
			              .nauty_m = nauty_m,
										.size=(nauty_m)*(n)*sizeof(graph),
										.g=g,
										.colors = colors,
									 };
	return gn;
}

bool graphesIdentiques(GRAPH_NAUTY g1, GRAPH_NAUTY g2)
{
	if(g1.n != g2.n || g1.m != g2.m)
		return false;
	if(g1.m == 0)
		return true;
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
	DYNALLOC2(graph,cg1,cg1_sz,g1.nauty_m,g1.n,"malloc");
	DYNALLOC2(graph,cg2,cg2_sz,g2.nauty_m,g2.n,"malloc");

	densenauty(g1.g,lab,ptn,orbits,&options,&stats,g1.nauty_m,g1.n,cg1);
	densenauty(g2.g,lab,ptn,orbits,&options,&stats,g2.nauty_m,g2.n,cg2);

	//compare canonically labeled graphs
	bool is_isomorphic = true;
	for (int k = 0; k < g1.nauty_m*(size_t)g1.n; ++k)
		if (cg1[k] != cg2[k])
			is_isomorphic = false;

	DYNFREE(lab, lab_sz);
	DYNFREE(ptn, ptn_sz);
	DYNFREE(orbits, orbits_sz);
	DYNFREE(cg1, cg1_sz);
	DYNFREE(cg2, cg2_sz);
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

char **colorMapping(GRAPH_NAUTY to_map, FAMILLE ref){
	GRAPH_NAUTY g1 = to_map;
	GRAPH_NAUTY g2 = ref.graphe;
	if(g1.n != g2.n || g1.m != g2.m)
		return NULL;
	//TODO : il y a un problème interne à nauty avec les graphes à 0 arêtes, à investiguer
	//prolly fixed tho
	/*
	if(g1.m == 0){
		return NULL;
	}*/
	// Comparer les graphes g1 et g2, renvoyer true s'ils sont identiques, false sinon
	DEFAULTOPTIONS_GRAPH(options);
	options.getcanon = TRUE;
	options.defaultptn = TRUE;

	DYNALLSTAT(int,lab1,lab1_sz);
	DYNALLSTAT(int,lab2,lab2_sz);
	DYNALLOC1(int,lab1,lab1_sz,g1.n,"malloc");
	DYNALLOC1(int,lab2,lab2_sz,g1.n,"malloc");
	//ignored parameters
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLOC1(int,ptn,ptn_sz,g1.n,"malloc");
	DYNALLOC1(int,orbits,orbits_sz,g1.n,"malloc");
	statsblk stats;
	//store canonically labeled graphs
	DYNALLSTAT(graph,cg1,cg1_sz);
	DYNALLSTAT(graph,cg2,cg2_sz);
	DYNALLOC2(graph,cg1,cg1_sz,g1.nauty_m,g1.n,"malloc");
	DYNALLOC2(graph,cg2,cg2_sz,g2.nauty_m,g2.n,"malloc");

	densenauty(g1.g,lab1,ptn,orbits,&options,&stats,g1.nauty_m,g1.n,cg1);
	densenauty(g2.g,lab2,ptn,orbits,&options,&stats,g2.nauty_m,g2.n,cg2);

	//compare canonically labeled graphs
	bool is_isomorphic = true;
	for (int k = 0; k < g1.nauty_m*(size_t)g1.n; ++k)
		if (cg1[k] != cg2[k])
			is_isomorphic = false;

	if(!is_isomorphic)
		return NULL;

	char **res = malloc(g1.n*sizeof(char*));
	for(int i = 0; i<g1.n; i++)
		res[i] = malloc(4);

	for(int i = 0; i<g1.n; i++)
		memcpy(res[lab2[i]], g1.colors[lab1[i]], 4);

	DYNFREE(lab1, lab1_sz);
	DYNFREE(lab2, lab2_sz);
	DYNFREE(ptn, ptn_sz);
	DYNFREE(orbits, orbits_sz);
	DYNFREE(cg1, cg1_sz);
	DYNFREE(cg2, cg2_sz);
	return res;
}

void ajouterMoleculeDansFamille(TABLEAUFAMILLES tf, int indiceFamille, char *m, GRAPH_NAUTY g)
{
	FAMILLE *f = &tf.familles[indiceFamille];
	f->nbMolecules++;
	f->nomMolecules = realloc(f->nomMolecules, f->nbMolecules*sizeof(char*));
	f->nomMolecules[f->nbMolecules - 1] = malloc((strlen(m)+1)*sizeof(char));
	strcpy(f->nomMolecules[f->nbMolecules - 1], m);

	f->molecules = realloc(f->molecules, f->nbMolecules*sizeof(MOLECULE_FAMILLE));
	f->molecules[f->nbMolecules - 1].nomMolecule = malloc((strlen(m)+1)*sizeof(char));
	strcpy(f->molecules[f->nbMolecules - 1].nomMolecule, m);
	f->molecules[f->nbMolecules -1].colors = colorMapping(g, *f);
}

void nouvelleFamille(TABLEAUFAMILLES *tf, char *m, GRAPH_NAUTY g)
{
	FAMILLE f;
	MOLECULE_FAMILLE *mols = malloc(sizeof(MOLECULE_FAMILLE));

	mols->nomMolecule = malloc((strlen(m)+1)*sizeof(char));
	strcpy(mols->nomMolecule, m);
	mols->colors = malloc(g.n*sizeof(char*));
	for(int i = 0; i<g.n; ++i){
		mols->colors[i] = malloc(4);
		memcpy(mols->colors[i], g.colors[i], 4);
	}
	f.molecules = mols;

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
		char *filename = malloc(strlen(dir) + 1 + strlen(nomsFichiers[i]) + 1);
		sprintf(filename, "%s/%s", dir, nomsFichiers[i]);
		FILE *F = fopen(filename, "r");

		int n = strlen(nomsFichiers[i]);
		nomsFichiers[i][n-4] = 0;

		// Calcul du graphe de la molécule courante
		GRAPH_NAUTY g = moleculeVersGraphe(F);
		fclose(F);

		// On cherche l'indice de la famille à laquele appartient la molécule courante
		int indiceFamille = recupererIndiceFamille(g, tf);
		// Si l'indice ne vaut pas -1, on stock la molécule dans sa famille, sinon, on crée une famille
		if(indiceFamille != -1)
		{

			// Ajout de la molécule courante dans sa famille
			ajouterMoleculeDansFamille(tf, indiceFamille, nomsFichiers[i], g);
			// Le graphe est déjà stocké dans la famille, il faut le free.
			freeGrapheNauty(g);
		}

		else
		{
			// Création d'une nouvelle famille
			nouvelleFamille(&tf, nomsFichiers[i], g);
		}
		free(filename);
		free(nomsFichiers[i]);
	}

	printf("Nombre de familles trouvées : %d\n", tf.nbFamilles);
	free(nomsFichiers);
	return tf;
}

void afficherFamilles(TABLEAUFAMILLES tf)
{
	for(int i = 0; i < tf.nbFamilles; i++)
	{
		printf("/// Famille %d ///\n", i);

		for(int j = 0; j < tf.familles[i].nbMolecules; j++)
		{
			printf("%s", tf.familles[i].molecules[j].nomMolecule);
			if(tf.familles[i].molecules[j].colors){
				printf(" colors : ");
				for(int k = 0; k<tf.familles[i].graphe.n; ++k){
					printf("%s ", tf.familles[i].molecules[j].colors[k]);
				}
			}
			printf("\n");
		}

		printf("\n");
	}
}

void freeGrapheNauty(GRAPH_NAUTY gn){
	free(gn.g);
	if(gn.colors){
		for(int i = 0; i < gn.n; ++i)
			free(gn.colors[i]);
		free(gn.colors);
	}
}

void freeMoleculeFamille(MOLECULE_FAMILLE mf, int n){
	free(mf.nomMolecule);
	for(int i = 0; i<n; ++i)
		free(mf.colors[i]);
	free(mf.colors);
}

void freeFamille(FAMILLE f){
	freeGrapheNauty(f.graphe);
	for(int i = 0; i<f.nbMolecules; ++i){
		if(f.nomMolecules) free(f.nomMolecules[i]);
		freeMoleculeFamille(f.molecules[i], f.graphe.n);
	}
	if(f.nomMolecules) free(f.nomMolecules);
	free(f.molecules);
}

void libererMemoire(TABLEAUFAMILLES tf)
{
	for(int i = 0; i < tf.nbFamilles; i++){
		freeFamille(tf.familles[i]);
	}
	free(tf.familles);
}
