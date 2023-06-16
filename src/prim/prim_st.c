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
    int **d = (int **)malloc(graph_size * sizeof(int *));
    int **v_t = calloc(graph_size, sizeof(int *));

    // raíz do grafo começa com peso 0
    int root_weight = 0;
    d[graph_root] = &root_weight;

    // marcar vértice com tendo sido visitado
    int visited = 1;
    v_t[graph_root] = &visited;

    // obter todas as arestas que pertencem à raíz
    int infinite = -1;
    for (int v = 0; v < graph_size; v++)
    {
        if (v != graph_root)
        {
            int edge_weight = get_edge(graph, v, graph_root);
            if (edge_weight > 0)
            {
                d[v] = &edge_weight;
            }
            else
            {
                d[v] = &infinite;
            }
        }
    }

    // int visited_count = 1;
    // while (visited_count < graph_size - 1)
    // {
    //     int u = 0;

    //     for (int v = 0; v < visited_count; v++)
    //     {
    //         int edge_weight = get_edge(graph, u, v);

    //         if ((*d[v] < edge_weight))
    //         {
    //             d[v] = &edge_weight;
    //             remove_edge(u, v);
    //         }
    //     }

    //     // v_t[u] = &visited;
    //     u++;
    //     visited_count++;

    //     for (int v = 0; v < visited_count; v++)
    //     {
    //         int weight = get_edge(graph, u, v);

    //         if ((u == -1 || weight < *d[v]))
    //         {
    //             d[v] = &weight;
    //         }
    //     }
    // }

    // emitir a árvore geradora mínima
    printf("\nÁrvore mínima ");
    for (int i = 0; i < graph_size; i++)
    {
        // if (*d[i] > 0)
        printf(" vertice: %d peso: %d | ", i, *d[i]); // debug
                                                      // printf("=> %d", i);
    }
    printf("\n");

    return v_t;
}