#ifndef DRAW_GRAPH
#define DRAW_GRAPH
#include "draw_graph.h"
#endif

GVC_t *gvc;

/*
    draw_graph

    cria uma representação gráfica do grafo em contexto
*/
void draw_graph(float *graph, int graph_size, int graph_root, int *v_t, int vt_size, const char *filename, char *graph_title)
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
        "neato",
        "-Tsvg",
        "-Goverlap=scale",
        // "-Gsize=67!",
        // "-Goutputorder=nodesfirst",
        // "-Gsplines=curved",
        // "-GK=3",
        // "-Gmaxiter=500",
        image_name};

    gvParseArgs(gvc, sizeof(args) / sizeof(char *), args);

    // criar o grafo não direccionado no formato graphviz
    g = agopen("g", Agundirected, NULL);

    // preparar título
    agsafeset(g, "label", graph_title, "");
    agsafeset(g, "labelloc", "t", "");
    // agsafeset(g, "ranksep", "0.9", "");
    // agsafeset(g, "nodesep", "0.5", "");

    int count = 0;
    // criar os vértices
    for (int i = 1; i <= graph_size; i++)
    {
        sprintf(string_temp, "%d", i);
        n_node = agnode(g, string_temp, 1);
        agsafeset(n_node, "shape", "circle", ""); // círculos

        // raíz fica com o texto a preto
        if (i == graph_root)
        {
            agsafeset(n_node, "fontcolor", "black", ""); // texto a azul
        }
        else
        {
            agsafeset(n_node, "fontcolor", "blue", ""); // texto a azul
        }
        count++;
    }

#ifdef DEBUG
    printf("%d nodes added\n", count);
#endif

    count = 0;
    for (int col = 2; col <= graph_size; col++)
    {
        for (int row = 1; row < col; row++)
        {
            // procurar os vértices, por nome
            sprintf(string_temp, "%d", col);
            n_node = agnode(g, string_temp, 0);
            sprintf(string_temp, "%d", row);
            m_node = agnode(g, string_temp, 0);

            // e criar uma aresta, se tiver ligação
            float weight = get_edge(graph, col, row);
            if (weight < INFINITE && weight > 0)
            {
                edge = agedge(g, n_node, m_node, 0, 1);

                // as arestas têm o seu peso inscrito como label
                sprintf(string_temp, "%3.3f", weight);
                agsafeset(edge, "label", string_temp, "");
                agsafeset(edge, "fontsize", "8", "");

                // se fizer parte da MST então a aresta será mais grossa
                if (v_t[col] == row || v_t[row] == col)
                {
                    agsafeset(edge, "penwidth", "1.0", "");
                }
                else
                {
                    agsafeset(edge, "penwidth", "0.5", "");
                    agsafeset(edge, "color", "#808080B3", "");
                }

                count++;
            }
        }
    }

#ifdef DEBUG
    printf("%d edges added\n", count);

    // escrever o grafo num ficheiro dot
    FILE *dotFile = fopen("graph_output.dot", "w");
    if (dotFile)
    {
        agwrite(g, dotFile);
        fclose(dotFile);
    }
    else
    {
        printf("Error opening file for writing.\n");
    }
#endif

    // calcular disposição
    gvLayoutJobs(gvc, g);
#ifdef DEBUG
    printf("Layout done\n");
#endif
    // criar o grafo
    gvRenderJobs(gvc, g);

#ifdef DEBUG
    printf("Render done\n");
#endif
    // libertar memória
    gvFreeLayout(gvc, g);
    agclose(g);

    // encerrar e terminar o contexto
    gvFreeContext(gvc);
}