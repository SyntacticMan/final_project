#ifndef DRAW_GRAPH
#define DRAW_GRAPH
#include "draw_graph.h"
#endif

void draw_graph(void)
{
    Agraph_t *g;
    Agnode_t *n, *m;
    Agedge_t *e;
    // Agsym_t *a;
    GVC_t *gvc;
    printf("draw_graph\n");
    /* set up a graphviz context */
    gvc = gvContext();

    char *args[] = {
        "dot",
        /* gif output */
        "-Tpng",
        /* output to file abc.gif */
        "-oabc.png"};

    gvParseArgs(gvc, 3, args);

    /* Create a simple digraph */
    g = agopen("g", Agundirected, NULL);
    n = agnode(g, "n", 1);
    m = agnode(g, "m", 1);
    e = agedge(g, n, m, 0, 1);

    /* Set an attribute - in this case one that affects the visible rendering */
    // agsafeset(n, "color", "red", "");

    /* Compute a layout using layout engine from command line args */
    gvLayoutJobs(gvc, g);

    /* Write the graph according to -T and -o options */
    gvRenderJobs(gvc, g);

    /* Free layout data */
    gvFreeLayout(gvc, g);

    /* Free graph structures */
    agclose(g);
    /* close output file, free context, and return number of errors */
    gvFreeContext(gvc);
}