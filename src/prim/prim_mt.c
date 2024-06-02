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
    int num_threads;
    float *local_graph;
    float *local_d;
} thread_data;

typedef struct _message
{
    pthread_cond_t wait;
    pthread_mutex_t lock;
    int u;
    bool *ready;
} message;

pthread_mutex_t mutex_lock;

// condições para os processos
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_wait = PTHREAD_COND_INITIALIZER;

pthread_barrier_t barrier_wait;
pthread_barrier_t barrier_visited;

// variáveis partilhadas
int min_u;
float min_weight;
int *global_u;
float *global_weight;
int *v_t;
bool *visited;
bool all_vertices_visited;
message broadcast_message;

/*
    funções internas
*/
static int get_u(float *d, int start_col, int end_col);
static float *split_graph(float *graph, int start_col, int end_col, int graph_size);
static float *split_d(float *d, int start_col, int end_col, int graph_size);
static bool all_visited(int graph_size);

/*
    funções para lidar com os processos
*/
static void process_error(char *name, int result);

static void *worker_prim(void *arg);

int *prim_mt_mst(float *graph, int graph_size, int graph_root, int num_threads)
{
    struct timeval start, end;

    // certificar que não se pedem mais processos que vértices
    if (num_threads > graph_size)
        num_threads = graph_size;

    pthread_t threads[num_threads];

    // alocar os recursos partilhados
    v_t = calloc(graph_size + 1, sizeof(int));
    visited = calloc(graph_size + 1, sizeof(bool));
    float *d = calloc(graph_size + 1, sizeof(float));
    global_u = calloc(num_threads, sizeof(int));
    global_weight = calloc(num_threads, sizeof(float));

    min_u = 0;
    min_weight = INFINITE;
    all_vertices_visited = false;

    broadcast_message.ready = false;
    broadcast_message.u = 0;
    broadcast_message.ready = calloc(num_threads, sizeof(bool));

    process_error("broadcast_message condition init", pthread_cond_init(&broadcast_message.wait, NULL));
    process_error("broadcast_message lock init", pthread_mutex_init(&broadcast_message.lock, NULL));

    // inicializar condições
    process_error("cond init", pthread_cond_init(&cond, NULL));
    process_error("cond_wait init", pthread_cond_init(&cond_wait, NULL));
    process_error("mutex_lock init", pthread_mutex_init(&mutex_lock, NULL));
    process_error("barrier_wait init", pthread_barrier_init(&barrier_wait, NULL, num_threads + 1));
    process_error("barrier_visited init", pthread_barrier_init(&barrier_visited, NULL, num_threads + 1));

    // tarefas acessórias
    thread_data thread_data[num_threads];

    // calcular o número de vértices (n) a alocar a cada processo
    // divisão pode não ser inteira, por isso o resultado é arredondado
    int num_vertices = round(graph_size / num_threads);
    int last_end_col = 0;

    // inicializar a árvore mínima
    v_t[graph_root] = graph_root;
    d[graph_root] = 0;
    visited[0] = false;

    for (int v = 1; v <= graph_size; v++)
    {
        visited[v] = (v == graph_root);
        float weight = get_edge(graph, graph_root, v);
        d[v] = (weight < INFINITE) ? weight : INFINITE;
        v_t[v] = (weight < INFINITE) ? graph_root : 0;
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
        last_end_col = end_col;

        thread_data[i].start_col = start_col;

        /*
            na preparação do último processo ter em conta
            que a distribuição de vértices não é igual
            quando a divisão n/p não tem resto 0
        */
        if (i == num_threads - 1)
        {
            end_col = end_col + (graph_size - last_end_col);
        }

        thread_data[i].end_col = end_col;

        thread_data[i].local_graph = split_graph(graph, start_col, end_col, graph_size);
        thread_data[i].local_d = split_d(d, start_col, end_col, graph_size);

#ifdef TRACE
        print_graph_mt(thread_data[i].local_graph, start_col, end_col, graph_size);
#endif

        process_error("worker_thread create", pthread_create(&threads[i], NULL, worker_prim, (void *)&thread_data[i]));
    }

    // começar a cronometrar a execução
    gettimeofday(&start, NULL);

    do
    {

#ifdef DEBUG
        printf("[thread main] waiting for worker threads\n");
#endif
        pthread_barrier_wait(&barrier_wait);

#ifdef DEBUG
        printf("[thread main] compute global_u\n");
#endif

        pthread_mutex_lock(&broadcast_message.lock);

        broadcast_message.u = 0;

        pthread_cond_broadcast(&broadcast_message.wait);

        // min_u = 0;
        // min_weight = INFINITE;

        for (int i = 0; i < num_threads; i++)
        {
            /* se fôr entregue um u = 0 é porque o processo
                já visitou todos os vértices que lhe pertencem
                ou estes não têm ligação a nenhum vértice pois
                ainda estão como inicializados (peso infinito)

                de qualquer maneira são omitidos do u global
            */
            // if (global_u[i] != 0 && global_weight[i] < min_weight)
            // {
            //     min_u = global_u[i];
            //     min_weight = global_weight[i];
            // }

#ifdef TRACE
            printf("global_u[%d] = %d\tglobal_weight[%d] = %0.3f\n", i, global_u[i], i, global_weight[i]);
#endif
            broadcast_message.ready[i] = true;
        }

        broadcast_message.u = min_u;
        pthread_cond_broadcast(&broadcast_message.wait);

#ifdef DEBUG
        printf("[thread main] broadcast global_u: %d\tmin_weight: %0.2f\n", min_u, min_weight);

#endif
#ifdef TRACE
        for (int v = 1; v <= graph_size; v++)
        {
            printf("d[%d]=%0.2f\tv_t[%d]=%d\tvisited[%d]=%d\n", v, d[v], v, v_t[v], v, visited[v]);
        }
#endif
        pthread_mutex_unlock(&broadcast_message.lock);

        pthread_barrier_wait(&barrier_visited);
        all_vertices_visited = all_visited(graph_size);
        min_u = 0;
        min_weight = INFINITE;

    } while (!all_vertices_visited);

    // recuperar os processos criados
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], (void **)NULL);
    }

    // emitir o tempo de execução do algoritmo
    gettimeofday(&end, NULL);

    double seconds = (double)(end.tv_sec - start.tv_sec);
    double microseconds = (double)(end.tv_usec - start.tv_usec);
    double elapsed_time = seconds + microseconds / 1e6;

    printf("Execution time: %.6f seconds\n", elapsed_time);

    // libertar a memória alocada pelo processo
    free(global_u);
    free(global_weight);
    free(d);
    free(visited);

    process_error("broadcast_message cond destroy", pthread_cond_destroy(&broadcast_message.wait));
    process_error("broadcast_message mutex destory", pthread_mutex_destroy(&broadcast_message.lock));

    process_error("mutex_lock destroy", pthread_mutex_destroy(&mutex_lock));
    process_error("condition destroy", pthread_cond_destroy(&cond));
    process_error("condition wait destroy", pthread_cond_destroy(&cond_wait));
    process_error("barrier_wait destroy", pthread_barrier_destroy(&barrier_wait));
    process_error("barrier_visited destroy", pthread_barrier_destroy(&barrier_visited));

