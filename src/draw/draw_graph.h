
#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

#include <graphviz/gvc.h>
// #include <igraph/igraph.h>
// #include <cairo/cairo.h>

// extern GVC_t *gvc;

void draw_graph(float *graph, unsigned int graph_size, const char *filename, char *graph_title);