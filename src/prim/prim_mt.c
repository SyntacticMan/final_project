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
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int count;
} CountdownLatch;

typedef struct _thread_data
{
    int thread_id;
    int graph_size;
    int start_col;
    int end_col;
    int num_threads;
    float *local_graph;
    float *local_d;
} thread_data;

// typedef struct _main_data
// {
//     int num_threads;
// } MainData;

pthread_mutex_t mutex_minu;
pthread_mutex_t mutex_lock;
pthread_mutex_t mutex_sync;
pthread_mutex_t mutex_globalu;
pthread_mutex_t mutex_globalweight;
pthread_mutex_t mutex_vt;
pthread_mutex_t mutex_finish_count;
pthread_mutex_t mutex_visited;
pthread_mutex_t mutex_thread_counter;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_wait = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_finished = PTHREAD_COND_INITIALIZER;

// pthread_barrier_t finish_barrier;
// pthread_barrier_t barrier;

CountdownLatch finish_latch;

// variáveis partilhadas
int min_u;
int *global_u;
float *global_weight;
int *v_t;
bool *visited;
int thread_counter;
bool global_u_set;
bool all_vertices_visited;
static int get_u(float *d, int v, int start_col, int end_col);
static float *split_graph(float *graph, int start_col, int end_col, int graph_size);
static float *split_d(float *d, int start_col, int end_col, int graph_size);
static bool all_visited(int graph_size);

/*
    funções para lidar com os processos
*/
static void process_error(char *name, int result);

static void initialize_mutexes(void);
static void destroy_mutexes(void);

static void *worker_prim(void *arg);

void countdown_latch_init(CountdownLatch *latch, int initial_count);
void countdown_latch_count_down(CountdownLatch *latch);
void countdown_latch_await(CountdownLatch *latch);
void countdown_latch_destroy(CountdownLatch *latch);

int *prim_mt_mst(float *graph, int graph_size, int graph_root, int num_threads)
{
    // certificar que não se pedem mais processos que vértices
    if (num_threads > graph_size)
        num_threads = graph_size;

    pthread_t threads[num_threads];

    // alocar os recursos partilhados
    v_t = malloc((graph_size + 1) * sizeof(int));
    visited = malloc((graph_size + 1) * sizeof(bool));
    float *d = malloc((graph_size + 1) * sizeof(float));
    global_u = malloc((num_threads + 1) * sizeof(int));
    global_weight = malloc((num_threads + 1) * sizeof(float));
    min_u = graph_root;
    thread_counter = 0;
    global_u_set = false;
    all_vertices_visited = false;

    // inicializar os vetores globais a 0
    if (global_u != NULL)
        memset(global_u, 0, (num_threads + 1) * sizeof(int));

    if (global_weight != NULL)
        memset(global_weight, 0, (num_threads + 1) * sizeof(float));

    // inicializar condições e barreira
    process_error("pthread_cond_init", pthread_cond_init(&cond, NULL));
    // process_error("barrier_init", pthread_barrier_init(&barrier, NULL, num_threads + 1));
    initialize_mutexes();

    // tarefas acessórias
    thread_data thread_data[num_threads];

    // calcular o número de vértices (n) a alocar a cada processo
    int num_vertices = graph_size / num_threads;

    // inicializar a árvore mínima
    v_t[graph_root] = graph_root;
    d[graph_root] = 0;
    visited[graph_root] = true;

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
            v_t[v] = 0;
        }

#ifdef DEBUG
        printf("d[%d]=%0.2f\tv_t[%d]=%d\tvisited[%d]=%d\n", v, d[v] /*get_d(d, v)*/, v, v_t[v], v, visited[v]);
#endif
    }

    // lançar os processos
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].graph_size = graph_size;
        thread_data[i].num_threads = num_threads;

        // calcular os vértices de início e fim com base no número de vértices a processar
        int start_col = (i * num_vertices) + 1;
        int end_col = start_col + num_vertices - 1;

        thread_data[i].start_col = start_col;
        thread_data[i].end_col = end_col;

        thread_data[i].local_graph = split_graph(graph, start_col, end_col, graph_size);
        thread_data[i].local_d = split_d(d, start_col, end_col, graph_size);

#ifdef TRACE
        print_graph_mt(thread_data[i].local_graph, start_col, end_col, graph_size);
