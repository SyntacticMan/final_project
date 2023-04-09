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
int check_edge(edge *edge_head, int u, int v);

int random_generator(int max, int min);

int main()
{
	srand(time NULL);

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

	return 0;
}

int random_generator(int max, int min)
{
	return (rand() % (max - min + 1)) + min;
}

graph *create_graph()
{
	int graph_size = random_generator(GRAPH_SIZE_MAX, GRAPH_SIZE_MIN);
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
	// escolher os vertices a ligar
	int edge_count = 10;
	int u = 0;
	int v = 0;

	edge *edge_head = add_edge(u, v);
	edge *edge_list = edge_head;

	while (edge_count >= MAX_EDGE)
	{

		u = random_generator(1, graph_size);
		v = random_generator(1, graph_size);

		edge_count = check_edge(edge_head, u, v);
	}

	edge_list->next_edge = add_edge(u, v);
	edge_list = edge_list->next_edge;

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

int check_edge(edge *edge_head, int u, int v)
{
	// se ainda não tiverem sido criadas arestas
	// devolver logo contagem a 0
	if (edge_head == NULL)
		return 0;

	edge *edge_list = edge_head;
	int u_count = 0;
	int v_count = 0;

	while (edge_list->next_edge != NULL)
	{
		if (edge_list->u == u)
			u_count++;

		if (edge_list->v == v)
			v_count++;
	}

	// devolver a maior das contagens
	if (u_count >= v_count)
		return u_count;
	else
		return v_count;
}

vertice *add_vertice(int n)
{
	vertice *new_vertice = malloc(sizeof(vertice));

	new_vertice->next_vertice = NULL;
	new_vertice->id = n;

	return new_vertice;
}