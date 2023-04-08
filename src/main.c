#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int u, v;
} edge;

typedef struct graph_element
{
	int id;
	edge *edge_element;
	struct graph_element *next_element;
} graph_element;

int main()
{
	graph_element *test = malloc(sizeof(graph_element));
	test->id = 1;
	test->edge_element = malloc(sizeof(edge));
	test->edge_element->u = 1;
	test->edge_element->v = 2;

	printf("%d", test->id);
	printf("%d", test->edge_element->u);
	printf("%d\n", test->edge_element->v);

	return 0;
}