#endif

        process_error("worker_thread create", pthread_create(&threads[i], NULL, worker_prim, (void *)&thread_data[i]));
    }

    int valor = 0;

    while (!all_vertices_visited)
    {
        all_vertices_visited = all_visited(graph_size);
        valor++;

        pthread_mutex_lock(&mutex_sync);

        while (thread_counter < num_threads)
        {
#ifdef DEBUG
            printf("[thread main] waiting for worker threads\n");
            printf("[thread main] num_threads: %d\tthread_counter: %d\n", num_threads, thread_counter);
#endif
            pthread_cond_wait(&cond_wait, &mutex_sync);
        }
        thread_counter = 0;
        global_u_set = false;
        pthread_mutex_unlock(&mutex_sync);

#ifdef DEBUG
        printf("[thread main] compute global_u\n");
#endif

        pthread_mutex_lock(&mutex_lock);
        min_u = 0;
        float min_weight = INFINITE;

        for (int i = 0; i < num_threads; i++)
        {
            printf("global_u[%d] -> global_weight[%0.3f]\n", global_u[i], global_weight[i]);

            if (visited[global_u[i]])
                continue;

            if (global_weight[i] < min_weight)
            {
                min_u = global_u[i];
                min_weight = global_weight[i];
            }
        }

        global_u_set = true;
#ifdef DEBUG
        printf("[thread main] broadcast global_u: %d\tmin_weight: %0.2f\n", min_u, min_weight);
#endif
        pthread_mutex_unlock(&mutex_lock);

        pthread_mutex_lock(&mutex_sync);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex_sync);

        all_vertices_visited = all_visited(graph_size);
    }

    // pthread_cond_broadcast(&cond_finished);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], (void **)NULL);
    }

    free(global_u);
    free(global_weight);
    free(d);
    free(visited);
    free(v_t);

    destroy_mutexes();
    process_error("condition destroy", pthread_cond_destroy(&cond));

#ifdef DEBUG
    printf("print_mst_mt ended\n");
#endif
    return v_t;
}

