#include "graph.h"

int random_generator(int max, int min)
{
    return (rand() % (max - min + 1)) + min;
}

graph *create_graph()
{
    int graph_size = random_generator(GRAPH_SIZE_MAX, GRAPH_SIZE_MIN);
    graph *graph = malloc(sizeof(graph));

    // adicionar os vértices
    vertice *vertice_head = NULL;
    vertice *vertice_list = NULL;

    for (int i = 1; i <= graph_size; i++)
    {
        if (i == 1)
        {
            vertice_head = add_vertice(i);
            vertice_list = vertice_head;
        }
        else
        {
            vertice_list->next_vertice = add_vertice(i);
            vertice_list = vertice_list->next_vertice;
        }
    }

    // adicionar arestas
    int vertice = 0;
    vertice_list = vertice_head;

    edge *edge_head = NULL;
    edge *edge_list = NULL;

    while (vertice_list != NULL)
    {
        // escolher um vértice
        vertice = random_generator(graph_size, 1);

        // se ainda não tiver arestas, simplesmente adicionar
        if (edge_head == NULL)
        {
            edge_head = add_edge(vertice_list->id, vertice);
            edge_list = edge_head;
        }
        else
        {
            // avaliar o vértice da lista ao qual vou adicionar uma aresta
            int vertice_count = check_edge(edge_head, vertice_list->id);

            if (vertice_count <= MAX_EDGE)
            {
                // avaliar o vértice escolhido para formar uma aresta
                vertice_count = check_edge(edge_head, vertice);

                // se nenhum dos dois exceder o limite, adicionar a aresta
                if (vertice_count <= MAX_EDGE)
                {
                    edge_list->next_edge = add_edge(vertice_list->id, vertice);
                    edge_list = edge_list->next_edge;
                }
            }
        }

        vertice_list = vertice_list->next_vertice;
    }

    graph->vertice_head = vertice_head;
    graph->edge_head = edge_head;

    return graph;
}

edge *add_edge(int u, int v)
{
    edge *new_edge = malloc(sizeof(edge));
    new_edge->next_edge = NULL;
    new_edge->u = u;
    new_edge->v = v;
    new_edge->weight = random_generator(MAX_WEIGHT, MIN_WEIGHT);

    return new_edge;
}

int check_edge(edge *edge_head, int vertice)
{
    // se ainda não tiverem sido criadas arestas
    // devolver logo contagem a 0
    if (edge_head == NULL)
        return 0;

    edge *edge_list = edge_head;
    int vertice_count = 0;

    while (edge_list != NULL)
    {
        if (edge_list->u == vertice || edge_list->v == vertice)
            vertice_count++;

        edge_list = edge_list->next_edge;
    }

    return vertice_count;
}

vertice *add_vertice(int n)
{
    vertice *new_vertice = malloc(sizeof(vertice));

    new_vertice->next_vertice = NULL;
    new_vertice->id = n;

    return new_vertice;
}