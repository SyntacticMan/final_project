#ifndef DRAW_GRAPH
#define DRAW_GRAPH
#include "draw_graph.h"
#endif

GVC_t *gvc;

/*
    draw_graph

    cria uma representação gráfica do grafo em contexto
*/
void draw_graph(float *graph_to_draw, unsigned int graph_size, const char *filename, char *graph_title)
{
    Agraph_t *g;
    Agnode_t *n_node, *m_node;
    Agedge_t *edge;
    char string_temp[50];

    // criar o contexto de visualização
    gvc = gvContext();

    // preparar a opção com o nome do ficheiro a gerar
    char image_name[50] = "-o";
    strcat(image_name, filename);

    // definir algoritmo de colocação, tipo e nome da imagem a gerar
    char *args[] = {
        "sfdp",
        "-Tsvg",
        "-Goverlap=prism",
        "-Gsize=67!",
        "-Goutputorder=nodesfirst",
        "-Gsplines=curved",
        "-GK=3000",
        // "-Gmaxiter=100",
        image_name};

    gvParseArgs(gvc, sizeof(args) / sizeof(char *), args);

    // criar o grafo, com disciplina por omissão
    g = agopen("g", Agundirected, NULL);

    agsafeset(g, "label", graph_title, "");
    agsafeset(g, "labelloc", "t", "");
    agsafeset(g, "ranksep", "0.9", "");
    agsafeset(g, "nodesep", "0.5", "");

    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row < col; row++)
        {
            // printf("col: %d, row: %d\n", col, row);

            sprintf(string_temp, "%d", col);
            n_node = agnode(g, string_temp, 1);
            agsafeset(n_node, "shape", "circle", "");
            agsafeset(n_node, "fontcolor", "blue", "");

            sprintf(string_temp, "%d", row);
            m_node = agnode(g, string_temp, 1);
            agsafeset(m_node, "shape", "circle", "");
            agsafeset(m_node, "fontcolor", "blue", "");

            float weight = get_edge(graph_to_draw, col, row);
            if (weight != INFINITE && weight > 0)
            {
                // printf("weight: %3.3f\n", weight);

                edge = agedge(g, n_node, m_node, 0, 1);
                sprintf(string_temp, "%3.3f", weight);
                agsafeset(edge, "label", string_temp, "");
                agsafeset(edge, "fontsize", "8", "");
                agsafeset(edge, "penwidth", "0.5", "");
                agsafeset(edge, "color", "#808080B3", ""); // cinzento claro, 80% transparência
            }
        }
    }

    // calcular disposição
    gvLayoutJobs(gvc, g);

    // criar o grafo
    gvRenderJobs(gvc, g);

    // libertar memória
    gvFreeLayout(gvc, g);
    agclose(g);

    // encerrar e terminar o contexto
    gvFreeContext(gvc);
}