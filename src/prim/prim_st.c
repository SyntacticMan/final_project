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

void prim_mst(int array_size, int graph_size, int graph_root)
{
    // v_t = root_node
    // vetor d é inicializado com o tamanho do grafo
    // e o vértice raíz igual a zero
    int **d = (int **)malloc(graph_size * sizeof(int *));

    int root_weight = 0;
    d[graph_root] = &root_weight;

    // v_t tem o mesmo tamanho de graph
    int **v_t = calloc(graph_size, sizeof(int *));

    // marcar vértice com tendo sido visitado
    int visited = 1;
    v_t[graph_root] = &visited;

    // obter todas as arestas que pertencem à raíz
    int infinite = -1;
    for (int v = 0; v < graph_size; v++)
    {
        int edge_weight = get_edge(graph_root, v);
        if (edge_weight > 0)
        {
            d[v] = &edge_weight;
        }
        else
        {
            d[v] = &infinite;
        }
    }

    int visited_count = 1;
    while (visited_count < graph_size - 1)
    {
        int u = 0;

        for (int v = 0; v < visited_count; v++)
        {
            int edge_weight = get_edge(u, v);

            if ((*d[v] < edge_weight))
            {
                d[v] = &edge_weight;
                remove_edge(u, v);
            }
        }

        // v_t[u] = &visited;
        u++;
        visited_count++;

        for (int v = 0; v < visited_count; v++)
        {
            int weight = get_edge(u, v);

            if ((u == -1 || weight < *d[v]))
            {
                d[v] = &weight;
            }
        }
    }

    // emitir a árvore geradora mínima
    printf("%d", graph_root);

    for (int i = 0; i < graph_size; i++)
    {
        if (*d[i] > 0)
            printf("=>%dw(%d)", i, *d[i]);
    }
    printf("\n");
}