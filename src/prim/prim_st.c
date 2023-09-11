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

static int get_u(int v, float *d, int *v_t, bool *visited, int graph_size);

int *prim_mst(float *graph, int graph_size, int graph_root)
{
    // como vou usar de 1 a graph_size e o vetor vai de 0 a graph_size - 1
    // é necessário adicionar o elemento em falta
    int *v_t = malloc((graph_size + 1) * sizeof(int));
    float *d = malloc((graph_size + 1) * sizeof(float));
    bool *visited = malloc((graph_size + 1) * sizeof(bool));

    v_t[graph_root] = graph_root;
    d[graph_root] = 0;
    visited[graph_root] = true;

    // inicializar a árvore mínima
    for (int v = 1; v <= graph_size; v++)
    {
        if (v != graph_root)
            visited[v] = false;

        float weight = get_edge(graph, graph_root, v);

        if (weight < INFINITE)
        {
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
        // excluir a raíz e v-v_t
        if (v == graph_root || visited[v])
            continue;

        // obter o vértice u
        int u = get_u(v, d, v_t, visited, graph_size);

        // printf("Found u: %d\n", u);

        visited[u] = true;

        for (int i = v; i <= graph_size; i++)
        {
            // excluir a diagonal e v-v_t
            if (i == u || visited[i])
                continue;

            float u_weight = get_edge(graph, u, i);

            if (u_weight == 0)
                continue;

            if (u_weight < d[i])
            {
                d[i] = u_weight;
                v_t[i] = u;
            }

#ifdef TRACE
            printf("(%d,%d) => weight: %f | d[v]: %f\n", u, i, u_weight, d[i]);
#endif
        }
    }

    free(d);
    free(visited);

    return v_t;
}

int get_u(int v, float *d, int *v_t, bool *visited, int graph_size)
{
    int u_min;
    float min_weight = INFINITE;

    for (int u = 1; u <= graph_size; u++)
    {
        // excluir a diagonal e v-v_t
        if (u == v || visited[u])
            continue;

        // float u_weight = get_edge(g)
        if (!visited[u] && d[u] < min_weight)
        {
            u_min = u;
            min_weight = d[u];
            // printf("v->%d | u_min->%d | min_weight: %f | d[v]: %f\n", v, u_min, min_weight, d[v]);
        }
    }

    return u_min;
}
