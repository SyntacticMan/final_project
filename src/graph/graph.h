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

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#define MAX_EDGE 3
#define MIN_EDGE 1

#define MAX_WEIGHT 8.0
#define MIN_WEIGHT 1.0
#define SCALE_FACTOR 1000

#define GRAPH_SIZE_MIN 10
#define GRAPH_SIZE_MAX 50

#define INFINITE MAX_WEIGHT + 2.0
#define THREADS 1

unsigned short int *create_graph(int graph_size, int edge_percentage);
unsigned short int *create_locked_graph();
void add_edge(int col, int row, float weight);
void add_null_edge(int col, int row);
unsigned long long get_matrix_size(int graph_size);

float get_edge(unsigned short int *graph, int col, int row);
int get_edge_count(unsigned short int *graph, int graph_size);
double get_edge_percentage(unsigned short int *graph, int graph_size);
unsigned long long get_index(int col, int row);

int pick_graph_root(int graph_size);

void print_graph(unsigned short int *graph, int graph_size);
void print_graph_mt(unsigned short int *graph, int start_col, int end_col, int graph_size);
void print_array(unsigned short int *graph, int graph_size);