#ifdef DEBUG
    printf("print_mst_mt ended\n");
#endif
    return v_t;
}

/*
    worker_prim

    obtém os vértices u e atualiza d para os vértices atribuídos ao processo
*/
void *worker_prim(void *arg)
{
    thread_data *data = (thread_data *)arg;
    bool local_all_visited = false;

#ifdef DEBUG
    printf("[thread %d]: start_col = %d || end_col = %d\n", data->thread_id, data->start_col, data->end_col);
#endif

    do
    {
        // obter o vértice u
        int u = get_u(data->local_d, data->start_col, data->end_col);

#ifdef DEBUG
        printf("[thread %d] Found u: %d (weight = %0.3f)\n", data->thread_id, u, data->local_d[u]);
#endif

        // enviar para ser avaliado
        pthread_mutex_lock(&mutex_lock);

        // global_u[data->thread_id] = u;
        // global_weight[data->thread_id] = data->local_d[u];

        if (data->local_d[u] < min_weight)
            min_u = u;

        pthread_mutex_unlock(&mutex_lock);

#ifdef DEBUG
        printf("[thread %d] waiting for main thread to compute global_u\n", data->thread_id);
#endif
        pthread_barrier_wait(&barrier_wait);

        pthread_mutex_lock(&broadcast_message.lock);

        while (!broadcast_message.ready[data->thread_id])
        {
            pthread_cond_wait(&broadcast_message.wait, &broadcast_message.lock);
        }

        u = broadcast_message.u;
        broadcast_message.ready[data->thread_id] = false;
#ifdef DEBUG
        printf("[thread %d] received u = %d\n", data->thread_id, u);
#endif
        pthread_mutex_unlock(&broadcast_message.lock);

        // se tiver sido escolhido o u deste processo, adicioná-lo a v_t
        if (u >= data->start_col && u <= data->end_col)
        {
            pthread_mutex_lock(&mutex_lock);
            visited[u] = true;
            pthread_mutex_unlock(&mutex_lock);
#ifdef DEBUG
            printf("[thread %d]  ===== set %d as visited ===== \n", data->thread_id, u);
#endif
        }

        for (int i = data->start_col; i <= data->end_col; i++)
        {
            // excluir a diagonal e v-v_t
            if (visited[i] || i == u)
            {
#ifdef TRACE
                printf("[thread %d] excluding v = %d\n", data->thread_id, i);
#endif
                continue;
            }
#ifdef TRACE
            else
                printf("[thread %d] processing v = %d for u = %d\n", data->thread_id, i, u);
#endif

            float u_weight = get_edge(data->local_graph, u, i);

            // se não tiver ligação, omitir
            if (u_weight == INFINITE)
                continue;

#ifdef TRACE
            printf("[thread %d]  u_weight (%d) = %f\td_weight (%d) = %f\n", data->thread_id, u, u_weight, i, data->local_d[i]);
#endif

            if (/*u_weight != INFINITE &&*/ u_weight < data->local_d[i])
            {
                data->local_d[i] = u_weight;
                pthread_mutex_lock(&mutex_lock);
                v_t[i] = u;

#ifdef DEBUG
                printf("[thread %d] ===> set d[%d]=%0.2f\tv_t[%d]=%d <=== \n", data->thread_id, i, data->local_d[i], i, v_t[i]);
#endif
                pthread_mutex_unlock(&mutex_lock);
            }
        }

        pthread_barrier_wait(&barrier_visited);
        // revalidar se todos os vértices já foram visitados
        local_all_visited = all_visited(data->graph_size);

#ifdef TRACE
        for (int v = 1; v <= data->graph_size; v++)
        {
            printf("[thread %d]\td[%d]=%0.2f\n", data->thread_id, v, data->local_d[v]);
        }
#endif
    } while (!local_all_visited);

#ifdef DEBUG
    pthread_mutex_lock(&mutex_lock);

    for (int i = data->start_col; i <= data->end_col; i++)
    {
        printf("[thread %d] set d[%d]=%0.2f\tv_t[%d]=%d\n", data->thread_id, i, data->local_d[i], i, v_t[i]);
    }
    pthread_mutex_unlock(&mutex_lock);

    printf("[thread %d] >>>>> finished <<<<<\n", data->thread_id);
#endif

    free(data->local_d);
    free(data->local_graph);

    pthread_exit(NULL);
}

