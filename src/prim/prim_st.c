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

    // raíz do grafo começa com peso 0
    int root_weight = 0;

    // marcar vértice com tendo sido visitado
    int visited = 1;
    v_t[graph_root] = &visited;

    // obter todas as arestas que pertencem à raíz
    int infinite = -1;
    for (int v = 0; v < graph_size; v++)
    {
        if (v != graph_root)
        {
            // cada peso tem de ser novamente alocado
            int *edge_weight = (int *)malloc(sizeof(int));
            *edge_weight = get_edge(graph, graph_root, v);

            if (*edge_weight > 0)
            {
                add_edge(v_t, graph_root, v, *edge_weight);
            }
            else
            {
                add_null_edge(v_t, graph_root, v);
            }
        }
    }

    // int visited_count = 1;
    // for (int u = 0; u < graph_size; u++)
    // {
    //     // passar a raíz à frente
    //     if (u == graph_root)
    //         continue;

    //     int min_edge_weight = MAX_WEIGHT + 1;
    //     for (int v = 0; v < graph_size; v++)
    //     {

    //         int edge_weight = get_edge(graph, u, v);

    //         if (edge_weight != infinite && edge_weight < min_edge_weight)
    //         {
    //             min_edge_weight = edge_weight;
    //         }
    //     }
    //     // add_edge(v_t, u, v, edge_weight);
    //     printf("g> %d\n", *graph[u]);
    //     // printf("v_t> %d\n", *v_t[u]);

    //     // int u = 0;

    //     // // v_t[u] = &visited;
    //     // u++;
    //     // visited_count++;

    //     // for (int v = 0; v < visited_count; v++)
    //     // {
    //     //     int weight = get_edge(graph, u, v);

    //     //     if ((u == -1 || weight < *d[v]))
    //     //     {
    //     //         d[v] = &weight;
    //     //     }
    //     // }
    // }

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