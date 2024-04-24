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
    int num_threads;
    int graph_size;
    int start_col;
    int end_col;
    float *local_graph; // V
} ThreadData;

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

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_barrier_t thread_barrier;
pthread_barrier_t barrier;

// variáveis partilhadas
int min_u, finish_count;
int *global_u;
float *global_weight;
int *v_t;

static int get_u(float *d, int *v_t, bool *visited, int graph_size);
static float get_edge_mt(float *local_graph, int col, int row, int num_vertices);
static int get_vertice_mt(int v, int num_vertices);

static void set_vt(int *v_t, int index, float value);

static float *split_graph(float *graph, int start_col, int end_col, int graph_size);

static int get_min_u(void);
static void set_min_u(int value);
static int get_global_u(int index);
static void set_global_u(int index, int value);
static float get_global_weight(int index);
static void set_global_weight(int index, float value);
static int get_finish_count(void);
static void set_finish_count(void);

static void process_error(char *name, int result);

static void initialize_mutexes(void);
static void destroy_mutexes(void);

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

    v_t = malloc((graph_size + 1) * sizeof(int));
    global_u = malloc((num_threads + 1) * sizeof(int));
    global_weight = malloc((num_threads + 1) * sizeof(float));

    // inicializar os vetores
    if (global_u != NULL)
        memset(global_u, 0, (num_threads + 1) * sizeof(int));

    if (global_weight != NULL)
        memset(global_weight, 0, (num_threads + 1) * sizeof(float));

    // inicializar o contador de término das tarefas
    finish_count = 0;

    process_error("pthread_cond_init", pthread_cond_init(&cond, NULL));

    process_error("barrier_init", pthread_barrier_init(&barrier, NULL, num_threads));

    initialize_mutexes();

    // tarefas acessórias
    ThreadData thread_data[num_threads];

    // calcular o número de vértices (n) a alocar a cada processo
    int num_vertices = graph_size / num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].graph_root = graph_root;
        thread_data[i].num_vertices = num_vertices;
        thread_data[i].graph_size = graph_size;
        thread_data[i].num_threads = num_threads;

        // calcular os vértices de início e fim com base no número de vértices a processar
        int start_col = (i * num_vertices) + 1;
        int end_col = start_col + num_vertices - 1;

        thread_data[i].start_col = start_col;
        thread_data[i].end_col = end_col;

        // criar o subconjunto de V a alocar à tarefa
        thread_data[i].local_graph = split_graph(graph, start_col, end_col, graph_size);

        process_error("worker_thread create", pthread_create(&threads[i], NULL, prim_mst, (void *)&thread_data[i]));
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], (void **)NULL);
    }

    free(global_u);
    free(global_weight);

    destroy_mutexes();
    pthread_barrier_destroy(&barrier);

    process_error("condition destroy", pthread_cond_destroy(&cond));

#ifdef DEBUG
    printf("print_mst_mt ended\n");
#endif
    return v_t;
}

