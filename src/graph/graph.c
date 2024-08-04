/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Biblioteca de grafos												*
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef GRAPH
#define GRAPH
#include "graph.h"
#endif

#include <pthread.h>
#include <sched.h>

// argumentos para os processos
typedef struct _thread_data
{
    int start_col;
    int end_col;
    int cpu_id;
    unsigned long long matrix_size;
    int edge_percentage;
    int local_graph_size;
} thread_data;

typedef struct _random_coordinates
{
    int column;
    int row;
} random_coordinates;

// variáveis partilhadas
unsigned short int *graph;

static void *initialize_graph(void *arg);

static void add_random_edge(unsigned short int *graph, int u, int v);

static void create_valid_edge(unsigned short int *graph, int start_col, int end_col);

static int random_generator(int max, int min);
static float random_float_generator(float max, float min);
static random_coordinates *random_coordinate_generator(int start_col, int end_col);
static void print_line(int graph_size);

#ifdef TRACE
static void print_progress_bar(int progress, int total, int barWidth);
#endif

/*
 * create_graph

 * cria um novo grafo, com lista de vértices e arestas
 */
unsigned short int *create_graph(int graph_size, int edge_percentage)
{
    // alocar as colunas
    unsigned long long matrix_size = get_matrix_size(graph_size);

    pthread_t threads[THREADS];

    // tarefas acessórias
    thread_data thread_data[THREADS];

#ifdef DEBUG
    unsigned long int ram_b = (matrix_size * sizeof(unsigned short int));
    unsigned long int ram_kb = ram_b / 1024;
    unsigned long int ram_mb = ram_kb / 1024;
    unsigned long int ram_gb = ram_mb / 1024;
    printf("Allocating %lu b | %lu kb | %lu mb | %lu gb (matrix size: %llu)\n", ram_b, ram_kb, ram_mb, ram_gb, matrix_size);
#endif

    graph = malloc(matrix_size * sizeof(unsigned short int) + 1);

    if (graph == NULL)
    {
        printf("Nao foi possivel alocar o grafo\n");
        return NULL;
    }

    for (int i = 0; i < THREADS; i++)
    {
        // calcular o número de vértices (n) a alocar a cada processo
        // divisão pode não ser inteira, por isso o resultado é arredondado
        int num_vertices = round(graph_size / THREADS);
        int last_end_col = 0;

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
        if (i == THREADS - 1)
        {
            end_col = end_col + (graph_size - last_end_col);
        }

        thread_data[i].end_col = start_col + num_vertices - 1;
        thread_data[i].cpu_id = i;
        thread_data[i].matrix_size = matrix_size;
        thread_data[i].edge_percentage = edge_percentage;
        thread_data[i].local_graph_size = end_col - start_col;

        pthread_create(&threads[i], NULL, initialize_graph, (void *)&thread_data[i]);
    }

    // recuperar os processos criados
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], (void **)NULL);
    }

#ifdef DEBUG
    printf("Inicializacao completa, adicionando arestas aleatorias\n");
#endif

    return graph;
}

void *initialize_graph(void *arg)
{
    thread_data *data = (thread_data *)arg;

#ifdef DEBUG
    printf("start_col = %d\tend_col = %d\tthread_id = %d\n", data->start_col, data->end_col, data->cpu_id);
#endif

    // uma primeira passagem para preencher a matriz com infinitos
    for (int col = data->start_col; col <= data->end_col; col++)
    {
        // se linha = coluna o vértice ligar-se-ia a ele mesmo
        // se linha > coluna estou na triangular inferior
        // por isso apenas é feita a iteração até linha < coluna
        for (int row = 1; row < col; row++)
        {
            add_null_edge(graph, col, row);
        }

        // todos os vértices têm de ter pelo menos uma ligação
        int row = random_generator(col, 1);

        add_random_edge(graph, col, row);
    }

    // obter o número de arestas correspondentes à percentagem pedida
    // excluindo as que já têm
    int num_edges = (data->matrix_size * (data->edge_percentage / 100.0));
    num_edges -= data->local_graph_size;

#ifdef DEBUG
    printf("num_edges= %d\n", num_edges);
#endif

    for (int i = 0; i < num_edges; i++)
    {
        create_valid_edge(graph, data->start_col, data->end_col);
    }

    pthread_exit(NULL);
}

