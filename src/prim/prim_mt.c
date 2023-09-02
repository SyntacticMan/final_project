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
    int graph_root;
    int num_vertices;
    int start_vertice;
    int end_vertice;
    float *local_graph; // V
    int *v_t;
} ThreadData;

static int get_u(int v, float *d, int *v_t, bool *visited, int graph_size);
static float *split_graph(float *graph, int start_vertice, int end_vertice);
static void *prim_mst(void *arg);

int *prim_mt_mst(float *graph, int graph_size, int graph_root, int num_threads)
{
    // certificar que não se pedem mais processos que vértices
    if (num_threads > graph_size)
        num_threads = graph_size;

#ifdef DEBUG
    printf("num_threads %d\n", num_threads);
#endif

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int *v_t = malloc((graph_size + 1) * sizeof(int));

    // calcular o número de vértices (n) a alocar a cada processo
    int num_vertices = graph_size / num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].graph_root = graph_root;
        thread_data[i].num_vertices = num_vertices;

        thread_data[i].v_t = v_t;

        // calcular os vértices de início e fim com base no número de vértices a processar
        int start_vertice = (i * num_vertices) + 1;
        int end_vertice = start_vertice + num_vertices - 1;
        thread_data[i].start_vertice = start_vertice;
        thread_data[i].end_vertice = end_vertice;

        // criar o subconjunto de V a alocar à tarefa
        thread_data[i].local_graph = split_graph(graph, start_vertice, end_vertice);

        pthread_create(&threads[i], NULL, prim_mst, (void *)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return v_t;
}

void *prim_mst(void *arg)
{

    ThreadData *data = (ThreadData *)arg;
    int graph_root = data->graph_root;
    float *local_graph = data->local_graph;
    int *v_t = data->v_t;

#ifdef DEBUG
    printf("thread %d: start_vertice = %d || end_vertice = %d || num_vertices = %d\n", data->thread_id, start_vertice, end_vertice, data->num_vertices);
#endif

    // alocar tendo em conta o número de vértices que foram atribuídos
    // int *v_t = malloc((n + 1) * sizeof(int));
    float *d = malloc((data->num_vertices + 1) * sizeof(float));
    bool *visited = malloc((data->num_vertices + 1) * sizeof(bool));

    // processar graph_root apenas se estiver nos vértices
    // que foram atribuídos a este processo
    if (data->start_vertice < graph_root && graph_root < data->end_vertice)
    {
        v_t[graph_root] = graph_root;
        d[graph_root] = 0;
        visited[graph_root] = true;

        // inicializar a árvore mínima
        for (int v = data->start_vertice; v <= data->end_vertice; v++)
        {
            if (v != graph_root)
                visited[v] = false;

            float weight = get_edge(local_graph, graph_root, v);

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
    }
    else
    {
        // senão fazer a inicialização simples
        for (int v = data->start_vertice; v <= data->end_vertice; v++)
        {
            visited[v] = false;

            d[v] = INFINITE;
            v_t[v] = 0; // marcado a 0 pois não há nenhum vértice 0
        }
    }

    for (int v = data->start_vertice; v <= data->end_vertice; v++)
    {
        // excluir a raíz e v-v_t
        if (v == graph_root || visited[v])
            continue;

        // obter o vértice u
        int u = get_u(v, d, v_t, visited, data->num_vertices);

#ifdef TRACE
        printf("(thread %d) Found u: %d\n", thread_id, u);
#endif

        visited[u] = true;

        for (int i = v; i <= data->end_vertice; i++)
        {
            // excluir a diagonal e v-v_t
            if (i == u || visited[i])
                continue;

            float u_weight = get_edge(local_graph, u, i);

            if (u_weight == 0)
                continue;

            if (u_weight < d[i])
            {
                d[i] = u_weight;
                v_t[i] = u;
            }

#ifdef TRACE
            printf("[thread %d](%d,%d) => weight: %f | d[v]: %f\n", data->thread_id, u, i, u_weight, d[i]);
#endif
        }
    }

    // free(v_t);
    // free(visited);
    // free(d);

    pthread_exit(NULL);
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

#ifdef TRACE
            printf("v->%d | u_min->%d | min_weight: %f | d[v]: %f\n", v, u_min, min_weight, d[v]);
#endif
        }
    }

    return u_min;
}

/*
    split_graph

    cria um subconjunto de graph, com início em start_vertice e fim em end_vertice
*/
float *split_graph(float *graph, int start_vertice, int end_vertice)
{
    if (graph == NULL)
        return NULL;

    // determinar os indíces de início e fim do vetor a copiar
    // início é col e primeira linha
    unsigned int start_index = get_index(start_vertice, 1);

    // como considero apenas a triangular superior
    // fim é col e a penúltima linha (col, col-1)
    unsigned int end_index = get_index(end_vertice, (end_vertice - 1));

    // determinar o tamanho do subconjunto
    unsigned int graph_subset = end_index - start_index + 1;

    float *sub_graph = (float *)malloc(graph_subset * sizeof(float));

    // copiar o grafo para o novo vetor
    for (int i = 0; i < graph_subset; i++)
    {
        sub_graph[i] = graph[start_index + i];
    }

    return sub_graph;
}
