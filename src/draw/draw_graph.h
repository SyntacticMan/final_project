
#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

#include "gvc.h"

GVC_t *gvc;

void draw_graph(int **graph_to_draw, int graph_size, const char *filename, char *graph_title);