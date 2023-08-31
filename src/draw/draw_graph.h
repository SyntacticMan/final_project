
#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

#include <graphviz/gvc.h>
// #include <igraph/igraph.h>
// #include <cairo/cairo.h>

// extern GVC_t *gvc;

void draw_graph(float *graph, int graph_size, int graph_root, int *v_t, int vt_size, const char *filename, char *graph_title);