void *worker_prim(void *arg)
{
    thread_data *data = (thread_data *)arg;
    bool local_visited = false;

#ifdef DEBUG
    printf("[thread %d]: start_col = %d || end_col = %d\n", data->thread_id, data->start_col, data->end_col);
#endif

    bool local_all_visited = all_visited(6);

    while (!local_all_visited)
    {
        for (int v = data->start_col; v <= data->end_col; v++)
        {
            pthread_mutex_lock(&mutex_lock);
            local_visited = visited[v];
            pthread_mutex_unlock(&mutex_lock);
            // excluir v-v_t
            if (local_visited)
                continue;
            // obter o vértice u
            int u = get_u(data->local_d, v, data->start_col, data->end_col);

#ifdef DEBUG
            printf("[thread %d] Found u: %d (v=%d | weight = %0.3f)\n", data->thread_id, u, v, data->local_d[u]);
#endif

            // enviar para ser avaliado
            pthread_mutex_lock(&mutex_lock);
            global_u[data->thread_id] = u;
            global_weight[data->thread_id] = get_edge(data->local_graph, v, u);
            pthread_mutex_unlock(&mutex_lock);

            pthread_mutex_lock(&mutex_sync);
            if (thread_counter < data->num_threads)
                thread_counter++;
            pthread_cond_signal(&cond_wait);

#ifdef DEBUG
            printf("[thread %d] signal main thread to resume\tthread_counter: %d\n", data->thread_id, thread_counter);
#endif

            // aguardar que o global_u tenha sido calculado
            while (!global_u_set)
            {
#ifdef DEBUG
                printf("[thread %d] waiting for main thread to compute global_u\n", data->thread_id);
#endif
                pthread_cond_wait(&cond, &mutex_sync);
            }
#ifdef DEBUG
            printf("[thread %d] main thread says resume\n", data->thread_id);
#endif
            pthread_mutex_unlock(&mutex_sync);

            // se tiver sido escolhido o u deste processo, adicioná-lo a v_t
            pthread_mutex_lock(&mutex_lock);
            int local_min_u = min_u;
            pthread_mutex_unlock(&mutex_lock);

#ifdef DEBUG
            printf("[thread %d] received min_u = %d\n", data->thread_id, local_min_u);
#endif

            if (local_min_u >= data->start_col && local_min_u <= data->end_col)
            {
                pthread_mutex_lock(&mutex_lock);
                visited[local_min_u] = true;
                pthread_mutex_unlock(&mutex_lock);
#ifdef DEBUG
                printf("[thread %d] set %d as visited\n", data->thread_id, u);
#endif
            }

            u = local_min_u;

            for (int i = data->start_col; i <= data->end_col; i++)
            {
                pthread_mutex_lock(&mutex_lock);
                local_visited = visited[i];
                pthread_mutex_unlock(&mutex_lock);

                // excluir a diagonal e v-v_t
                if (local_visited || i == u)
                {
#ifdef DEBUG
                    printf("[thread %d] excluding v = %d\n", data->thread_id, i);
#endif
                    continue;
                }
#ifdef DEBUG
                else
                    printf("[thread %d] processing v = %d for u = %d\n", data->thread_id, i, u);
#endif

                float u_weight = get_edge(data->local_graph, u, i);
                float d_weight = data->local_d[i];
#ifdef TRACE
                printf("[thread %d]  u_weight (%d) = %f\td_weight (%d) = %f\n", data->thread_id, u, u_weight, i, d_weight);
#endif
                if (u_weight == INFINITE)
                    continue;

                if (u_weight < d_weight)
                {
                    data->local_d[i] = u_weight;
                    pthread_mutex_lock(&mutex_lock);
                    v_t[i] = u;
                    printf("[thread %d] set d[%d]=%0.2f\tv_t[%d]=%d\n", data->thread_id, i, data->local_d[i], i, v_t[i]);
                    pthread_mutex_unlock(&mutex_lock);
                }
            }
            local_all_visited = all_visited(6);
        }

        // se ainda existirem vértices por visitar, recomeçar
        // pthread_mutex_lock(&mutex_lock);
        // if (v == data->end_col && !all_vertices_visited)
        // {
        //     v = data->start_col;
        // }
        // pthread_mutex_unlock(&mutex_lock);
    }

#ifdef DEBUG
    pthread_mutex_lock(&mutex_lock);

    for (int i = data->start_col; i <= data->end_col; i++)
    {
        printf("[thread %d] set d[%d]=%0.2f\tv_t[%d]=%d\n", data->thread_id, i, data->local_d[i], i, v_t[i]);
    }
    pthread_mutex_unlock(&mutex_lock);

    printf("[thread %d] finished\n", data->thread_id);
#endif

    pthread_mutex_lock(&mutex_lock);
    global_u[data->thread_id] = 0;
    global_weight[data->thread_id] = INFINITE;
    pthread_mutex_unlock(&mutex_lock);

    // pthread_cond_wait(&cond_finished);
    pthread_exit(NULL);
}

/*
    get_u

    obtém o vértice com o menor peso em d
    dos que pertencem a v-v_t
*/
int get_u(float *d, int v, int start_col, int end_col)
{
    pthread_mutex_lock(&mutex_lock);
    int u_min = v_t[v];
    float min_weight = d[v];

    for (int u = start_col; u <= end_col; u++)
    {
        // excluir os já visitados (v-vt)
        if (visited[u] || u == v)
        {
#ifdef DEBUG
            printf("[get_u] excluding u = %d for v = %d\n", u, v);
#endif
            continue;
        }

        if (d[u] < min_weight)
        {
            u_min = u;
            min_weight = d[u];
        }
    }
    pthread_mutex_unlock(&mutex_lock);

    return u_min;
}

/*
    split_graph

    cria um subconjunto de graph, com início em start_col e fim em end_col
*/
float *split_graph(float *graph, int start_col, int end_col, int graph_size)
{
    if (graph == NULL)
        return NULL;

    unsigned long long matrix_size = get_matrix_size(graph_size);
    float *split_graph = malloc(matrix_size * sizeof(float));

    // inicializar o grafo
    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row <= graph_size; row++)
        {
            if (row >= col)
                continue;

            add_null_edge(split_graph, col, row);
#ifdef TRACE
            printf("(col:%d|row:%d|null)\n", col, row);
#endif
        }
    }

    for (int col = 1; col <= graph_size; col++)
    {
        // apenas processar as linhas que pertencem
        // às colunas pedidas
        if (col >= start_col && col <= end_col)
        {
            for (int row = 1; row <= graph_size; row++)
            {
                if (col == row)
                    continue;
                add_edge(split_graph, col, row, get_edge(graph, col, row));
#ifdef TRACE
                printf("(col:%d|row:%d|%f)\n", col, row, get_edge(graph, col, row));
#endif
            }
        }
    }

    return split_graph;
}

