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

// argumentos para os processos
typedef struct _thread_data
{
    int thread_id;
    int graph_size;
    int start_col;
    int end_col;
    int num_threads;
    int cpu_id;
} thread_data;

// mensagem
typedef struct _message
{
    pthread_cond_t wait;
    pthread_mutex_t lock;
    int u;
    bool *ready;
} message;

// mutexes e condições
pthread_mutex_t mutex_lock;
pthread_mutex_t mutex_visited;

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

float *d;
float *local_graph;

/*
    funções internas
*/
static int get_u(float *d, int start_col, int end_col);
static bool all_visited(int graph_size);

static void process_error(char *name, int result);

static void *worker_prim(void *arg);

int *prim_mt_mst(float *graph, int graph_size, int graph_root, int num_threads)
{
    // certificar que não se pedem mais processos que vértices
    if (num_threads > graph_size)
        num_threads = graph_size;

    // determinar o número de cpus disponíveis
    int num_available_cpus = sysconf(_SC_NPROCESSORS_ONLN);

    // não criar mais processos que os cpus disponíveis
    if (num_threads > num_available_cpus)
        num_threads = num_available_cpus;

    pthread_t threads[num_threads];

    // alocar os recursos partilhados
    v_t = calloc(graph_size + 1, sizeof(int));
    visited = calloc(graph_size + 1, sizeof(bool));
    d = calloc(graph_size + 1, sizeof(float));
    global_u = calloc(num_threads, sizeof(int));
    global_weight = calloc(num_threads, sizeof(float));

    min_u = 0;
    min_weight = INFINITE;
    all_vertices_visited = false;

    broadcast_message.ready = false;
    broadcast_message.u = 0;
    broadcast_message.ready = calloc(num_threads, sizeof(bool));

    // inicializar mutexes e condições

    process_error("broadcast_message condition init", pthread_cond_init(&broadcast_message.wait, NULL));
    process_error("broadcast_message lock init", pthread_mutex_init(&broadcast_message.lock, NULL));

    process_error("cond init", pthread_cond_init(&cond, NULL));
    process_error("cond_wait init", pthread_cond_init(&cond_wait, NULL));

    process_error("mutex_lock init", pthread_mutex_init(&mutex_lock, NULL));
    process_error("mutex lock visited init", pthread_mutex_init(&mutex_visited, NULL));

    process_error("barrier_wait init", pthread_barrier_init(&barrier_wait, NULL, num_threads + 1));
    process_error("barrier_visited init", pthread_barrier_init(&barrier_visited, NULL, num_threads + 1));

    // tarefas acessórias
    thread_data thread_data[num_threads];

    // calcular o número de vértices (n) a alocar a cada processo
    // divisão pode não ser inteira, por isso o resultado é arredondado
    int num_vertices = round(graph_size / num_threads);
    int last_end_col = 0;

    int start_col = 0;
    int end_col = 0;

    // inicializar a árvore mínima
    v_t[graph_root] = graph_root;
    d[graph_root] = 0;
    visited[0] = false;

    // enviar graph para o local de MT
    local_graph = graph;

    for (int v = 1; v <= graph_size; v++)
    {
        visited[v] = (v == graph_root);
        float weight = get_edge(graph, graph_root, v);
        d[v] = (weight < INFINITE) ? weight : INFINITE;
        v_t[v] = (weight < INFINITE) ? graph_root : 0;
    }

    // variável para controlar o cpu ao qual entregar o processo que está a ser criado
    int cpu_step = 0;

    // lançar os processos
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].graph_size = graph_size;
        thread_data[i].num_threads = num_threads;
        thread_data[i].cpu_id = cpu_step;

        // calcular os vértices de início e fim com base no número de vértices a processar
        start_col = (i * num_vertices) + 1;
        end_col = start_col + num_vertices - 1;
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
        cpu_step++;

        // se o número de processos pedidos exceder o número de cpus
        // é feito aqui o reset de novo para 0
        if (cpu_step > num_available_cpus)
            cpu_step = 0;

        process_error("worker_thread create", pthread_create(&threads[i], NULL, worker_prim, (void *)&thread_data[i]));
    }

    do
    {
        pthread_barrier_wait(&barrier_wait);

        pthread_mutex_lock(&broadcast_message.lock);

        broadcast_message.u = 0;

        pthread_cond_broadcast(&broadcast_message.wait);

        for (int i = 0; i < num_threads; i++)
        {
            /* se fôr entregue um u = 0 é porque o processo
                já visitou todos os vértices que lhe pertencem
                ou estes não têm ligação a nenhum vértice pois
                ainda estão como inicializados (peso infinito)

                de qualquer maneira são omitidos do u global
            */
            if (global_u[i] != 0 && global_weight[i] < min_weight)
            {
                min_u = global_u[i];
                min_weight = global_weight[i];
            }

            broadcast_message.ready[i] = true;
        }

        broadcast_message.u = min_u;
        pthread_cond_broadcast(&broadcast_message.wait);
        pthread_mutex_unlock(&broadcast_message.lock);
        pthread_barrier_wait(&barrier_visited);
        all_vertices_visited = all_visited(graph_size);

        // fazer reset ao u encontrado previamente
        min_u = 0;
        min_weight = INFINITE;

    } while (!all_vertices_visited);

    // recuperar os processos criados
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], (void **)NULL);
    }

    // libertar a memória alocada pelo processo
    free(global_u);
    free(global_weight);
    free(d);
    free(visited);

    // destruir os mutexes e condições
    process_error("broadcast_message cond destroy", pthread_cond_destroy(&broadcast_message.wait));
    process_error("broadcast_message mutex destroy", pthread_mutex_destroy(&broadcast_message.lock));

    process_error("mutex_lock destroy", pthread_mutex_destroy(&mutex_lock));
    process_error("mutex lock visited destroy", pthread_mutex_destroy(&mutex_visited));

    process_error("condition destroy", pthread_cond_destroy(&cond));
    process_error("condition wait destroy", pthread_cond_destroy(&cond_wait));

    process_error("barrier_wait destroy", pthread_barrier_destroy(&barrier_wait));
    process_error("barrier_visited destroy", pthread_barrier_destroy(&barrier_visited));

    return v_t;
}

