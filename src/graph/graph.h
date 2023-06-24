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
#include <time.h>

#define MAX_EDGE 3
#define MIN_EDGE 1

#define MAX_WEIGHT 8
#define MIN_WEIGHT 1

#define GRAPH_SIZE_MIN 10
#define GRAPH_SIZE_MAX 50

#define INFINITE MAX_WEIGHT + 2

// grafo é representado como um vetor de pointeiros dinamicamente alocado de ints
// uma vez que o tamanho do grafo é determinado durante a execução
extern int **graph;

void create_graph(int graph_size, int edge_percentage);
void create_locked_graph(int graph_size, int edge_percentage);
void add_edge(int **graph, int u, int v, int weight);
void add_null_edge(int **graph, int u, int v);
int get_array_size(int graph_size);

int get_edge(int **graph, int u, int v);
int get_edge_count();
int get_max_edge_count(int graph_size);
float get_edge_percentage(int graph_size);

int pick_graph_root(int graph_size);

void print_graph(int graph_size);
