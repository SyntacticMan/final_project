#include "graph.h"

/*
 *   Função random_generator
 *   gera um inteiro entre min e max
 */
int random_generator(int max, int min)
{
    return (rand() % (max - min + 1)) + min;
}

/*
 * Função create_graph
 * cria um novo grafo, com lista de vértices e arestas
 */
int **create_graph(int graph_size)
{
    // alocar as colunas
    int **new_graph = (int **)malloc((graph_size + (graph_size - 1)) * sizeof(int *));

    // como a matriz é triangular superior
    // apenas aloco os elementos que vão ser utilizados
    for (int col = 0; col <= graph_size; col++)
    {
        for (int row = 0; row < graph_size; row++)
        {
            // se linha = coluna o vértice ligar-se-ia a ele mesmo
            // se linha > coluna estou na triangular inferior
            // em ambos os casos passo à frente
            if ((row == col) || (row > col))
                continue;

            add_edge(new_graph, col, row);
        }
    }

    return new_graph;
}

int get_col_index(int col)
{
    if (col == 2)
        return 0;

    int index = 0;

    for (int i = 2; i < col; i++)
    {
        index += i - 1;
    }

    return index;
}

int get_row_index(int col_index, int row)
{
    return 0;
}

/*
 * Função add_edge
 * cria uma aresta entre os vértices u e v
 */
void add_edge(int **graph, int u, int v)
{
    int col_index = get_col_index(u);
    int row_index = col_index + v;

    // o peso é atribuído ao acaso
    int weight = random_generator(MAX_WEIGHT, MIN_WEIGHT);

    printf("col: %d, row: %d, weight: %d\n", u, v, weight);
    graph[row_index] = (int *)malloc(sizeof(int));
    graph[row_index] = &weight;
}

// /*
//  * Função check_edge
//  * devolve o número de arestas que o vertice dado
//  * tem na lista de arestas edge
//  */
// int check_edge(edge *edge_head, int vertice)
// {
//     // se ainda não tiverem sido criadas arestas
//     // devolver logo contagem a 0
//     if (edge_head == NULL)
//         return 0;

//     edge *edge_list = edge_head;
//     int vertice_count = 0;

//     while (edge_list != NULL)
//     {
//         if (edge_list->u == vertice || edge_list->v == vertice)
//             vertice_count++;

//         edge_list = edge_list->next_edge;
//     }

//     return vertice_count;
// }

// /*
//  * Função add_vertice
//  * adiciona um vértice com id n
//  */
// vertice *add_vertice(int n)
// {
//     vertice *new_vertice = malloc(sizeof(vertice));

//     new_vertice->next_vertice = NULL;
//     new_vertice->id = n;

//     return new_vertice;
// }