/*
    create_locked_graph

    recria o grafo do livro fixo para validar funcionamento do algoritmo
*/
unsigned short int *create_locked_graph()
{
    srand(time(NULL));

    // alocar as colunas
    unsigned short int *graph = (unsigned short int *)malloc(get_matrix_size(6) * sizeof(unsigned short int));

    if (graph == NULL)
    {
        return NULL;
    }

#ifdef DEBUG
    // unsigned int matrix_size = get_matrix_size(graph_size);
    unsigned long long matrix_size = get_matrix_size(6);

    unsigned long int ram_b = (matrix_size * sizeof(unsigned short int));
    unsigned long int ram_kb = ram_b / 1024;
    unsigned long int ram_mb = ram_kb / 1024;
    unsigned long int ram_gb = ram_mb / 1024;
    printf("Allocating %lu b | %lu kb | %lu mb | %lu gb (matrix size: %llu)\n", ram_b, ram_kb, ram_mb, ram_gb, matrix_size);
#endif

    for (int col = 1; col <= 6; col++)
    {
        for (int row = 1; row < col; row++)
        {
            add_null_edge(graph, col, row);
        }
    }

    //            --c  r  w
    add_edge(graph, 2, 1, 1);
    add_edge(graph, 3, 1, 3);
    add_edge(graph, 6, 1, 3);
    add_edge(graph, 3, 2, 5);
    add_edge(graph, 4, 2, 1);
    add_edge(graph, 4, 3, 2);
    add_edge(graph, 5, 3, 1);
    add_edge(graph, 5, 4, 4);
    add_edge(graph, 6, 5, 5);

    return graph;
}

/*
    create_valid_edge

    função recursiva para atribuir uma aresta
    no vértice válido que ainda não tenha aresta
*/
void create_valid_edge(unsigned short int *graph, int start_col, int end_col)
{
    random_coordinates *coords = random_coordinate_generator(start_col, end_col);

    float edge = get_edge(graph, coords->column, coords->row);

#ifdef TRACE
    printf("col = %d | row = %d | found_edge = %f\n", coords->column, coords->row, edge);
#endif
    // apenas aceitar coordenadas de forem válidas
    if (edge == INFINITE && coords->column <= end_col && coords->row < coords->column)
    {
        add_random_edge(graph, coords->column, coords->row);
    }
    else
    {
        create_valid_edge(graph, start_col, end_col);
    }

    free(coords);
}

/*
 * add_random_edge

 * cria uma aresta entre os vértices u e v com um peso aleatório
 */
void add_random_edge(unsigned short int *graph, int col, int row)
{
    // o peso é atribuído ao acaso
    float weight = random_float_generator(MAX_WEIGHT, MIN_WEIGHT);

    // ocasionalmente gera vértices isolados
    // o código seguinte é para garantir que isso não acontece
    if (weight < MIN_WEIGHT)
        weight = MIN_WEIGHT;

    if (weight > MAX_WEIGHT)
        weight = MAX_WEIGHT;

    add_edge(graph, col, row, weight);
}

/*
 * add_edge

 * cria uma aresta entre os vértices u e v com o peso indicado
 */
void add_edge(unsigned short int *graph, int col, int row, float weight)
{
    // a diagonal é sempre 0, devolvido por get_edge
    // por isso não é necessário adicionar
    if (col == row)
        return;

    // adicionar apenas na triangular superior
    if (col < row)
    {
        int temp = col;
        col = row;
        row = temp;
    }

    int index = get_index(col, row);
    graph[index] = (unsigned short int)(weight * SCALE_FACTOR);

#ifdef TRACE
    printf("add %f to col:%d | row: %d\n", weight, col, row);
#endif
}

