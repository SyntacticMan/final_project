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
    int graph_size;
    int start_col;
    int end_col;
    float *local_graph;
    float *local_d;
} thread_data;

typedef struct _main_data
{
    int num_threads;
} MainData;

pthread_mutex_t mutex_minu;
pthread_mutex_t mutex_wait;
pthread_mutex_t mutex_globalu;
pthread_mutex_t mutex_globalweight;
pthread_mutex_t mutex_vt;
pthread_mutex_t mutex_finish_count;
pthread_mutex_t mutex_visited;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_barrier_t finish_barrier;
pthread_barrier_t barrier;

// variáveis partilhadas
int min_u; //, finish_count;
int *global_u;
float *global_weight;
int *v_t;
bool *visited;

static int get_u(float *d, int v, int start_col, int end_col);

static float *split_graph(float *graph, int start_col, int end_col, int graph_size);
static float *split_d(float *d, int start_col, int end_col, int graph_size);
static bool all_visited(int graph_size);

/*
    funções de acesso a recursos partilhados
*/
static void set_vt(int index, int value);
static int get_vt(int index);
static void set_visited(int index, bool value);
static bool get_visited(int index);
static int get_min_u(void);
static void set_min_u(int value);
static int get_global_u(int index);
static void set_global_u(int index, int value);
static float get_global_weight(int index);
static void set_global_weight(int index, float value);

static void process_error(char *name, int result);

static void initialize_mutexes(void);
static void destroy_mutexes(void);

static void *prim_mst(void *arg);

int *prim_mt_mst(float *graph, int graph_size, int graph_root, int num_threads)
{
    // certificar que não se pedem mais processos que vértices
    if (num_threads > graph_size)
        num_threads = graph_size;

    pthread_t threads[num_threads];

    v_t = malloc((graph_size + 1) * sizeof(int));
    visited = malloc((graph_size + 1) * sizeof(bool));
    float *d = malloc((graph_size + 1) * sizeof(float));
    global_u = malloc((num_threads + 1) * sizeof(int));
    global_weight = malloc((num_threads + 1) * sizeof(float));
    min_u = graph_root;

    // inicializar os vetores
    if (global_u != NULL)
        memset(global_u, 0, (num_threads + 1) * sizeof(int));

    if (global_weight != NULL)
        memset(global_weight, 0, (num_threads + 1) * sizeof(float));

    process_error("pthread_cond_init", pthread_cond_init(&cond, NULL));

    process_error("barrier_init", pthread_barrier_init(&barrier, NULL, num_threads + 1));

    initialize_mutexes();

    // tarefas acessórias
    thread_data thread_data[num_threads];

    // calcular o número de vértices (n) a alocar a cada processo
    int num_vertices = graph_size / num_threads;

    // inicializar a árvore mínima

    set_vt(graph_root, graph_root);
    d[graph_root] = 0; // set_d(d, graph_root, 0);
    set_visited(graph_root, true);

    for (int v = 1; v <= graph_size; v++)
    {
        if (v != graph_root)
            set_visited(v, false);

        float weight = get_edge(graph, graph_root, v);

        if (weight < INFINITE)
        {
            d[v] = weight;
            set_vt(v, graph_root);
        }
        else
        {
            d[v] = INFINITE;
            set_vt(v, 0);
        }

#ifdef DEBUG
        printf("d[%d]=%0.2f\tv_t[%d]=%d\tvisited[%d]=%d\n", v, d[v] /*get_d(d, v)*/, v, get_vt(v), v, get_visited(v));
#endif
    }

    // lançar os processos
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].graph_size = graph_size;

        // calcular os vértices de início e fim com base no número de vértices a processar
        int start_col = (i * num_vertices) + 1;
        int end_col = start_col + num_vertices - 1;

        thread_data[i].start_col = start_col;
        thread_data[i].end_col = end_col;

        thread_data[i].local_graph = split_graph(graph, start_col, end_col, graph_size);
        thread_data[i].local_d = split_d(d, start_col, end_col, graph_size);
        // thread_data[i].min_u = min_u;

#ifdef TRACE
        print_graph_mt(thread_data[i].local_graph, start_col, end_col, graph_size);
