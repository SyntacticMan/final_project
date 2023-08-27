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
    igraph_matrix_t *m;

    /* Create a directed graph with no vertices or edges. */
    igraph_empty(&d_graph, 0, IGRAPH_UNDIRECTED);

    igraph_add_vertices(&d_graph, graph_size + 1, NULL);

    printf("Adding edges\n");
    // adicionar as arestas

    int edge_count = 0;
    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row <= graph_size; row++)
        {
            float weight = get_edge(graph_to_draw, col, row);

            if (weight < INFINITE && weight > 0)
            {
                igraph_add_edge(&d_graph, col, row);
                edge_count++;
            }
        }
    }

    printf("edge count %d\n", edge_count);

    assert(igraph_vcount(&d_graph) == graph_size + 1);
    assert(igraph_ecount(&d_graph) == edge_count);

    printf("Preparing layout\n");

    igraph_matrix_init(
        m, graph_size, graph_size);

    igraph_layout_graphopt(&d_graph, m,
                           500,  // iterações a fazer
                           0.01, // node_charge
                           300,  // node_mass
                           2,    // spring_length
                           1,    // spring_constant
                           15,   // max_sa_movement
                           0     // false
    );

    printf("Creating image \n");

    // Create a Cairo surface (image)
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1000, 1000);
    cairo_t *cr = cairo_create(surface);

    for (int i = 0; i < igraph_vcount(&d_graph); ++i)
    {
        double x = MATRIX(*m, i, 0);
        double y = MATRIX(*m, i, 1);
        cairo_arc(cr, x, y, 5, 0, 2 * M_PI);
        cairo_fill(cr);

        // Draw label
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 10);
        cairo_move_to(cr, x - 14 / 2, y - 15 / 2);
        cairo_show_text(cr, "Node");
    }

    igraph_integer_t from;
    igraph_integer_t to;

    for (int i = 0; i < igraph_ecount(&d_graph); ++i)
    {
        // printf("%d\n", i);
        // igraph_edge(&d_graph, i, &from, &to);
        // double x1 = MATRIX(*m, from, 0);
        // double y1 = MATRIX(*m, from, 1);
        // double x2 = MATRIX(*m, to, 0);
        // double y2 = MATRIX(*m, to, 1);

        // cairo_move_to(cr, x1, y1);
        // cairo_line_to(cr, x2, y2);
        // cairo_stroke(cr);
    }

    // Export the Cairo image to a PNG file
    cairo_surface_write_to_png(surface, "output.png");

    // igraph_write_graph_dot(&d_graph, stdout);
    igraph_destroy(&d_graph);
}