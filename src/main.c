/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#include <stdio.h>
#include <time.h>

#ifndef GRAPH
#define GRAPH
#include "graph.h"
#endif

int main()
{
	srand(time NULL);

	graph *test = create_graph();

	// impressão do grafo de teste
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

	// limpeza da memória
	while (test->vertice_head != NULL)
	{
		vertice *temp = test->vertice_head->next_vertice;
		free(test->vertice_head);
		test->vertice_head = temp;
	}

	while (test->edge_head != NULL)
	{
		edge *temp = test->edge_head->next_edge;
		free(test->edge_head);
		test->edge_head = temp;
	}

	free(test);

	return 0;
}