void *prim_mst(void *arg)
{

    ThreadData *data = (ThreadData *)arg;
    int graph_root = data->graph_root;

    float *local_graph = data->local_graph; // thread safe, cada processo tem a sua cópia

    float *d = malloc((data->num_vertices + 1) * sizeof(float));
    bool *visited = malloc((data->num_vertices + 1) * sizeof(bool));

    int corrected_v = 0;
    int root_mt = 0;

#ifdef DEBUG
    printf("thread %d: start_col = %d || end_col = %d || num_vertices = %d\n", data->thread_id, data->start_col, data->end_col, data->num_vertices);

    print_graph_mt(local_graph, data->start_col, data->end_col, data->graph_size);
#endif

    // processar graph_root apenas se estiver nos vértices
    // que foram atribuídos a este processo
    if (data->start_col < graph_root && graph_root < data->end_col)
    {
        set_vt(v_t, graph_root, 0);

        // corrigir graph_root para as variáveis locais
        root_mt = get_vertice_mt(graph_root, data->num_vertices);

        d[root_mt] = 0;
        visited[root_mt] = true;
    }

    // como v_t é global, v tem de manter o valor original
    // e os ciclos são sempre feitos em relação a v global

    // inicializar a árvore mínima
    for (int v = data->start_col; v <= data->end_col; v++)
    {
        // obter o v para os vetores locais
        corrected_v = get_vertice_mt(v, data->num_vertices);

        // como v é global, a avaliação contra a raíz
        // também tem de ser global
        if (v != graph_root)
        {
            visited[corrected_v] = false;
        }

        float weight = get_edge(local_graph, graph_root, v);

        if (weight < INFINITE)
        {
            d[corrected_v] = weight;

            set_vt(v_t, v, graph_root);
        }
        else
        {
            d[corrected_v] = INFINITE;

            set_vt(v_t, v, 0); // marcado a 0 pois não há nenhum vértice 0
        }

#ifdef DEBUG
        printf("d[%d]=%0.2f v_t[%d]=%d\n", v, d[corrected_v], v, v_t[v]);
#endif
    }
    //     else
    //     {
    //         // senão fazer a inicialização simples
    //         for (int v = data->start_col; v <= data->end_col; v++)
    //         {
    //             corrected_v = get_vertice_mt(v, data->num_vertices);

    //             visited[corrected_v] = false;

    //             d[corrected_v] = INFINITE;

    //             set_vt(v_t, v, 0); // marcado a 0 pois não há nenhum vértice 0

    // #ifdef DEBUG
    //             printf("d[%d]=%0.2f v_t[%d]=%d\n", v, d[corrected_v], v, v_t[v]);
    // #endif
    //         }
    //     }

    for (int v = data->start_col; v <= data->end_col; v++)
    {
        corrected_v = get_vertice_mt(v, data->num_vertices);

        // excluir a raíz e v-v_t
        if (corrected_v == graph_root || visited[corrected_v])
            continue;

        // obter o vértice u
        int u = get_u(d, v_t, visited, data->num_vertices);

#ifdef DEBUG
        printf("[thread %d] Found u: %d (v=%d)\n", data->thread_id, u, v);
#endif
/* TODO: se fôr o processo 0, fica a aguardar todos os u dos outros processos
 para depois obter o global_u
 a seguir envia-o para os processos
    se não fôr o processo 0, envia o u para o processo 0 e fica a aguardar o u_globla
    se u == u_global então regista-o em v_t local, senão simplesmente faz u = u_global
    e passa ao próximo
*/
// Lock and update the global result
#ifdef TRACE
        printf("Thread %d check and update global result\n", data->thread_id);
#endif

        if (get_edge_mt(local_graph, corrected_v, u, data->num_vertices) < get_global_weight(data->thread_id))
        {
            set_global_u(data->thread_id, u);
            set_global_weight(data->thread_id, get_edge_mt(local_graph, corrected_v, u, data->num_vertices));
        }

#ifdef TRACE
        printf("Thread %d check and update global result done\n", data->thread_id);
#endif
        // se fôr o processo 0, obter o u geral
        if (data->thread_id == 0)
        {
#ifdef DEBUG
            printf("Thread %d hold\n", data->thread_id);
#endif

            // aguardar que as tarefas tenham encontrado o seu u
            if (get_finish_count() <= data->num_threads - 1)
                pthread_barrier_wait(&barrier);

#ifdef DEBUG
            printf("Thread %d resume\n", data->thread_id);
#endif

            // o processo 0 é responsável por determinar o u global
            set_min_u(0);
            int min_weight = INFINITE;

            for (int i = 0; i < data->num_threads; i++)
            {
                if (get_global_weight(i) < min_weight)
                {
                    set_min_u(get_global_u(i));
                    min_weight = get_global_weight(i);
                }
            }

#ifdef DEBUG
            printf("min_u: %d\n", get_min_u());
            printf("finish_count: %d\n", get_finish_count());
#endif

            // fazer a transmissão
            pthread_cond_broadcast(&cond);
        }
        else
        {
// os outros processos ficam à espera do resultado
#ifdef DEBUG
            printf("Thread %d hold\n", data->thread_id);
#endif

            pthread_barrier_wait(&barrier);

#ifdef DEBUG
            printf("Thread %d resume\n", data->thread_id);
#endif

            pthread_cond_wait(&cond, &mutex_wait);
        }

        // se tiver sido escolhido o u deste processo, adicioná-lo a v_t
        if (get_min_u() == u)
            visited[u] = true;
        else
            u = get_min_u();

        for (int i = v; i <= data->end_col; i++)
        {
            corrected_v = get_vertice_mt(i, data->num_vertices);

            // excluir a diagonal e v-v_t
            if (i == u || visited[corrected_v])
                continue;

            // aqui é necessário corrigir o vértice
            float u_weight = get_edge_mt(local_graph, u, i, data->num_vertices);

            if (u_weight == 0)
                continue;

            if (u_weight < d[i])
            {
                d[corrected_v] = u_weight;

                set_vt(v_t, i, u);
            }

#ifdef TRACE
            printf("[thread %d](%d,%d) => weight: %f | d[v]: %f | v_t[%d]: %d\n", data->thread_id, u, i, u_weight, d[corrected_v], i, v_t[i]);
#endif
        }
    }

    // sinalizar que já terminou de processar o bloco
    set_finish_count();

#ifdef DEBUG
    printf("Thread %d finished\n", get_finish_count());
#endif
    free(visited);
    free(d);

    pthread_exit(NULL);
}

