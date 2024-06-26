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
#include <stdint.h>

#define MAX_EDGE 3
#define MIN_EDGE 1

#define MAX_WEIGHT 8.0
#define MIN_WEIGHT 1.0

#define GRAPH_SIZE_MIN 10
#define GRAPH_SIZE_MAX 50

#define INFINITE MAX_WEIGHT + 2.0

float *create_graph(int graph_size, int edge_percentage);
float *create_locked_graph();
void add_edge(float *graph, int col, int row, float weight);
void add_null_edge(float *graph, int col, int row);
unsigned long long get_matrix_size(int graph_size);

float get_edge(float *graph, int col, int row);
int get_edge_count(float *graph, int graph_size);
double get_edge_percentage(float *graph, int graph_size);
unsigned long long get_index(int col, int row);

int pick_graph_root(int graph_size);

void print_graph(float *graph, int graph_size);
void print_graph_mt(float *graph, int start_col, int end_col, int graph_size);
void print_array(float *graph, int graph_size);