#endif

        process_error("worker_thread create", pthread_create(&threads[i], NULL, prim_mst, (void *)&thread_data[i]));
    }

    // while vt != v
    while (!all_visited(graph_size))
    {

#ifdef DEBUG
        printf("Main hold at barrier\n");
#endif
        pthread_barrier_wait(&barrier);

#ifdef DEBUG
        printf("Main resume from barrier\n");
#endif
        set_min_u(0);
        int min_weight = INFINITE;

        for (int i = 0; i < num_threads; i++)
        {
            if (get_global_weight(i) < min_weight)
            {
                set_min_u(get_global_u(i));
                min_weight = get_global_weight(i);
            }
        }

#ifdef DEBUG
        printf("broadcast global_min_u: %d\n", get_min_u());
#endif

        // fazer a transmissão
        pthread_cond_broadcast(&cond);
    }

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
    process_error("barrier destroy", pthread_barrier_destroy(&barrier));
    process_error("condition destroy", pthread_cond_destroy(&cond));

#ifdef DEBUG
    printf("print_mst_mt ended\n");
#endif
    return v_t;
}

void *prim_mst(void *arg)
{
    thread_data *data = (thread_data *)arg;

#ifdef DEBUG
    printf("thread %d: start_col = %d || end_col = %d\n", data->thread_id, data->start_col, data->end_col);

    // print_graph_mt(local_graph, data->start_col, data->end_col, data->graph_size);
#endif
    while (!all_visited(data->graph_size))
    {
        for (int v = data->start_col; v <= data->end_col; v++)
        {
            // excluir v-v_t
            if (get_visited(v))
                continue;

            // obter o vértice u
            int u = get_u(data->local_d, v, data->start_col, data->end_col);

#ifdef DEBUG
            printf("[thread %d] Found u: %d (v=%d)\n", data->thread_id, u, v);
#endif

            // enviar para ser avaliado
            set_global_u(data->thread_id, u);
            set_global_weight(data->thread_id, get_edge(data->local_graph, v, u));

#ifdef DEBUG
            printf("Thread %d hold at barrier [%d]\n", data->thread_id, v);
#endif

            pthread_barrier_wait(&barrier);
#ifdef DEBUG
            printf("Thread %d resume from barrier [%d]\n", data->thread_id, v);
#endif

#ifdef DEBUG
            printf("Thread %d hold at condition [%d]\n", data->thread_id, v);
#endif

            pthread_mutex_lock(&mutex_wait);
            pthread_cond_wait(&cond, &mutex_wait);
            pthread_mutex_unlock(&mutex_wait);

#ifdef DEBUG
            printf("Thread %d resume from condition [%d]\n", data->thread_id, v);
#endif

            // se tiver sido escolhido o u deste processo, adicioná-lo a v_t
            int local_min_u = get_min_u();
            if (local_min_u == u)
                set_visited(u, true);
            else
                u = local_min_u;

            for (int i = 1; i <= data->graph_size; i++)
            {
                // excluir a diagonal e v-v_t
                if (get_visited(i) || i == u)
                {
#ifdef DEBUG
                    printf("Excluding v = %d\n", i);
#endif
                    continue;
                }

                float u_weight = get_edge(data->local_graph, u, i);
                float d_weight = data->local_d[i]; // get_d(data->local_d, i);

                if (u_weight == INFINITE)
                    continue;

                if (u_weight < d_weight)
                {
                    data->local_d[i] = u_weight;
                    set_vt(i, u);
                }
            }
        }
    }
#ifdef DEBUG
    for (int i = data->start_col; i <= data->end_col; i++)
    {
        printf("Thread %d=> d[%d]=%0.2f\tv_t[%d]=%d\n", data->thread_id, i, data->local_d[i], i, get_vt(i));
    }
    printf("Thread %d finished\n", data->thread_id);
#endif

    pthread_exit(NULL);
}

/*
    get_u

    obtém o vértice com o menor peso em d
    dos que pertencem a v-v_t
*/
int get_u(float *d, int v, int start_col, int end_col)
{
    int u_min = get_vt(v);
    float min_weight = d[v]; // get_d(d, v);

    for (int u = start_col; u <= end_col; u++)
    {
        // excluir os já visitados (v-vt)
        if (get_visited(u) || u == v)
            continue;

        float d_weight = d[u]; // get_d(d, u);

        if (d_weight < min_weight)
        {
            u_min = u;
            min_weight = d_weight;
        }
    }

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
            split_d[i] = -1;
        }
