/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Implementação em tarefa simples                                  *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef PRIM_ST
#define PRIM_ST
#include "prim_st.h"
#endif

int **prim_mst(int array_size, int graph_size, int graph_root)
{
    // vetor d e v_t são inicializados com o mesmo tamanho que o grafo
    int **v_t = (int **)malloc(get_array_size(graph_size) * sizeof(int *));
    int *visited = malloc(graph_size * sizeof(int *));
    int *d = malloc(graph_size * sizeof(int *));

    // raíz do grafo começa com peso 0
    d[graph_root] = 0;
    visited[graph_root] = 1;

    // inicializar a árvore mínima
    for (int v = 0; v < graph_size; v++)
    {
        for (int u = 0; u < graph_size; u++)
        {
            add_null_edge(v_t, v, u);
        }
    }

    for (int v = 0; v < graph_size; v++)
    {
        if (v != graph_root)
        {
            int edge_weight = get_edge(graph, graph_root, v);

            // se tiver uma ligação, adicionar
            if (edge_weight > 0)
            {
                d[v] = edge_weight;
                add_edge(v_t, graph_root, v, edge_weight);
            }
            else
            {
                d[v] = INFINITE;
                add_null_edge(v_t, graph_root, v);
            }
        }
    }

    for (int v = 0; v < graph_size; v++)
    {
        int min_edge_weight = MAX_WEIGHT + 1;
        int min_vertex = 0;

        for (int u = 0; u < graph_size; u++)
        {
            // passar a raíz à frente
            // if (u == graph_root)
            //     continue;

            if (u == v)
            {
                // add_edge(v_t, u, v, 0);
                continue;
            }

            if (visited[u] == 1)
                continue;

            int edge_weight = get_edge(graph, u, v);

            if (edge_weight != INFINITE)
            {
                if (edge_weight > 0 && edge_weight < d[u])
                {
                    min_vertex = u;
                    d[u] = edge_weight;
                }
            }
            else
            {
                add_null_edge(v_t, u, v);
            }
        }
        visited[v] = 1;
        add_edge(v_t, min_vertex, v, d[min_vertex]);
    }

    // emitir a árvore geradora mínima

    print_graph(graph_size);
    printf("\nÁrvore mínima \n");

    graph = v_t;
    print_graph(graph_size);
    // for (int i = 0; i < graph_size; i++)
    // {
    //     // if (*d[i] > 0)
    //     printf(" vertice: %d peso: %d | ", i, *d[i]); // debug
    //                                                   // printf("=> %d", i);
    // }
    printf("\n");

    return v_t;
}