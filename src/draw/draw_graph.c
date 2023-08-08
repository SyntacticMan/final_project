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
    Agnode_t *n, *m;
    Agedge_t *e;
    char string_temp[50];

    // criar o contexto de visualização
    gvc = gvContext();

    // preparar a opção com o nome do ficheiro a gerar
    char image_name[50] = "-o";
    strcat(image_name, filename);

    // definir algoritmo de colocação, tipo e nome da imagem a gerar
    char *args[] = {
        "dot",
        "-Tpng",
        image_name};

    gvParseArgs(gvc, sizeof(args) / sizeof(char *), args);

    // criar o grafo, com disciplina por omissão
    g = agopen("g", Agundirected, NULL);

    agsafeset(g, "label", graph_title, "");
    agsafeset(g, "labelloc", "t", "");

    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row < col; row++)
        {
            // printf("col: %d, row: %d\n", col, row);

            sprintf(string_temp, "%d", col);
            n = agnode(g, string_temp, 1);
            agsafeset(n, "shape", "circle", "");

            sprintf(string_temp, "%d", row);
            m = agnode(g, string_temp, 1);
            agsafeset(m, "shape", "circle", "");

            float weight = get_edge(graph_to_draw, col, row);
            if (weight != INFINITE && weight > 0)
            {
                // printf("weight: %3.3f\n", weight);

                e = agedge(g, n, m, 0, 1);
                sprintf(string_temp, "%3.3f", weight);
                agsafeset(e, "label", string_temp, "");
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