/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Implementação em tarefa simples                                  *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#include <stdbool.h>

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

int *prim_mst(float *graph, int graph_size, int graph_root);
void print_mst(float *d, int *v_t, int graph_size);