#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EDGE 3
#define MIN_EDGE 1

#define MAX_WEIGHT 15
#define MIN_WEIGHT 1

#define GRAPH_SIZE_MIN 10
#define GRAPH_SIZE_MAX 50

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

// funções
graph *create_graph();
vertice *add_vertice(int n);
edge *add_edge(int u, int v);
int random_generator(int max, int min);

int main()
{
	srand(time NULL);
	int graph_size = random_generator(GRAPH_SIZE_MAX, GRAPH_SIZE_MIN);

	printf("rand: %d\n", graph_size);

	graph *test = create_graph(graph_size);

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

int random_generator(int max, int min)
{
	return (rand() % (max - min + 1)) + min;
}

graph *create_graph(int graph_size)
{
	graph *graph = malloc(sizeof(graph));

	// adicionar os vértices
	vertice *vertice_head = add_vertice(1);
	vertice *vertice_list = vertice_head;

	for (int i = 2; i <= graph_size; i++)
	{
		vertice_list->next_vertice = add_vertice(i);
		vertice_list = vertice_list->next_vertice;
	}

	// adicionar arestas
	edge *edge_head = add_edge(1, 2);
	edge *edge_list = edge_head;

	for (int i = 2; i < graph_size; i++)
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
	new_edge->weight = random_generator(MAX_WEIGHT, MIN_WEIGHT);

	return new_edge;
}

vertice *add_vertice(int n)
{
	vertice *new_vertice = malloc(sizeof(vertice));

	new_vertice->next_vertice = NULL;
	new_vertice->id = n;

	return new_vertice;
}