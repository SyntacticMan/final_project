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

int *prim_mst(int array_size, int graph_size, int graph_root)
{
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
    for (int v = 0; v < graph_size; v++)
    {
        // passar a raíz à frente
        if (v == graph_root)
            continue;

        // processar apenas se ainda não tiver sido visitado
        if (!visited[v])
        {
            for (int u = 0; u < graph_size; u++)
            {
                if (v == u)
                    continue;

                int edge_weight = get_edge(graph, v, u);

                if (edge_weight < d[u])
                {
                    d[u] = edge_weight;
                    visited[u] = true;
                }
            }
        }
    }
    /* TODO: pôr aqui um array que guarde os nós a que correspondem os d[u]*/
    free(visited);
    return d;
}