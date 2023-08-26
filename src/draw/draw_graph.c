#ifndef DRAW_GRAPH
#define DRAW_GRAPH
#include "draw_graph.h"
#endif

#include <assert.h>

GVC_t *gvc;

/*
    draw_graph

    cria uma representação gráfica do grafo em contexto
*/
void draw_graph(float *graph_to_draw, unsigned int graph_size, const char *filename, char *graph_title)
{
    igraph_t d_graph;

    /* Create a directed graph with no vertices or edges. */
    igraph_empty(&d_graph, 0, IGRAPH_UNDIRECTED);

    igraph_add_vertices(&d_graph, graph_size + 1, NULL);

    assert(igraph_vcount(&d_graph) == graph_size + 1);

    igraph_destroy(&d_graph);
}