float *split_d(float *d, int start_col, int end_col, int graph_size)
{
    if (d == NULL)
        return NULL;

    float *split_d = malloc((graph_size + 1) * sizeof(float));

    for (int i = 1; i <= graph_size; i++)
    {
        if (i >= start_col && i <= end_col)
        {
            split_d[i] = d[i];
        }
        else
        {
            split_d[i] = INFINITE;
        }
#ifdef TRACE
        printf("split_d[%d]=%0.2f\tv_t[%d]=%d\n", i, split_d[i], i, v_t[i]);
#endif
    }

    return split_d;
}

bool all_visited(int graph_size)
{
    bool result = true;
    pthread_mutex_lock(&mutex_lock);
    for (int i = 1; i <= graph_size; i++)
    {
        if (!visited[i])
            result = false;
    }
    pthread_mutex_unlock(&mutex_lock);
    return result;
}

void initialize_mutexes(void)
{
    process_error("mutex_minu init", pthread_mutex_init(&mutex_minu, NULL));
    process_error("mutex_globalu", pthread_mutex_init(&mutex_globalu, NULL));
    process_error("mutex_globalweight", pthread_mutex_init(&mutex_globalweight, NULL));
    process_error("mutex_vt init", pthread_mutex_init(&mutex_vt, NULL));
    process_error("mutex_lock init", pthread_mutex_init(&mutex_lock, NULL));
    process_error("mutex_lock init", pthread_mutex_init(&mutex_sync, NULL));
    process_error("mutex_finish_count init", pthread_mutex_init(&mutex_finish_count, NULL));
    process_error("mutex_visited init", pthread_mutex_init(&mutex_visited, NULL));
    process_error("mutext_thread_counter init", pthread_mutex_init(&mutex_thread_counter, NULL));
}

void destroy_mutexes(void)
{
    process_error("mutex_minu destroy", pthread_mutex_destroy(&mutex_minu));
    process_error("mutex_globalu destroy", pthread_mutex_destroy(&mutex_globalu));
    process_error("mutex_globalweight destroy", pthread_mutex_destroy(&mutex_globalweight));
    process_error("mutex_vt destroy", pthread_mutex_destroy(&mutex_vt));
    process_error("mutex_lock destroy", pthread_mutex_destroy(&mutex_lock));
    process_error("mutex_lock destroy", pthread_mutex_destroy(&mutex_sync));
    process_error("mutex_finish_count destroy", pthread_mutex_destroy(&mutex_finish_count));
    process_error("mutex_visited destroy", pthread_mutex_destroy(&mutex_visited));
    process_error("mutex_thread_counteer destroy", pthread_mutex_destroy(&mutex_thread_counter));
}

void process_error(char *name, int result)
{
    if (result != 0)
        printf("%s has returned an error\n", name);
}

void countdown_latch_init(CountdownLatch *latch, int initial_count)
{
    pthread_mutex_init(&latch->mutex, NULL);
    pthread_cond_init(&latch->condition, NULL);
    latch->count = initial_count;
}

void countdown_latch_count_down(CountdownLatch *latch)
{
    pthread_mutex_lock(&latch->mutex);
    latch->count--;
    if (latch->count == 0)
    {
        pthread_cond_broadcast(&latch->condition);
    }
    pthread_mutex_unlock(&latch->mutex);
}

void countdown_latch_await(CountdownLatch *latch)
{
    pthread_mutex_lock(&latch->mutex);
    while (latch->count > 0)
    {
        pthread_cond_wait(&latch->condition, &latch->mutex);
    }
    pthread_mutex_unlock(&latch->mutex);
}

void countdown_latch_destroy(CountdownLatch *latch)
{
    pthread_mutex_destroy(&latch->mutex);
    pthread_cond_destroy(&latch->condition);
}
