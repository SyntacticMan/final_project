#ifndef DRAW_GRAPH
#include "draw_graph.h"
#endif

/*
    draw_graph

    cria uma representação gráfica do grafo em contexto
*/
void draw_graph(int **graph_to_draw, int graph_size, const char *filename, char *graph_title)
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

    for (int col = 0; col < graph_size; col++)
    {
        for (int row = 0; row < graph_size; row++)
        {
            // apenas desenhar a triangular superior
            // para evitar duplicação de ligações
            if (row > col)
            {

                sprintf(string_temp, "%d", col);
                n = agnode(g, string_temp, 1);
                agsafeset(n, "shape", "circle", "");

                sprintf(string_temp, "%d", row);
                m = agnode(g, string_temp, 1);
                agsafeset(m, "shape", "circle", "");

                int weight = get_edge(graph_to_draw, col, row);
                if (weight != INFINITE && weight > 0)
                {
                    e = agedge(g, n, m, 0, 1);
                    sprintf(string_temp, "%d", weight);
                    agsafeset(e, "label", string_temp, "");
                }
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