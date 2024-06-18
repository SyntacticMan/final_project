/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Ferramenta para criar uma representação gráfica da AGM           *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef DRAW_GRAPH
#define DRAW_GRAPH
#include "draw_graph.h"
#endif

GVC_t *gvc;

/*
    draw_graph

    cria uma representação gráfica do grafo em contexto
*/
void draw_graph(float *graph, int graph_size, int graph_root, int edge_percentage, int *v_t, int vt_size)
{
    Agraph_t *g;
    Agnode_t *n_node, *m_node;
    Agedge_t *edge;
    char graph_title[50];
    char string_temp[50];

    // criar o contexto de visualização
    gvc = gvContext();

    // construir o nome da imagem a gerar
    // formato: draw_v<vertices>_p<percentagem arestas>.<ext>
    char image_name[200] = "-o ";
    strcat(image_name, "draw_v");
    sprintf(string_temp, "%d_", graph_size);
    strcat(image_name, string_temp);
    strcat(image_name, "p");
    sprintf(string_temp, "%d", edge_percentage);
    strcat(image_name, string_temp);
    strcat(image_name, ".svg");

    // preparar título
    sprintf(graph_title, "Grafo ");
    sprintf(string_temp, "%d vertices", graph_size);
    strcat(graph_title, string_temp);

    // se tiver AGM indicar no título
    if (vt_size > 0)
        strcat(graph_title, " (com AGM)");

#ifdef DEBUG
    printf("%s\n%s\n", image_name, graph_title);
#endif

    // definir algoritmo de colocação, tipo e nome da imagem a gerar
    char *args[] = {
        "neato",
        // "dot",
        "-Tsvg",
        "-Goverlap=scale",
        // "-Gsize=67!",
        // "-Goutputorder=nodesfirst",
        "-Gsplines=curved",
        // "-GK=3",
        // "-Gmaxiter=500",
        image_name};

    gvParseArgs(gvc, sizeof(args) / sizeof(char *), args);

    // criar o grafo não direccionado no formato graphviz
    g = agopen("g", Agundirected, NULL);

    // preparar título
    agsafeset(g, "label", graph_title, "");
    agsafeset(g, "labelloc", "t", "");

    // Definir atributos para evitar sobreposição
    agsafeset(g, "splines", "true", "");
    agsafeset(g, "overlap", "false", "");
    // agsafeset(g, "nodesep", "0.7", ""); // Separação entre nós
    // agsafeset(g, "ranksep", "1.0", ""); // Separação entre níveis hierárquicos

#ifdef DEBUG
    int count = 0;
#endif
    // criar os vértices
    for (int i = 1; i <= graph_size; i++)
    {
        sprintf(string_temp, "%d", i);
        n_node = agnode(g, string_temp, 1);
        agsafeset(n_node, "shape", "circle", ""); // círculos
        agsafeset(n_node, "style", "filled", ""); // Define que o nó deve ser preenchido
        agsafeset(n_node, "fillcolor", "white", "");

        // raíz fica com o texto de côr diferente
        if (i == graph_root)
        {
            agsafeset(n_node, "fontcolor", "red", ""); // texto a azul
        }
        else
        {
            agsafeset(n_node, "fontcolor", "blue", ""); // texto a azul
        }
#ifdef DEBUG
        count++;
#endif
    }

#ifdef DEBUG
    printf("%d nodes added\n", count);

    count = 0;
#endif

    // adicionar as arestas
    for (int col = 2; col <= graph_size; col++)
    {
        for (int row = 1; row <= graph_size; row++)
        {
            if (col < row)
                continue;

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
                // sprintf(string_temp, "%3.3f", weight);
                // agsafeset(edge, "label", string_temp, "");
                // agsafeset(edge, "fontsize", "8", "");

                // se fizer parte da MST então a aresta será mais grossa

                if (vt_size > 0 && (v_t[col] == row || v_t[row] == col))
                {
                    agsafeset(edge, "penwidth", "1.0", "");
                }
                else
                {
                    agsafeset(edge, "penwidth", "0.5", "");
                    agsafeset(edge, "color", "#808080B3", "");
                }
#ifdef DEBUG
                count++;
#endif
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