/*
    get_u

    obtém o vértice com o menor peso em d
    dos que pertencem a v-v_t
*/
int get_u(float *d, int start_col, int end_col)
{
    int u_min = 0;
    float min_weight = INFINITE;

    for (int u = start_col; u <= end_col; u++)
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
    split_graph

    cria um subconjunto de graph, com início em start_col e fim em end_col
*/
float *split_graph(float *graph, int start_col, int end_col, int graph_size)
{
    unsigned long long matrix_size = get_matrix_size(graph_size);
    float *split_graph = calloc(matrix_size, sizeof(float));

    for (int col = start_col; col <= end_col; col++)
    {
        for (int row = 1; row <= graph_size; row++)
        {
            if (col != row)
            {
                add_edge(split_graph, col, row, get_edge(graph, col, row));
            }
        }
    }

    return split_graph;
}

/*
    split_d

    cria uma versão local do vetor d que contém apenas os valores
    que estão entre start_col e end_col
*/
float *split_d(float *d, int start_col, int end_col, int graph_size)
{
    float *split_d = malloc((graph_size + 1) * sizeof(float));

    memset(split_d, INFINITE, (graph_size + 1) * sizeof(float));
    // memcpy(split_d, d, (graph_size + 1) * sizeof(float));

    for (int i = start_col; i <= end_col; i++)
    {
        split_d[i] = d[i];
    }

    return split_d;
}

/*
    all_visited

    avalia se todos os vértices do grafo já foram visitados

    -- thread safe --
*/
bool all_visited(int graph_size)
{
    pthread_mutex_lock(&mutex_lock);
    for (int i = 1; i <= graph_size; i++)
    {
        // sair assim que aparecer o primeiro vértice não visitado
        if (!visited[i])
        {
            pthread_mutex_unlock(&mutex_lock);
            return false;
        }
    }
    pthread_mutex_unlock(&mutex_lock);
    return true;
}

/*
    process_error

    emite o erro conforme o valor de result
*/
void process_error(char *name, int result)
{
    if (result != 0)
        printf("%s has returned an error\n", name);
}
