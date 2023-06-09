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

int **graph;

void add_random_edge(int u, int v);

static int get_index(int col, int row);
static double get_edge_probability(int graph_size, double requested_edge_percentage);

static int random_generator(int max, int min);

/*
 *   random_generator

 *   gera um inteiro entre min e max
 */
int random_generator(int max, int min)
{
    return (rand() % (max - min + 1)) + min;
}

/*
 * create_graph

 * cria um novo grafo, com lista de vértices e arestas
 */
void create_graph(int graph_size, int edge_percentage)
{
    srand(time(NULL));

    // alocar as colunas
    graph = (int **)malloc(get_array_size(graph_size) * sizeof(int *));

    if (graph == NULL)
    {
        return;
    }
    double edge_probability = get_edge_probability(graph_size, edge_percentage);
    // printf("Edge probability %f\n", edge_probability);

    // uma primeira passagem para assegurar
    // que todos os vértices têm, pelo menos, uma aresta
    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row <= graph_size; row++)
        {
            add_random_edge(col, row);
        }
    }

    for (int col = 0; col <= graph_size; col++)
    {
        for (int row = 0; row < graph_size; row++)
        {
            // se linha = coluna o vértice ligar-se-ia a ele mesmo
            // se linha > coluna estou na triangular inferior
            // em ambos os casos passo à frente
            if ((row >= col))
                continue;

            if ((double)random_generator(100, 1) < edge_probability)
                add_random_edge(col, row);
            else
                add_null_edge(graph, col, row);
        }
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
    graph = (int **)malloc(graph_size * sizeof(int *));

    for (int i = 0; i <= graph_size; i++)
    {
        graph[i] = (int *)malloc(sizeof(int));
    }

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

    // graph[1][0] = 1;

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
    get_index

    converte a coluna e linha no índice correspondente
    no vetor que guarda o grafo
*/
int get_index(int col, int row)
{
    int n = col - 1;
    int index = (n * (n + 1)) / 2;

    // adicionar a linha ao índice
    // para obter índice da coluna/linha
    if (row > 0)
        index += row;

    return index;
}

/*
    get_array_size

    calcula o tamanho necessário que o array tem de ter
    com base no índice da última coluna
*/
int get_array_size(int graph_size)
{
    return get_index(graph_size, 0) + (graph_size - 1);
}

/*
 * add_random_edge

 * cria uma aresta entre os vértices u e v com um peso aleatório
 */
void add_random_edge(int u, int v)
{
    int index = get_index(u, v);

    // o peso é atribuído ao acaso
    int *weight = (int *)malloc(sizeof(int));
    *weight = random_generator(MAX_WEIGHT, MIN_WEIGHT);

    // ocasionalmente gera vértices isolados
    // o código seguinte é para garantir que isso não acontece
    if (*weight < 1)
        *weight = 1;

    if (*weight > 8)
        *weight = 8;

    graph[index] = weight;
}

/*
 * add_edge

 * cria uma aresta entre os vértices u e v com o peso indicado
 */
void add_edge(int **graph, int u, int v, int weight)
{
    int index = get_index(u, v);
    int *allocated_weight = (int *)malloc(sizeof(int));
    *allocated_weight = weight;

    graph[index] = allocated_weight;
}

/*
    add_null_edge

    adiciona uma "não-ligação" entre os vértices u e v
    representada por um infinito
*/
void add_null_edge(int **graph, int u, int v)
{
    int index = get_index(u, v);

    int *inf = (int *)malloc(sizeof(int));
    *inf = INFINITE;
    graph[index] = inf;
}

/*
    get_edge

    obtém a aresta, ou a ausência dela,
    correspondente à coluna e linha indicada
*/
int get_edge(int **graph, int u, int v)
{

    // um vértice não tem ligação com ele mesmo
    // logo a aresta é sempre 0
    if (u == v)
        return 0;

    // se a coluna fôr mais pequena que a linha
    // é porque foi pedida uma aresta da triangular inferior
    // portanto vou inverter coluna e linha para obter a simétrica
    if (u < v)
    {
        int temp = u;
        u = v;
        v = temp;
    }

    int index = get_index(u, v);

    /* DEBUG */
    // printf("(%d,%d) => index = %d || weight = %d\n", u, v, index, *graph[index]);

    if (graph[index] == NULL)
    {
        return 0;
    }
    else
    {
        return *graph[index];
    }
}

/*
    get_max_edge_count

    obtém o número máximo de arestas que o grafo pode ter
*/
int get_max_edge_count(int graph_size)
{
    return ((graph_size * (graph_size - 1)) / 2);
}

/*
    get_edge_count

    obtém o número de arestas existentes no grafo
*/
int get_edge_count(int graph_size)
{
    int edge_count = 0;

    for (int i = 0; i < graph_size; i++)
    {
        for (int j = 0; j < graph_size; j++)
        {
            // para evitar contar duas vezes
            // apenas pedir da triangular superior
            // também ignorando i=j
            if (j < i && get_edge(graph, i, j) > 0)
            {
                edge_count++;
            }
        }
    }

    return edge_count;
}

/*
    get_edge_percentage

    calcula a percentagem de arestas existentes no grafo
    contra o número máximo de arestas que pode ter
*/
float get_edge_percentage(int graph_size)
{
    return ((float)get_edge_count(graph_size) / (float)get_max_edge_count(graph_size)) * 100.0;
}

/*
    get_edge_probability

    Calcula a possibilidade de uma aresta ser criada, tendo em conta
    a percentagem de arestas pedidas e o tamanho do grafo
*/
double get_edge_probability(int graph_size, double requested_edge_percentage)
{

    double edge_probability = 2.0 * (requested_edge_percentage * get_max_edge_count((double)graph_size)) / ((double)graph_size * ((double)graph_size - 1.0));

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
                printf("|%3d", row);
            }
            printf("\n");
        }
        printf("%d", col);

        for (int row = 0; row < graph_size; row++)
        {
            int edge = get_edge(graph, col, row);
            if (edge == INFINITE)
                printf("|inf");
            else
                printf("|%3d", edge);
        }

        printf("\n");
    }
}