/*
    add_null_edge

    adiciona uma "não-ligação" entre os vértices u e v
    representada por um infinito
*/
void add_null_edge(unsigned short int *graph, int col, int row)
{
#ifdef TRACE
    printf("col = %d | row = %d | null_edge = %hu\n", col, row, (unsigned short int)(INFINITE));
#endif
    graph[get_index(col, row)] = (unsigned int short)INFINITE;
}

/*
    get_matrix_size

    calcula o número de elementos da matriz de adjacência

*/
unsigned long long get_matrix_size(int graph_size)
{
    return (graph_size * (graph_size - 1ULL)) / 2ULL;
}

/*
    get_edge

    obtém a aresta, ou a ausência dela,
    correspondente à coluna e linha indicada
*/
float get_edge(unsigned short int *graph, int col, int row)
{

    // um vértice não tem ligação com ele mesmo
    // logo a aresta é sempre 0
    if (col == row)
        return 0;

    // se a coluna fôr mais pequena que a linha
    // é porque foi pedida uma aresta da triangular inferior
    // portanto vou inverter coluna e linha para obter a simétrica
    if (col < row)
    {
        int temp = col;
        col = row;
        row = temp;
    }

    unsigned int index = get_index(col, row);

#ifdef TRACE
    printf("get_edge (%d,%d) => index = %d || stored_weight = %d || weight = %f\n", col, row, index, graph[index], ((float)graph[index] / SCALE_FACTOR));
#endif
    unsigned int short edge = graph[index];

    // como INFINITE é simplesmente guardado no vector
    // é preciso determinar se aplico o SCALE_FACTOR ou não
    if (edge == (unsigned int short)INFINITE)
    {
        return (float)edge;
    }
    else
    {
        return ((float)(edge / SCALE_FACTOR));
    }
}

/*
    get_index

    converte a coluna e linha no índice correspondente
    no vetor que guarda o grafo
*/
unsigned long long get_index(int col, int row)
{
    // unsigned int index = ((col - 1) * (col - 2)) / 2;
    unsigned long long index = ((col - 1ULL) * (col - 2ULL)) / 2ULL;

    // adicionar a linha ao índice
    // para obter índice da coluna/linha
    if (row > 1)
        index += row - 1;

#ifdef TRACE
    if (col > 46000)
        printf("index for col: %d, row :%d => %lli\n", col, row, index);
#endif
    return index;
}

/*
    get_edge_count

    obtém o número de arestas existentes no grafo
*/
int get_edge_count(unsigned short int *graph, int graph_size)
{
    int edge_count = 0;

    for (int i = 2; i <= graph_size; i++)
    {
        for (int j = 1; j < i; j++)
        {
            float edge = get_edge(graph, i, j);

            if (edge > 0 && edge < INFINITE)
            {
                edge_count++;
            }
        }
    }

#ifdef DEBUG
    printf("edge count: %d\n", edge_count);
#endif

    return edge_count;
}

/*
    get_edge_percentage

    calcula a percentagem de arestas existentes no grafo
    contra o número máximo de arestas que pode ter
*/
double get_edge_percentage(unsigned short int *graph, int graph_size)
{
    return ((double)get_edge_count(graph, graph_size) / get_matrix_size(graph_size)) * 100.0;
}

/*
    pick_graph_root

    escolhe um vértice ao acaso como raíz do grafo
*/
int pick_graph_root(int graph_size)
{
    return random_generator(graph_size - 1, 1);
}

void print_array(unsigned short int *graph, int graph_size)
{
    unsigned long int array_size = get_matrix_size(graph_size);

    for (int i = 0; i < array_size; i++)
    {
        printf(" i: %d -> %3.3f", i, ((float)graph[i] / SCALE_FACTOR));
    }

    printf("\n");
}

/*
 *   random_generator

 *   gera um inteiro entre min e max
 */
int random_generator(int max, int min)
{
    return (rand() % (max - min + 1)) + min;
}

float random_float_generator(float max, float min)
{
    return (float)rand() / RAND_MAX * (max - min) + min;
}

