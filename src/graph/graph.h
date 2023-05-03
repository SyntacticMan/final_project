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
extern int **graph;

// vetor com a árvore geradora mínima
// int **d;

void create_graph(int graph_size, int edge_percentage);

int get_array_size(int graph_size);

int get_edge(int u, int v);
int get_edge_count();
int get_max_edge_count(int graph_size);
float get_edge_percentage(int graph_size);

void print_graph(int graph_size);