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

static int get_u(int v, float *d, int graph_size);

float *prim_mst(int array_size, int graph_size, int graph_root)
{
    int *v_t = malloc(graph_size * sizeof(int));
    float *d = malloc(graph_size * sizeof(float));

    v_t[graph_root] = graph_root;
    d[graph_root] = 0;

    // print_array(graph, graph_size);

    // inicializar a árvore mínima
    for (int v = 1; v <= graph_size; v++)
    {
        float weight = get_edge(graph, graph_root, v);

        if (weight < INFINITE)
        {
            printf("v: %d, w:%f\n", v, weight);
            d[v] = weight;
            v_t[v] = graph_root;
        }
        else
        {
            d[v] = INFINITE;
            v_t[v] = 0; // marcado a 0 pois não há nenhum vértice 0
        }
    }

    for (int v = 1; v <= graph_size; v++)
    {
        // passar a raíz à frente
        if (v == graph_root)
            continue;

        // obter o vértice u
        int u = get_u(v, d, graph_size);

        v_t[v] = u;

        for (int i = v; i < graph_size; i++)
        {
            // excluir a diagonal
            if (v == u)
                continue;

            int u_weight = get_edge(graph, u, v);
            if (u_weight < d[v])
            {
                d[v] = u_weight;
            }
        }

        // for (int u = 1; u <= graph_size; u++)
        // {

        //     int edge_weight = get_edge(graph, v, u);

        //     if (edge_weight < d[u])
        //     {
        //         d[u] = edge_weight;
        //         v_t[u] = v;
        //     }
        // }
    }

    return d;
}

int get_u(int v, float *d, int graph_size)
{
    // começa como infinito
    int u = INFINITE;

    for (int i = v; i < graph_size; i++)
    {
        // excluir a diagonal
        if (u == v)
            continue;

        float weight = get_edge(graph, u, v);

        if (d[v] < weight)
            d[v] = weight;

        if (d[v] == INFINITE)
        {
            weight = get_edge(graph, u, v);
        }
        else
        {
            weight = d[v];
        }

        if (u < weight)
        {
            u = v;
        }
    }

    return u;
}