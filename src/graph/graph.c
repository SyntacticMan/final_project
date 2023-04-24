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
int **create_graph(int graph_size)
{
    // alocar as colunas
    int **new_graph = (int **)malloc(get_array_size(graph_size) * sizeof(int *));

    printf("Array size: %d\n", get_array_size(graph_size));
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

            add_edge(new_graph, col, row);
        }
    }

    return new_graph;
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
    obtendo com base no indíce da última coluna
*/
int get_array_size(int graph_size)
{
    return get_index(graph_size, 0) + (graph_size - 1);
}

/*
 * add_edge

 * cria uma aresta entre os vértices u e v
 */
void add_edge(int **graph, int u, int v)
{
    int index = get_index(u, v);

    // o peso é atribuído ao acaso
    int *weight = (int *)malloc(sizeof(int));
    *weight = random_generator(MAX_WEIGHT, MIN_WEIGHT);

    graph[index] = weight;
}