#ifndef GRAPH
#define GRAPH
#include "graph.h"
#endif

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
    // alocar as colunas
    graph = (int **)malloc(get_array_size(graph_size) * sizeof(int *));

    // como a matriz é triangular superior
    // apenas aloco os elementos que vão ser utilizados
    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row < graph_size; row++)
        {
            // se linha = coluna o vértice ligar-se-ia a ele mesmo
            // se linha > coluna estou na triangular inferior
            // em ambos os casos passo à frente
            if ((row >= col))
                continue;

            if (row % 2 == 0)
                add_edge(col, row);
            else
                add_null_edge(col, row);
        }
    }
}

/*
    get_index

    converte a coluna e linha no indíce correspondente
    no vetor que guarda o grafo
*/
int get_index(int col, int row)
{
    // o array apenas guarda colunas a partir da 2
    if (col <= 2)
        return 0;

    int n = col - 2;
    int index = (n * (n + 1)) / 2;

    // adicionar a linha ao indíce
    // para obter indíce da coluna/linha
    if (row > 1)
        index += row - 1;

    return index;
}

/*
    get_array_size

    calcula o tamanho necessário que o array tem de ter
    com base no indíce da última coluna
*/
int get_array_size(int graph_size)
{
    return get_index(graph_size, 0) + (graph_size - 1);
}

/*
 * add_edge

 * cria uma aresta entre os vértices u e v
 */
void add_edge(int u, int v)
{
    int index = get_index(u, v);

    // o peso é atribuído ao acaso
    int *weight = (int *)malloc(sizeof(int));
    *weight = random_generator(MAX_WEIGHT, MIN_WEIGHT);

    graph[index] = weight;
}

/*
    add_null_edge

    adiciona uma "não-ligação" entre os vértices u e v
    representada por um infinito
*/
void add_null_edge(int u, int v)
{
    int index = get_index(u, v);

    // como o vetor é de inteiros
    // uso -1 para representar o infinito
    int *inf = (int *)malloc(sizeof(int));
    *inf = -1;
    graph[index] = inf;
}

/*
    get_edge

    obtém a aresta, ou a ausência dela,
    correspondente à coluna e linha indicada
*/
int get_edge(int u, int v)
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

    return *graph[index];
}