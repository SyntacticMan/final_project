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

// float *graph;

static void add_random_edge(float *graph, int u, int v);

static void create_valid_edge(float *graph, int graph_size);

static int random_generator(int max, int min);
static float random_float_generator(float max, float min);
static double random_coordinate_generator(int graph_size);

#ifdef DEBUG
static void print_progress_bar(int progress, int total, int barWidth);
#endif

/*
 * create_graph

 * cria um novo grafo, com lista de vértices e arestas
 */
float *create_graph(int graph_size, int edge_percentage)
{
    srand(time(NULL));

    // alocar as colunas
    unsigned int matrix_size = get_matrix_size(graph_size);

#ifdef DEBUG
    printf("Allocating %lu bytes for a matrix with %u elements\n", (matrix_size * sizeof(float)), matrix_size);
#endif

    float *graph = malloc(matrix_size * sizeof(float));

    if (graph == NULL)
    {
        printf("Nao foi possivel alocar o grafo\n");
        return NULL;
    }

    // uma primeira passagem para preencher a matriz com infinitos
    for (int col = 2; col <= graph_size; col++)
    {
        for (int row = 1; row < col; row++)
        {
            // se linha = coluna o vértice ligar-se-ia a ele mesmo
            // se linha > coluna estou na triangular inferior
            // em ambos os casos passo à frente
            if ((row >= col))
                continue;

            add_null_edge(graph, col, row);
        }
    }

    // todos os vértices têm de ter pelo menos uma ligação
    double num_edges_count = 0;
    for (int col = 2; col <= graph_size; col++)
    {
        for (int row = 1; row < col; row++)
        {
            // se linha = coluna o vértice ligar-se-ia a ele mesmo
            // se linha > coluna estou na triangular inferior
            // em ambos os casos passo à frente
            if ((row >= col))
                continue;

            add_random_edge(graph, col, row);
            num_edges_count++;
        }
    }

    // obter o número de arestas correspondentes à percentagem pedida
    // excluindo as que já têm
    double num_edges = ((double)matrix_size * (edge_percentage / 100.0));
    num_edges -= num_edges_count;

    // float num_edges = matrix_size * (edge_percentage / 100.0);
#ifdef DEBUG
    printf("num_edges= %f\n", num_edges);
#endif

    for (int i = 0; i < num_edges; i++)
    {
        create_valid_edge(graph, graph_size);
    }
    printf("num_edges_count= %f\n", num_edges_count);

    return graph;
}

/*
    create_locked_graph

    cria um grafo fixo para facilitar testes
*/
float *create_locked_graph(int graph_size, int edge_percentage)
{
    srand(time(NULL));

    // alocar as colunas
    float *graph = (float *)malloc(get_matrix_size(graph_size) * sizeof(float));

    if (graph == NULL)
    {
        return NULL;
    }

    for (int col = 1; col <= graph_size; col++)
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
void create_valid_edge(float *graph, int graph_size)
{
    int col, row;

    col = random_coordinate_generator(graph_size);

    row = random_coordinate_generator(col);

    // apenas aceitar coordenadas de forem válidas
    if (get_edge(graph, col, row) == INFINITE && col <= graph_size && row < col)
    {
        add_random_edge(graph, col, row);
    }
    else
    {
        create_valid_edge(graph, graph_size);
    }
}

/*
 * add_random_edge

 * cria uma aresta entre os vértices u e v com um peso aleatório
 */
void add_random_edge(float *graph, int col, int row)
{
    int index = get_index(col, row);

    // o peso é atribuído ao acaso
    float weight = random_float_generator(MAX_WEIGHT, MIN_WEIGHT);

    // ocasionalmente gera vértices isolados
    // o código seguinte é para garantir que isso não acontece
    if (weight < MIN_WEIGHT)
        weight = MIN_WEIGHT;

    if (weight > MAX_WEIGHT)
        weight = MAX_WEIGHT;

    graph[index] = weight;
}

/*
 * add_edge

 * cria uma aresta entre os vértices u e v com o peso indicado
 */
void add_edge(float *graph, int col, int row, float weight)
{
    int index = get_index(col, row);
    graph[index] = weight;
}

/*
    add_null_edge

    adiciona uma "não-ligação" entre os vértices u e v
    representada por um infinito
*/
void add_null_edge(float *graph, int col, int row)
{
    graph[get_index(col, row)] = INFINITE;
}

/*
    get_matrix_size

    calcula o número de elementos da matriz de adjacência

*/
unsigned int get_matrix_size(int graph_size)
{
    // por alguma razão, se tentar fazer a conta duma só vez,
    // começa a devolver resultados incorrectos acima de 46000
    unsigned int numerator = graph_size * (graph_size - 1);
    unsigned int matrix_size = numerator / 2;

#ifdef TRACE
    printf("graph_size %d | graph_size - 1 %d | numerator %u | matrix_size %u\n", graph_size, graph_size - 1, numerator, matrix_size);
#endif

    return matrix_size;
}

/*
    get_edge

    obtém a aresta, ou a ausência dela,
    correspondente à coluna e linha indicada
*/
float get_edge(float *graph, int col, int row)
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
    printf("(%d,%d) => index = %d || weight = %f\n", col, row, index, graph[index]);
#endif

    return graph[index];
}

