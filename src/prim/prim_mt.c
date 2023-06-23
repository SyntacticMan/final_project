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

typedef struct
{
    int thread_id;
    int num_threads;
    int graph_size;
    int graph_root;
    int *visited;
    int *d;
} ThreadData;

void *process_edges(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;
    int graph_size = data->graph_size;
    int graph_root = data->graph_root;
    int *visited = data->visited;
    int *d = data->d;

    int start = thread_id == 0 ? 0 : thread_id * (graph_size / num_threads);

    // recalcular o tamanho do grafo a ser processado
    int graph_columns = start + (graph_size / num_threads);

    printf("thread %d: start = %d || graph_columns = %d\n", thread_id, start, graph_columns);

    // processar a raíz, se ela estiver na parte que foi alocada
    if (graph_root >= start && graph_root <= graph_columns)
    {
        for (int v = 0; v < graph_size; v++)
        {
            int edge_weight = get_edge(graph, graph_root, v);
            if (edge_weight != INFINITE)
            {
                d[v] = edge_weight;
            }
        }
    }

    // encontrar u
    for (int v = start; v < graph_columns; v++)
    {
        printf("v = %d\n", v);

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

                printf("u = %d > weight = %d\n", u, edge_weight);
                if (edge_weight < d[u])
                {
                    d[u] = edge_weight;
                    visited[u] = true;
                }
            }
        }
    }

    pthread_exit(NULL);
}

int *prim_mt_mst(int array_size, int graph_size, int graph_root, int num_threads)
{
    int *visited = malloc(graph_size * sizeof(int));
    int *d = malloc(graph_size * sizeof(int));

    for (int v = 0; v < graph_size; v++)
    {
        visited[v] = false;
        if (v == graph_root)
        {
            visited[v] = true;
            d[v] = 0;
        }
        else
        {
            d[v] = INT_MAX;
        }
    }

    if (num_threads > graph_size)
        num_threads = graph_size;

    printf("num_threads %d\n", num_threads);

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        thread_data[i].graph_size = graph_size;
        thread_data[i].graph_root = graph_root;
        thread_data[i].visited = visited;
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
