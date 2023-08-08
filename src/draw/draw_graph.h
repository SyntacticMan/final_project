
#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

#include "gvc.h"

extern GVC_t *gvc;

void draw_graph(float *graph_to_draw, unsigned int graph_size, const char *filename, char *graph_title);