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

static int get_u(float *d, bool *visited, int graph_size);
static bool all_visited(int graph_size, bool *visited);

/*
    prim_mst

    implementação do algoritmo de prim
    versão em tarefa simples
*/
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

    while (!all_visited(graph_size, visited))
    {

        // obter o vértice u
        int u = get_u(d, visited, graph_size);
        visited[u] = true;

        for (int i = 1; i <= graph_size; i++)
        {
            // excluir os já visitados
            if (visited[i] || i == u)
            {
                continue;
            }

            float u_weight = get_edge(graph, u, i);
            float d_weight = d[i];

            // se não tiver ligação, omitir
            if (u_weight == INFINITE)
                continue;

            if (u_weight < d_weight)
            {
                d[i] = u_weight;
                v_t[i] = u;
            }
        }
    }

    free(d);
    free(visited);

    return v_t;
}
/*
    get_u

    obtém o vértice u com o menor peso que há em d[v]
*/
int get_u(float *d, bool *visited, int graph_size)
{
    int u_min = 0;
    float min_weight = INFINITE;

    for (int u = 1; u <= graph_size; u++)
    {
        // excluir os já visitados (v-vt)
        if (!visited[u] && d[u] < min_weight)
        {
            u_min = u;
            min_weight = d[u];
        }
    }

    return u_min;
}

/*
    all_visited

    avalia se todos os vértices do grafo já foram visitados

*/
bool all_visited(int graph_size, bool *visited)
{
    for (int i = 1; i <= graph_size; i++)
    {
        // sair assim que aparecer o primeiro vértice não visitado
        if (!visited[i])
        {
            return false;
            break;
        }
    }
    return true;
}