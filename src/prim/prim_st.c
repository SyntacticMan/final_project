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

    // inicializar a árvore mínima
    for (int v = 0; v < graph_size; v++)
    {
        // inicializar os vértices visitados
        visited[v] = false;
        if (v == graph_root)
        {
            visited[v] = true;
            d[v] = 0;
        }
        else
            d[v] = INFINITE;
    }

    // processar as arestas da raíz
    for (int v = 0; v < graph_size; v++)
    {
        int edge_weight = get_edge(graph, graph_root, v);
        if (edge_weight != INFINITE)
        {
            d[v] = edge_weight;
        }
    }

    // encontrar u
    for (int u = 0; u < graph_size; u++)
    {
        // passar a raíz à frente
        if (u == graph_root)
            continue;

        // processar apenas se ainda não tiver sido visitado
        if (!visited[u])
        {
            for (int v = 0; v < graph_size; v++)
            {
                if (v == u)
                    continue;

                int edge_weight = get_edge(graph, u, v);

                if (edge_weight < d[v])
                {
                    d[v] = edge_weight;
                    visited[u] = true;
                }
            }
        }
    }

    // emitir a árvore geradora mínima
    printf("\nÁrvore mínima \n");

    for (int col = 0; col < graph_size; col++)
    {
        for (int row = 0; row < graph_size; row++)
        {
            if (row == col)
            {
                add_edge(v_t, col, row, 0);
                continue;
            }

            int edge = get_edge(graph, col, row);

            if (edge == INFINITE)
                add_null_edge(v_t, col, row);
            else
            {
                if (edge == d[col])
                {
                    add_edge(v_t, col, row, edge);
                }
                else
                    add_null_edge(v_t, col, row);
            }
        }
    }

    graph = v_t;
    print_graph(graph_size);
    printf("\n");

    printf("    ");

    for (int i = 0; i < graph_size; i++)
    {
        // if (*d[i] > 0)
        printf("%2d|", i);
    }

    printf("\nd[] ");

    for (int i = 0; i < graph_size; i++)
    {
        printf("%2d|", d[i]); // debug
    }
    printf("\n");

    // for (int i = 0; i < graph_size; i++)
    // {
    //     printf("%2d|", visited[i]);
    // }
    // printf("\n");

    return v_t;
}