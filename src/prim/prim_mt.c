/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Implementação em multi tarefa                                    *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef PRIM_MT
#define PRIM_MT
#include "prim_mt.h"
#endif

typedef struct _thread_data
{
    int thread_id;
    int num_threads;
    int graph_size;
    int graph_root;
    bool *visited;
    int *v_t;
    float *d;
} ThreadData;

static int get_u(int v, float *d, int *v_t, bool *visited, int graph_size);

/*
    process_edges

    prepara a matriz para ser divida entre as tarefas
*/
void *process_edges(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;
    int graph_size = data->graph_size;
    int graph_root = data->graph_root;
    bool *visited = data->visited;
    int *v_t = data->v_t;
    float *d = data->d;

    // calcular n/p
    int n = graph_size / num_threads;
    int start = thread_id * n + 1;
    int end = start + n - 1;

    printf("thread %d: start = %d || end = %d || n_p = %d\n", thread_id, start, end, n);

    for (int v = start; v <= end; v++)
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

#ifdef DEBUG
            printf("(%d,%d) => weight: %f | d[v]: %f\n", u, i, u_weight, d[i]);
#endif
        }
    }

    pthread_exit(NULL);
}

float *prim_mt_mst(int array_size, int graph_size, int graph_root, int num_threads)
{
    int *v_t = malloc(graph_size * sizeof(int));
    float *d = malloc(graph_size * sizeof(float));
    bool *visited = malloc(graph_size * sizeof(bool));

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

    if (num_threads > graph_size)
        num_threads = graph_size;

#ifdef DEBUG
    printf("num_threads %d\n", num_threads);
#endif

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        thread_data[i].graph_size = graph_size;
        thread_data[i].graph_root = graph_root;
        thread_data[i].visited = visited;
        thread_data[i].v_t = v_t;
        thread_data[i].d = d;

        pthread_create(&threads[i], NULL, process_edges, (void *)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(visited);
    return d;
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

        if (!visited[u] && d[u] < min_weight)
        {
            u_min = u;
            min_weight = d[u];
            // printf("v->%d | u_min->%d | min_weight: %f | d[v]: %f\n", v, u_min, min_weight, d[v]);
        }
    }

    return u_min;
}