int get_u(float *d, int *v_t, bool *visited, int graph_size)
{

    int u_min = 0;
    float min_weight = INFINITE;

    for (int u = 1; u <= graph_size; u++)
    {
        // excluir os já visitados (v-vt)
        if (visited[u])
            continue;

        if (d[u] < min_weight)
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
    if (graph == NULL)
        return NULL;

    unsigned long long matrix_size = get_matrix_size(graph_size);
    float *split_graph = malloc(matrix_size * sizeof(float));

    for (int col = 1; col <= graph_size; col++)
    {
        // apenas processar as linhas que pertencem
        // às colunas pedidas
        if (col >= start_col && col <= end_col)
        {

            for (int row = 1; row < graph_size; row++)
            {
                // se linha = coluna o vértice ligar-se-ia a ele mesmo
                // se linha > coluna estou na triangular inferior
                // em ambos os casos passo à frente
                if (row >= col)
                    continue;

                add_edge(split_graph, col, row, get_edge(graph, col, row));
            }
        }
        else
        {
            for (int row = 1; row < graph_size; row++)
            {
                // se linha = coluna o vértice ligar-se-ia a ele mesmo
                // se linha > coluna estou na triangular inferior
                // em ambos os casos passo à frente
                if (row >= col)
                    continue;

                add_null_edge(split_graph, col, row);
            }
        }
    }

    return split_graph;
}

/*
    get_edge_mt

    obtém a aresta sempre com a coluna corrigida para o grafo local
*/
float get_edge_mt(float *local_graph, int col, int row, int num_vertices)
{
    /*
        fazer a inversão para a triangular superior aqui
        pois col tem de ser corrigida para o grafo local
        *antes* de ser enviada para get_edge()
    */
    if (col < row)
    {
        int temp = col;
        col = row;
        row = temp;
    }

    col = get_vertice_mt(col, num_vertices);

    return get_edge(local_graph, col, row);
}

/*
    get_vertice_mt

    corrige o vértice dado para o vetor local
    apenas considera quando v > num_vertices pois isso
    indica que está em P_i, i>0

    para P_0 não é necessária correção
*/
int get_vertice_mt(int v, int num_vertices)
{
#ifdef TRACE
    int uncorrected_vertice = v;
    if (v > num_vertices)
    {
        printf("vertice to correct: %d (num_vertices: %d)\n", v, num_vertices);
        v -= num_vertices;
    }
#else

    if (v > num_vertices)
    {
        v -= num_vertices;
    }
#endif

#ifdef TRACE
    if (uncorrected_vertice != v)
        printf("corrected vertice: %d\n", v);
#endif
    return v;
}

/******************************************************************************
 * Funções para aceder aos recursos partilhados
 *******************************************************************************/
void set_vt(int *v_t, int index, float value)
{
    process_error("lock vt", pthread_mutex_lock(&mutex_vt));
    v_t[index] = value;
    process_error("unlock vt", pthread_mutex_unlock(&mutex_vt));
}

void set_min_u(int value)
{
    process_error("lock minu", pthread_mutex_lock(&mutex_minu));
    min_u = value;
    process_error("unlock minu", pthread_mutex_unlock(&mutex_minu));
}

int get_min_u(void)
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
    process_error("lock globalu", pthread_mutex_lock(&mutex_globalu));
    int result = global_u[index];
    process_error("unlock globalu", pthread_mutex_unlock(&mutex_globalu));
    return result;
}

void set_global_weight(int index, float value)
{
    process_error("lock globalweight", pthread_mutex_lock(&mutex_globalweight));
    global_weight[index] = value;
    process_error("unlock globalweight", pthread_mutex_unlock(&mutex_globalweight));
}

float get_global_weight(int index)
{
    process_error("lock globalweight", pthread_mutex_lock(&mutex_globalweight));
    float result = global_weight[index];
    process_error("unlock globalweight", pthread_mutex_unlock(&mutex_globalweight));
    return result;
}

void set_finish_count(void)
{
    process_error("lock finish_count", pthread_mutex_lock(&mutex_finish_count));
    finish_count++;
    process_error("unlock finish_count", pthread_mutex_unlock(&mutex_finish_count));
}

int get_finish_count(void)
{
    process_error("lock finish_count", pthread_mutex_lock(&mutex_finish_count));
    int result = finish_count;
    process_error("unlock finish_count", pthread_mutex_unlock(&mutex_finish_count));
    return result;
}

void initialize_mutexes(void)
{
    process_error("mutex_minu init", pthread_mutex_init(&mutex_minu, NULL));
    process_error("mutex_globalu", pthread_mutex_init(&mutex_globalu, NULL));
    process_error("mutex_globalweight", pthread_mutex_init(&mutex_globalweight, NULL));
    process_error("mutex_vt init", pthread_mutex_init(&mutex_vt, NULL));
    process_error("mutext_wait init", pthread_mutex_init(&mutex_wait, NULL));
    process_error("mutext_finish_count init", pthread_mutex_init(&mutex_finish_count, NULL));
}

void destroy_mutexes(void)
{
    process_error("mutex_minu destroy", pthread_mutex_destroy(&mutex_minu));
    process_error("mutex_globalu destroy", pthread_mutex_destroy(&mutex_globalu));
    process_error("mutex_globalweight destroy", pthread_mutex_destroy(&mutex_globalweight));
    process_error("mutex_vt destroy", pthread_mutex_destroy(&mutex_vt));
    process_error("mutex_wait destroy", pthread_mutex_destroy(&mutex_wait));
    process_error("mutex_finish_count destroy", pthread_mutex_destroy(&mutex_finish_count));
}

void process_error(char *name, int result)
{
    if (result != 0)
        printf("%s has returned an error\n", name);
}