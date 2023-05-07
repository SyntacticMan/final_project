/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Biblioteca de grafos												*
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
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

extern int **v_t;

// vetor com a árvore geradora mínima
extern int **d;

void create_graph(int graph_size, int edge_percentage);

int get_array_size(int graph_size);

int get_edge(int u, int v);
// qq coisa get_all_edges(int v) -> provavelmente um array
// move_vertive(int u) -> move vertice v from graph to v_t
int get_edge_count();
int get_max_edge_count(int graph_size);
float get_edge_percentage(int graph_size);

int pick_graph_root(int graph_size);

void print_graph(int graph_size);