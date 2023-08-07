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

float *graph;

void add_random_edge(int u, int v);

static int get_index(int col, int row);
static double get_edge_probability(int graph_size, double requested_edge_percentage);
static void create_valid_edge(int graph_size);

static int random_generator(int max, int min);
static float random_float_generator(float max, float min);
static void print_progress_bar(int progress, int total, int barWidth);
double random_coordinate_generator(int graph_size);

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
    z1 = ((z1 + 2.0) * (graph_size - 1) / 4.0) + 2;

    return z0;
}

/*
 * create_graph

 * cria um novo grafo, com lista de vértices e arestas
 */
void create_graph(int graph_size, int edge_percentage)
{
    srand(time(NULL));

    // alocar as colunas
    int matrix_size = get_matrix_size(graph_size);
    graph = malloc(matrix_size * sizeof(float));

    if (graph == NULL)
    {
        return;
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

    // obter o número de arestas correspondentes à percentagem pedida
    float num_edges = (float)matrix_size * (edge_percentage / 100.0);

    for (int i = 0; i < num_edges; i++)
    {
        create_valid_edge(graph_size);
    }
}

/*
    create_locked_graph

    cria um grafo fixo para facilitar testes
*/
void create_locked_graph(int graph_size, int edge_percentage)
{
    srand(time(NULL));

    // alocar as colunas
    graph = (float *)malloc(get_matrix_size(graph_size) * sizeof(float));

    if (graph == NULL)
    {
        return;
    }

    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row < graph_size; row++)
        {
            add_null_edge(graph, col, row);
        }
    }

    //            --c  r
    add_edge(graph, 1, 0, 1);
    add_edge(graph, 2, 0, 3);
    add_edge(graph, 5, 0, 3);
    add_edge(graph, 2, 1, 5);
    add_edge(graph, 3, 1, 1);
    add_edge(graph, 3, 2, 2);
    add_edge(graph, 4, 2, 1);
    add_edge(graph, 4, 3, 4);
    add_edge(graph, 5, 4, 5);
}

/*
    create_valid_edge

    função recursiva para atribuir uma aresta
    no vértice válido que ainda não tenha aresta
*/
void create_valid_edge(int graph_size)
{
    int col, row;

    col = random_coordinate_generator(graph_size);
    // o máximo da linha é sempre col - 1
    row = random_coordinate_generator(col);

    // apenas aceitar coordenadas de forem válidas
    if (col <= graph_size && row < col && get_edge(graph, col, row) == INFINITE)
    {
        add_random_edge(col, row);
    }
    else
    {
        create_valid_edge(graph_size);
    }
}

/*
    get_index

    converte a coluna e linha no índice correspondente
    no vetor que guarda o grafo
*/
int get_index(int col, int row)
{
    int index = ((col - 1) * (col - 2)) / 2;

    // adicionar a linha ao índice
    // para obter índice da coluna/linha
    if (row > 1)
        index += row - 1;

    return index;
}

/*
    get_matrix_size

    calcula o número de elementos da matriz de adjacência

*/
unsigned int get_matrix_size(int graph_size)
{
    return ((graph_size * (graph_size - 1)) / 2);
}

/*
 * add_random_edge

 * cria uma aresta entre os vértices u e v com um peso aleatório
 */
void add_random_edge(int col, int row)
{
    int index = get_index(col, row);

    // o peso é atribuído ao acaso
    float weight = random_float_generator(MAX_WEIGHT, MIN_WEIGHT);

    // ocasionalmente gera vértices isolados
    // o código seguinte é para garantir que isso não acontece
    if (weight < 1.0)
        weight = 1.0;

    if (weight > 8.0)
        weight = 8.0;

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

    int index = get_index(col, row);

    /* DEBUG */
    // printf("(%d,%d) => index = %d || weight = %f\n", col, row, index, graph[index]);

    return graph[index];
}

/*
    get_edge_count

    obtém o número de arestas existentes no grafo
*/
int get_edge_count(int graph_size)
{
    int edge_count = 0;

    for (int i = 2; i <= graph_size; i++)
    {
        for (int j = 1; j < i; j++)
        {
            float edge = get_edge(graph, i, j);

            if (edge > 0 && edge <= MAX_WEIGHT)
            {
                edge_count++;
            }
        }
    }

    // printf("edge count: %d\n", edge_count);

    return edge_count;
}

/*
    get_edge_percentage

    calcula a percentagem de arestas existentes no grafo
    contra o número máximo de arestas que pode ter
*/
float get_edge_percentage(int graph_size)
{
    return ((float)get_edge_count(graph_size) / (float)get_matrix_size(graph_size)) * 100.0;
}

/*
    get_edge_probability

    Calcula a possibilidade de uma aresta ser criada, tendo em conta
    a percentagem de arestas pedidas e o tamanho do grafo
*/
double get_edge_probability(int graph_size, double requested_edge_percentage)
{

    double edge_probability = 2.0 * (requested_edge_percentage * get_matrix_size((double)graph_size)) / ((double)graph_size * ((double)graph_size - 1.0));

    // se o grafo fôr muito grande e a percentagem muito baixa
    // a probabilidade pode ficar negativa, nesse caso devolvo 1
    if (edge_probability < 0)
        edge_probability = 1.0;

    return edge_probability;
}

/*
    pick_graph_root

    escolhe um vértice ao acaso como raíz do grafo
*/
int pick_graph_root(int graph_size)
{
    return random_generator(graph_size - 1, 1);
}

/*
    print_graph

    imprime a matriz de adjacência do grafo na linha de comandos
*/
void print_graph(int graph_size)
{
    for (int col = 0; col < graph_size; col++)
    {
        // cabeçalho
        if (col == 0)
        {
            printf(" ");

            for (int row = 0; row < graph_size; row++)
            {
                printf("|%5d", row + 1);
            }
            printf("\n");
        }
        printf("%2d", col + 1);

        for (int row = 0; row < graph_size; row++)
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
