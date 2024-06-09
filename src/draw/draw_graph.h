/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Ferramenta para criar uma representação gráfica da AGM           *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

#include <graphviz/gvc.h>
// #include <igraph/igraph.h>
// #include <cairo/cairo.h>

// extern GVC_t *gvc;

void draw_graph(float *graph, int graph_size, int graph_root, int edge_percentage, int *v_t, int vt_size);