/*
 *  random_coordinate_generator

 * implementa o método Box-Muller para gerar uma coordenada aleatoriamente
 * tendendo para uma distribuição normal
*/
random_coordinates *random_coordinate_generator(int start_col, int end_col)
{
    double u1, u2, z0, z1;
    static int generate = 0;
    generate = 1 - generate;

    random_coordinates *coords = (random_coordinates *)(malloc(sizeof(random_coordinates)));

    // if (!generate)
    //     return z1;

    do
    {
        u1 = (double)rand() / RAND_MAX;
        u2 = (double)rand() / RAND_MAX;
    } while (u1 <= 1e-7);

    z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

    // ajustar os valores para ficarem entre 1 e graph_size
    z0 = ((z0 + 2.0) * (end_col - 1) / 4.0) + start_col;
    z1 = ((z1 + 2.0) * (end_col - 1) / 4.0) + start_col;

    // Para graph_size muito grande está a exceder
    // por isso limito-o a graph_size e subtraio um número ao acaso
    if (z0 > start_col)
        z0 = start_col - (double)rand() / end_col;
    if (z1 > start_col)
        z1 = start_col - (double)rand() / end_col;

#ifdef TRACE
    printf("z0->%f\nz1->%f\n", z0, z1);
#endif

    coords->column = z0;
    coords->row = z1;
    return coords;
}

/*
    print_graph

    imprime a matriz de adjacência do grafo na linha de comandos
*/
void print_graph(unsigned short int *graph, int graph_size)
{

    print_line(graph_size);

    for (int row = 1; row <= graph_size; row++)
    {
        // cabeçalho

        if (row == 1)
        {
            // printf("|");
            if (graph_size <= 10)
                printf("|%*c", 2, ' ');
            else if (graph_size <= 100)
                printf("%*c", 3, ' ');

            for (int col = 1; col <= graph_size; col++)
            {
                printf("|%5d", col);
            }

            printf("|\n");
            print_line(graph_size);
        }
        else
        {
            print_line(graph_size);
        }

        printf("|%3d", row);

        for (int col = 1; col <= graph_size; col++)
        {
            float edge = get_edge(graph, col, row);
            if (edge == INFINITE)
                printf("|    \u221e");
            else
                printf("|%3.3f", edge);
        }

        printf("|\n");
    }

    print_line(graph_size);
}

/*
    print_graph

    imprime a matriz de adjacência divida do grafo na linha de comandos
*/
void print_graph_mt(unsigned short int *graph, int start_col, int end_col, int graph_size)
{
    int cols = end_col - start_col;
    print_line(cols + 1);

    for (int row = 1; row <= graph_size; row++)
    {
        // cabeçalho
        if (row == 1)
        {
            if (cols <= 10)
                printf("|%*c", 3, ' ');
            else if (cols <= 100)
                printf("%*c", 2, ' ');

            for (int col = start_col; col <= end_col; col++)
            {
                printf("|%5d", col);
            }

            printf("|\n");
            print_line(cols + 1);
        }
        else
        {
            print_line(cols + 1);
        }

        printf("|%3d", row);

        for (int col = start_col; col <= end_col; col++)
        {
            float edge = get_edge(graph, col, row);
            if (edge == INFINITE)
                printf("|    \u221e");
            else
                printf("|%3.3f", edge);
        }

        printf("|\n");
    }

    print_line(cols + 1);
}

void print_line(int graph_size)
{
    for (int i = 0; i < graph_size + 1; i++)
    {
        printf(" ");

        if (i == 0)
            printf("---");
        else
        {
            printf("-----");
        }
    }

    printf("\n");
}

#ifdef TRACE
void print_progress_bar(int progress, int total, int barWidth)
{
    float percentage = (float)progress / total;
    int numBarToPrint = percentage * barWidth;

    printf("[");
    for (int i = 0; i < barWidth; i++)
    {
        if (i < numBarToPrint)
        {
            printf("=");
        }
        else
        {
            printf(" ");
        }
    }
    printf("] %.1f%%\r", percentage * 100);
    fflush(stdout);
}
#endif