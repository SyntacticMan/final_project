/********************************************************************
 *  Módulo graph                                                    *
 *                                                                  *
 *  Constrói um grafo com base nos valores    						*
 *  indicados nos #define                                           *
 * ******************************************************************/

#include <stdlib.h>
#include <stdio.h>

#define MAX_EDGE 3
#define MIN_EDGE 1

#define MAX_WEIGHT 15
#define MIN_WEIGHT 1

#define GRAPH_SIZE_MIN 10
#define GRAPH_SIZE_MAX 50

// grafo é representado como um vetor de pointeiros dinamicamente alocado de ints
// uma vez que o tamanho do grafo é determinado durante a execução
// int **graph;

// vetor com a árvore geradora mínima
// int **d;

int **create_graph();

int get_col_index(int col);
int get_row_index(int col_index, int row);
// vertice *add_vertice(int n);

void add_edge(int **graph, int u, int v);
// int check_edge(edge *edge_head, int vertice);

int random_generator(int max, int min);