/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Implementação em multi tarefa                                    *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef LIBS
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

#define MAX_THREADS 8

int *prim_mt_mst(float *graph, int graph_size, int graph_root, int num_threads);