/*
    get_index

    converte a coluna e linha no índice correspondente
    no vetor que guarda o grafo
*/
unsigned int get_index(int col, int row)
{
    unsigned int index = ((col - 1) * (col - 2)) / 2;

    // adicionar a linha ao índice
    // para obter índice da coluna/linha
    if (row > 1)
        index += row - 1;

#ifdef TRACE
    if (index % 20000 == 0)
    {
        fflush(stdout);
        printf("index for col: %d, row :%d => %i\n", col, row, index);
        fflush(stdout);
    }
#endif
    return index;
}

/*
    get_edge_count

    obtém o número de arestas existentes no grafo
*/
int get_edge_count(float *graph, int graph_size)
{
    int edge_count = 0;

    for (int i = 2; i <= graph_size; i++)
    {
        for (int j = 1; j < i; j++)
        {
            float edge = get_edge(graph, i, j);

            if (edge > MIN_WEIGHT && edge < INFINITE)
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
double get_edge_percentage(float *graph, int graph_size)
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

void print_array(float *graph, int graph_size)
{
    unsigned long int array_size = get_matrix_size(graph_size);

    for (int i = 0; i < array_size; i++)
    {
        printf(" i: %d -> %3.3f", i, graph[i]);
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
double random_coordinate_generator(int graph_size)
{
    double u1, u2, z0, z1;
    static int generate = 0;
    generate = 1 - generate;

    if (!generate)
        return z1;

    do
    {
        u1 = (double)rand() / RAND_MAX;
        u2 = (double)rand() / RAND_MAX;
    } while (u1 <= 1e-7);

    z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

    // ajustar os valores para ficarem entre 1 e graph_size
    z0 = ((z0 + 2.0) * (graph_size - 1) / 4.0) + 1;
    z1 = ((z1 + 2.0) * (graph_size - 1) / 4.0) + 1;

    // Para graph_size muito grande está a exceder
    // por isso limito-o a graph_size e subtraio um número ao acaso
    if (z0 > graph_size)
        z0 = graph_size - (double)rand() / RAND_MAX;
    if (z1 > graph_size)
        z1 = graph_size - (double)rand() / RAND_MAX;

#ifdef TRACE
    printf("z0->%f\nz1->%f\n", z0, z1);
#endif
    return z0;
}

/*
    print_graph

    imprime a matriz de adjacência do grafo na linha de comandos
*/
void print_graph(float *graph, int graph_size)
{
    for (int col = 1; col <= graph_size; col++)
    {
        // cabeçalho
        if (col == 1)
        {
            printf(" ");

            for (int row = 1; row <= graph_size; row++)
            {
                printf("|%5d", row);
            }
            printf("\n");
        }
        printf("%2d", col);

        for (int row = 1; row <= graph_size; row++)
        {
            float edge = get_edge(graph, col, row);
            if (edge == INFINITE)
                printf("|  inf");
            else
                printf("|%3.3f", edge);
        }

        printf("\n");
    }
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