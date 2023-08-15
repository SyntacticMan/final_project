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

float *prim_mst(int array_size, int graph_size, int graph_root)
{
    int *v_t = malloc(graph_size * sizeof(int));
    float *d = malloc(graph_size * sizeof(float));

    // int *visited = malloc(graph_size * sizeof(int));

    v_t[graph_root] = graph_root;
    d[graph_root] = 0;

    print_array(graph, graph_size);

    // inicializar a árvore mínima
    for (int v = 1; v <= graph_size; v++)
    {
        // inicializar os vértices visitados
        // visited[v] = false;

        float weight = get_edge(graph, graph_root, v);

        if (weight < INFINITE)
        {
            // visited[v] = true;
            printf("v: %d, w:%f\n", v, weight);
            d[v] = weight;
            v_t[v] = graph_root;
        }
        else
        {
            d[v] = INFINITE;
            v_t[v] = 0;
        }
    }

    // // processar as arestas da raíz
    // for (int v = 0; v < graph_size; v++)
    // {
    //     int edge_weight = get_edge(graph, graph_root, v);
    //     if (edge_weight != INFINITE)
    //     {
    //         d[v] = edge_weight;
    //     }
    // }

    // // encontrar u
    // for (int v = 0; v < graph_size; v++)
    // {
    //     // passar a raíz à frente
    //     if (v == graph_root)
    //         continue;

    //     // processar apenas se ainda não tiver sido visitado
    //     if (!visited[v])
    //     {
    //         for (int u = 0; u < graph_size; u++)
    //         {
    //             if (v == u)
    //                 continue;

    //             int edge_weight = get_edge(graph, v, u);

    //             if (edge_weight < d[u])
    //             {
    //                 d[u] = edge_weight;
    //                 visited[u] = true;
    //             }
    //         }
    //     }
    // }
    /* TODO: pôr aqui um array que guarde os nós a que correspondem os d[u]*/
    // free(visited);
    return d;
}