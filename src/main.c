#include <stdio.h>
#include <stdlib.h>

#define MAX_EDGE 3;
#define MIN_EDGE 1;
#define GRAPH_SIZE 10;

typedef struct edge
{
	int u, v;
	int weight;
	struct edge *next_edge;
} edge;

typedef struct vertice
{
	int id;
	struct vertice *next_vertice;
} vertice;

typedef struct graph
{
	vertice *vertice_head;
	edge *edge_head;
} graph;

graph *create_graph();
vertice *add_vertice(int n);
edge *add_edge(int u, int v);

int main()
{
	graph *test = create_graph();

	while (test->edge_head != NULL)
	{
		printf("aresta: %d->%d (%d)\n", test->edge_head->u, test->edge_head->v, test->edge_head->weight);
		test->edge_head = test->edge_head->next_edge;
	}

	while (test->vertice_head != NULL)
	{
		printf("vertice: %d\n", test->vertice_head->id);
		test->vertice_head = test->vertice_head->next_vertice;
	}

	// printf("%d", test->id);
	// printf("%d", test->edge_element->u);
	// printf("%d\n", test->edge_element->v);

	return 0;
}

graph *create_graph()
{
	graph *graph = malloc(sizeof(graph));

	// adicionar os vértices
	vertice *vertice_head = add_vertice(1);
	vertice *vertice_list = vertice_head;

	for (int i = 2; i <= 10; i++)
	{
		vertice_list->next_vertice = add_vertice(i);
		vertice_list = vertice_list->next_vertice;
	}

	// adicionar arestas
	edge *edge_head = add_edge(1, 2);
	edge *edge_list = edge_head;

	for (int i = 2; i < 10; i++)
	{
		edge_list->next_edge = add_edge(i, i + 1);
		edge_list = edge_list->next_edge;
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
	new_edge->weight = 1;

	return new_edge;
}

vertice *add_vertice(int n)
{
	vertice *new_vertice = malloc(sizeof(vertice));

	new_vertice->next_vertice = NULL;
	new_vertice->id = n;

	return new_vertice;
}