/*
    worker_prim

    obtém os vértices u e atualiza d para os vértices atribuídos ao processo
*/
void *worker_prim(void *arg)
{
    thread_data *data = (thread_data *)arg;
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(data->cpu_id, &cpuset);

    // enviar o processo para o cpu que lhe foi atribuído
    pthread_t thread = pthread_self();
    process_error("thread affinity set", pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset));

    bool local_all_visited = false;

    do
    {
        // obter o vértice u
        int u = get_u(d, data->start_col, data->end_col);

        // enviar para ser avaliado
        pthread_mutex_lock(&mutex_lock);

        global_u[data->thread_id] = u;
        global_weight[data->thread_id] = d[u];

        pthread_mutex_unlock(&mutex_lock);

        pthread_barrier_wait(&barrier_wait);

        // aguardar pela mensagem com o u mínimo
        pthread_mutex_lock(&broadcast_message.lock);

        while (!broadcast_message.ready[data->thread_id])
        {
            pthread_cond_wait(&broadcast_message.wait, &broadcast_message.lock);
        }

        u = broadcast_message.u;
        broadcast_message.ready[data->thread_id] = false;

        pthread_mutex_unlock(&broadcast_message.lock);

        // se tiver sido escolhido o u deste processo adicioná-lo a v-v_t
        if (u >= data->start_col && u <= data->end_col)
        {
            pthread_mutex_lock(&mutex_visited);
            visited[u] = true;
            pthread_mutex_unlock(&mutex_visited);
        }

        // processar todos os vértices que lhe foram atribuídos em relação a u
        for (int i = data->start_col; i <= data->end_col; i++)
        {
            // excluir a diagonal e v-v_t
            if (visited[i] || i == u)
            {
                continue;
            }

            float u_weight = get_edge(local_graph, u, i);

            // se não tiver ligação, omitir
            if (u_weight == INFINITE)
                continue;

            if (/*u_weight != INFINITE &&*/ u_weight < d[i])
            {
                pthread_mutex_lock(&mutex_lock);
                d[i] = u_weight;
                v_t[i] = u;

                pthread_mutex_unlock(&mutex_lock);
            }
        }

        pthread_barrier_wait(&barrier_visited);
        // revalidar se todos os vértices já foram visitados
        local_all_visited = all_visited(data->graph_size);

    } while (!local_all_visited);

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
    all_visited

    avalia se todos os vértices do grafo já foram visitados

    -- thread safe --
*/
bool all_visited(int graph_size)
{
    pthread_mutex_lock(&mutex_visited);
    for (int i = 1; i <= graph_size; i++)
    {
        // sair assim que aparecer o primeiro vértice não visitado
        if (!visited[i])
        {
            pthread_mutex_unlock(&mutex_visited);
            return false;
        }
    }
    pthread_mutex_unlock(&mutex_visited);
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
