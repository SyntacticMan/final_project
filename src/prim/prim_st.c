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
    // int **v_t = (int **)malloc(get_array_size(graph_size) * sizeof(int *));
    int *v_t = malloc(graph_size * sizeof(int *));
    int *d = malloc(graph_size * sizeof(int *));

    // inicializar a árvore mínima
    for (int v = 0; v < graph_size; v++)
    {
        // inicializar os vértices visitados
        v_t[v] = false;
        if (v == graph_root)
        {
            v_t[v] = true;
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
        if (!v_t[u])
        {
            for (int v = 0; v < graph_size; v++)
            {
                if (v == u)
                    continue;

                int edge_weight = get_edge(graph, u, v);

                if (edge_weight < d[v])
                {
                    d[v] = edge_weight;
                    v_t[u] = true;
                }
            }
        }
    }

    // emitir a árvore geradora mínima
    printf("\nÁrvore mínima \n");

    // graph = v_t;
    // print_graph(graph_size);
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

    return v_t;
}