#ifdef TRACE
        printf("split_d[%d]=%0.2f\tv_t[%d]=%d\n", i, split_d[i] /*get_d(d, v)*/, i, get_vt(i));
#endif
    }

    return split_d;
}

bool all_visited(int graph_size)
{
    for (int i = 1; i <= graph_size; i++)
    {
        if (!get_visited(i))
            return false;
    }
    return true;
}

/******************************************************************************
 * Funções para aceder aos recursos partilhados
 *******************************************************************************/
void set_vt(int index, int value)
{
    process_error("lock vt", pthread_mutex_lock(&mutex_vt));
    v_t[index] = value;
    process_error("unlock vt", pthread_mutex_unlock(&mutex_vt));
}

int get_vt(int index)
{
    process_error("lock vt", pthread_mutex_lock(&mutex_vt));
    float result = v_t[index];
    process_error("unlock vt", pthread_mutex_unlock(&mutex_vt));
    return result;
}

void set_visited(int index, bool value)
{
    process_error("lock visited", pthread_mutex_lock(&mutex_visited));
    visited[index] = value;
    process_error("unlock visited", pthread_mutex_unlock(&mutex_visited));
}

bool get_visited(int index)
{
    process_error("lock visited", pthread_mutex_lock(&mutex_visited));
    bool result = visited[index];
    process_error("unlock visited", pthread_mutex_unlock(&mutex_visited));
    return result;
}

void set_min_u(int value)
{
    process_error("lock minu", pthread_mutex_lock(&mutex_minu));
    min_u = value;
    process_error("unlock minu", pthread_mutex_unlock(&mutex_minu));
}

int get_min_u()
{
    process_error("lock minu", pthread_mutex_lock(&mutex_minu));
    int result = min_u;
    process_error("unlock minu", pthread_mutex_unlock(&mutex_minu));
    return result;
}

void set_global_u(int index, int value)
{
    process_error("lock globalu", pthread_mutex_lock(&mutex_globalu));
    global_u[index] = value;
    process_error("unlock globalu", pthread_mutex_unlock(&mutex_globalu));
}

int get_global_u(int index)
{
    process_error("lock global_u", pthread_mutex_lock(&mutex_globalu));
    int result = global_u[index];
    process_error("unlock global_u", pthread_mutex_unlock(&mutex_globalu));
    return result;
}

void set_global_weight(int index, float value)
{
    process_error("lock global_weight", pthread_mutex_lock(&mutex_globalweight));
    global_weight[index] = value;
    process_error("unlock global_weight", pthread_mutex_unlock(&mutex_globalweight));
}

float get_global_weight(int index)
{
    process_error("lock globalweight", pthread_mutex_lock(&mutex_globalweight));
    float result = global_weight[index];
    process_error("unlock globalweight", pthread_mutex_unlock(&mutex_globalweight));
    return result;
}

void initialize_mutexes(void)
{
    process_error("mutex_minu init", pthread_mutex_init(&mutex_minu, NULL));
    process_error("mutex_globalu", pthread_mutex_init(&mutex_globalu, NULL));
    process_error("mutex_globalweight", pthread_mutex_init(&mutex_globalweight, NULL));
    process_error("mutex_vt init", pthread_mutex_init(&mutex_vt, NULL));
    process_error("mutex_wait init", pthread_mutex_init(&mutex_wait, NULL));
    process_error("mutex_finish_count init", pthread_mutex_init(&mutex_finish_count, NULL));
    process_error("mutex_visited init", pthread_mutex_init(&mutex_visited, NULL));
}

void destroy_mutexes(void)
{
    process_error("mutex_minu destroy", pthread_mutex_destroy(&mutex_minu));
    process_error("mutex_globalu destroy", pthread_mutex_destroy(&mutex_globalu));
    process_error("mutex_globalweight destroy", pthread_mutex_destroy(&mutex_globalweight));
    process_error("mutex_vt destroy", pthread_mutex_destroy(&mutex_vt));
    process_error("mutex_wait destroy", pthread_mutex_destroy(&mutex_wait));
    process_error("mutex_finish_count destroy", pthread_mutex_destroy(&mutex_finish_count));
    process_error("mutex_visited destroy", pthread_mutex_init(&mutex_visited, NULL));
}

void process_error(char *name, int result)
{
    if (result != 0)
        printf("%s has returned an error\n", name);
}