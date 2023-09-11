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
    int start_vertice;
    int end_vertice;
    float *local_graph; // V
    float *d;           // sub_d
    bool *visited;
    int *v_t;
} ThreadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int min_u, min_weight;
int *global_u;

static int get_u(float *local_graph, float *d, int v, int *v_t, bool *visited, int graph_size);
static float get_corrected_edge(float *local_graph, int col, int row, int num_vertices);
static int get_corrected_vertice(int v, int num_vertices);

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
    float *d = malloc((graph_size + 1) * sizeof(float));
    bool *visited = malloc((graph_size + 1) * sizeof(bool));

    global_u = malloc(num_threads + 1);

    // calcular o número de vértices (n) a alocar a cada processo
    int num_vertices = graph_size / num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].graph_root = graph_root;
        thread_data[i].num_vertices = num_vertices;
        thread_data[i].graph_size = graph_size;
        thread_data[i].num_threads = num_threads;

        // passar os vetores globais aos processos
        thread_data[i].v_t = v_t;
        thread_data[i].d = d;
        thread_data[i].visited = visited;

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
    printf("thread %d: start_vertice = %d || end_vertice = %d || num_vertices = %d\n", data->thread_id, data->start_vertice, data->end_vertice, data->num_vertices);
#endif

    // alocar tendo em conta o número de vértices que foram atribuídos
    float *d = data->d;
    bool *visited = data->visited;

    // processar graph_root apenas se estiver nos vértices
    // que foram atribuídos a este processo
    if (data->start_vertice < graph_root && graph_root < data->end_vertice)
    {
        v_t[graph_root] = graph_root;
        d[graph_root] = 0;
        visited[graph_root] = true;

        // corrigir graph_root para local_graph
        int corrected_root = get_corrected_vertice(graph_root, data->num_vertices);

        // inicializar a árvore mínima
        for (int v = data->start_vertice; v <= data->end_vertice; v++)
        {
            if (v != graph_root)
                visited[v] = false;

            float weight = get_corrected_edge(local_graph, corrected_root, v, data->num_vertices);

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

        // inicializar o u global deste processo
        // data->global_u[data->thread_id] = 0;

        // obter o vértice u
        int u = get_u(local_graph, d, v, v_t, visited, data->num_vertices);

#ifdef DEBUG
        printf("(thread %d) Found u: %d for v: %d\n", data->thread_id, u, v);
#endif

        // Lock and update the global result
        pthread_mutex_lock(&mutex);
        global_u[data->thread_id] = u;
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);

        // o processo 0 é responsável por determinar o u global
        if (data->thread_id == 0)
        {
            min_u = 0;
            min_weight = INFINITE;

            for (int i = 0; i < data->num_threads; i++)
            {
                if (d[global_u[i]] < min_weight)
                {
                    min_u = global_u[i];
                    min_weight = d[global_u[i]];
                }
            }

            // fazer a transmissão
            pthread_cond_broadcast(&cond);
        }
        else
        {
            // os outros ficam a aguardar a transmissão
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_unlock(&mutex);

        // se tiver sido escolhido o u deste processo, adicioná-lo a v_t
        if (min_u == u)
            visited[u] = true;
        else
            u = min_u;

        for (int i = v; i <= data->end_vertice; i++)
        {
            // excluir a diagonal e v-v_t
            if (i == u || visited[i])
                continue;

            // aqui é necessário corrigir o vértice
            float u_weight = get_corrected_edge(local_graph, u, i, data->num_vertices);

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

    pthread_exit(NULL);
}

int get_u(float *local_graph, float *d, int v, int *v_t, bool *visited, int graph_size)
{
    int u_min = 1;
    float min_weight = INFINITE;

    for (int u = 1; u <= graph_size; u++)
    {
        // excluir a diagonal e v-v_t
        if (u == v || visited[u])
            continue;

        if ((d[u] < min_weight && d[u] > 0))
        {
            u_min = u;
            min_weight = d[u];
        }

        int u_weight = get_corrected_edge(local_graph, v, u, graph_size);
        if (u_weight < min_weight)
        {
            u_min = u;
            min_weight = u_weight;
        }

#ifdef DEBUG
        printf("v->%d | u_min->%d | min_weight: %f | d[v]: %f\n", v, u_min, min_weight, d[v]);
#endif
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
    // início é col e primeira linha (col, 1)
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

/*
    get_corrected_edge

    obtém a aresta sempre com a coluna corrigida para o grafo local
*/
float get_corrected_edge(float *local_graph, int col, int row, int num_vertices)
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

    col = get_corrected_vertice(col, num_vertices);

    return get_edge(local_graph, col, row);
}

/*
    get_corrected_vertice

    corrige o vértice dado para o vetor local
    apenas considera quando v > num_vertices pois isso
    indica que está em P_i, i>0

    para P_0 não é necessária correção
*/
int get_corrected_vertice(int v, int num_vertices)
{
    if (v > num_vertices)
        v -= num_vertices;

    return v;
}
