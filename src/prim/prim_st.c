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

static int get_u(int v, float *d, bool *visited, int graph_size);

float *prim_mst(int array_size, int graph_size, int graph_root)
{
    int *v_t = malloc(graph_size * sizeof(int));
    float *d = malloc(graph_size * sizeof(float));
    bool *visited = malloc(graph_size * sizeof(bool));

    v_t[graph_root] = graph_root;
    d[graph_root] = 0;
    visited[graph_root] = true;

    printf("graph_size: %d\n", graph_size);

    // print_array(graph, graph_size);

    // inicializar a árvore mínima
    for (int v = 1; v <= graph_size; v++)
    {
        printf("v:%d\n", v);
        if (v != graph_root)
            visited[v] = false;

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
        if (v == graph_root || visited[v])
            continue;

        // obter o vértice u
        // int u = get_u(v, d, visited, graph_size);

        // printf("Found u: %d\n", u);

        // v_t[v] = u;
        // visited[u] = true;

        // for (int i = v; i < graph_size; i++)
        // {
        //     // excluir a diagonal
        //     if (v == u || visited[v])
        //         continue;

        //     int u_weight = get_edge(graph, u, v);
        //     if (u_weight < d[v])
        //     {
        //         d[v] = u_weight;
        //     }
        // }

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

int get_u(int v, float *d, bool *visited, int graph_size)
{
    // começa como infinito
    int u_min = 0;

    for (int u = 1; u <= graph_size; u++)
    {
        printf("u->%d\n", u);
        // excluir a diagonal e v-v_t
        if (u == v || visited[u])
            continue;

        float weight = get_edge(graph, u, v);

        printf("weight: %f | d[v]: %f\n", weight, d[v]);

        if (d[v] < weight)
        {
            u_min = u;
            d[u] = d[v];
        }
        else if (weight < d[v])
        {
            u_min = u;
            d[u] = weight;
        }
    }

    return u_min;
}