#ifndef DRAW_GRAPH
#define DRAW_GRAPH
#include "draw_graph.h"
#endif

#include <assert.h>

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

    // Create a Cairo surface (image)
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 2000, 2000);
    cairo_t *cr = cairo_create(surface);

    // Export the Cairo image to a PNG file
    cairo_surface_write_to_png(surface, "output.png");
    // igraph_write_graph_cairo(&d_graph, IGRAPH_CAIRO_FORMAT_PNG, "output.png", IGRAPH_DRAW_ALL);

    igraph_destroy(&d_graph);
}