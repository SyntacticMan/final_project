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
    int n;
    int start;
    int end;
} ThreadData;

static int get_u(int v, float *d, int *v_t, bool *visited, int graph_size);
static void *prim_mst(void *arg);
static void print_mst(float *d, int *v_t, int graph_size);

/*
    process_edges

    prepara a matriz para ser divida entre as tarefas
*/
// void *process_edges(void *arg)
// {
//     ThreadData *data = (ThreadData *)arg;
//     int thread_id = data->thread_id;
//     int num_threads = data->num_threads;
//     int graph_size = data->graph_size;
//     int graph_root = data->graph_root;
//     bool *visited = data->visited;
//     int *v_t = data->v_t;
//     float *d = data->d;

//     printf("thread %d: start = %d || end = %d || n_p = %d\n", thread_id, start, end, n);

//     for (int v = start; v <= end; v++)
//     {
//         // excluir a raíz e v-v_t
//         if (v == graph_root || visited[v])
//             continue;

//         // obter o vértice u
//         int u = get_u(v, d, v_t, visited, graph_size);

//         // printf("Found u: %d\n", u);

//         visited[u] = true;

//         for (int i = v; i <= graph_size; i++)
//         {
//             // excluir a diagonal e v-v_t
//             if (i == u || visited[i])
//                 continue;

//             float u_weight = get_edge(graph, u, i);

//             if (u_weight == 0)
//                 continue;

//             if (u_weight < d[i])
//             {
//                 d[i] = u_weight;
//                 v_t[i] = u;
//             }

// #ifdef DEBUG
//             printf("(%d,%d) => weight: %f | d[v]: %f\n", u, i, u_weight, d[i]);
// #endif
//         }
//     }

//     pthread_exit(NULL);
// }

void prim_mt_mst(int graph_size, int graph_root, int num_threads)
{
    // certificar que não se pedem mais processos que vértices
    if (num_threads > graph_size)
        num_threads = graph_size;

#ifdef DEBUG
    printf("num_threads %d\n", num_threads);
#endif

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    // calcular o número de vértices (n) a alocar a cada processo
    int n = graph_size / num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].graph_root = graph_root;
        thread_data[i].n = n;

        // calcular ponto de início e fim de d_i
        thread_data[i].start = i * n + 1;
        thread_data[i].end = i * n + n;

        pthread_create(&threads[i], NULL, prim_mst, (void *)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void *prim_mst(void *arg)
{

    ThreadData *data = (ThreadData *)arg;
    int thread_id = data->thread_id;
    int graph_root = data->graph_root;
    int n = data->n;
    int start = data->start;
    int end = data->end;

    printf("thread %d: start = %d || end = %d || n = %d\n", thread_id, start, end, n);

    // alocar tendo em conta o número de vértices que foram atribuídos
    int *v_t = malloc(n * sizeof(int));
    float *d = malloc(n * sizeof(float));
    bool *visited = malloc(n * sizeof(bool));

    // processar graph_root apenas se estiver nos vértices
    // que foram atribuídos a este processo
    if (start < graph_root && graph_root < end)
    {
        v_t[graph_root] = graph_root;
        d[graph_root] = 0;
        visited[graph_root] = true;

        // inicializar a árvore mínima
        for (int v = start; v <= end; v++)
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
    }
    else
    {
        // senão fazer a inicialização simples
        for (int v = start; v <= end; v++)
        {
            visited[v] = false;

            d[v] = INFINITE;
            v_t[v] = 0; // marcado a 0 pois não há nenhum vértice 0
        }
    }

    for (int v = start; v <= end; v++)
    {
        // excluir a raíz e v-v_t
        if (v == graph_root || visited[v])
            continue;

        // obter o vértice u
        int u = get_u(v, d, v_t, visited, n);

        printf("(thread %d) Found u: %d\n", thread_id, u);

        visited[u] = true;

        for (int i = v; i <= end; i++)
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
            printf("[thread %d](%d,%d) => weight: %f | d[v]: %f\n", thread_id, u, i, u_weight, d[i]);
#endif
        }
    }

    print_mst(d, v_t, n);
    // free(v_t);
    // free(visited);
    // free(d);

    pthread_exit(NULL);

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
            printf("v->%d | u_min->%d | min_weight: %f | d[v]: %f\n", v, u_min, min_weight, d[v]);
        }
    }

    return u_min;
}

/*
    print_mst

    imprime a versão textual da árvore mínima
*/
void print_mst(float *d, int *v_t, int graph_size)
{
    printf("\n");

    for (int i = 1; i <= graph_size; i++)
    {
        printf(" %d=(%2f)=>%d ", v_t[i], d[i], i);

        // omitir na última iteração
        if (i != graph_size)
            printf(">");
    }

    printf("\n");
}
