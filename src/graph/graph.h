/********************************************************************
 *  Módulo graph                                                    *
 *                                                                  *
 *  Constrói um grafo com base nos valores    						*
 *  indicados nos #define                                           *
 * ******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_EDGE 3
#define MIN_EDGE 1

#define MAX_WEIGHT 15
#define MIN_WEIGHT 1

#define GRAPH_SIZE_MIN 10
#define GRAPH_SIZE_MAX 50

// grafo é representado como um vetor de pointeiros dinamicamente alocado de ints
// uma vez que o tamanho do grafo é determinado durante a execução
int **graph;

// vetor com a árvore geradora mínima
// int **d;

void create_graph(int graph_size, int edge_percentage);

int get_index(int col, int row);
int get_array_size(int graph_size);

void add_edge(int u, int v);
void add_null_edge(int u, int v);

int random_generator(int max, int min);