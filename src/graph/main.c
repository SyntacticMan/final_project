/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Gerador de grafos												*
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

	int **graph = create_graph(6);

	// impressão do grafo de teste
	// while (test->edge_head != NULL)
	// {
	// 	printf("aresta: %d->%d (%d)\n", test->edge_head->u, test->edge_head->v, test->edge_head->weight);
	// 	test->edge_head = test->edge_head->next_edge;
	// }

	for (int i = 0; i < 9; i++)
	{
		printf("vertice: %d\n", *graph[i]);
	}

	// // limpeza da memória
	// while (test->vertice_head != NULL)
	// {
	// 	vertice *temp = test->vertice_head->next_vertice;
	// 	free(test->vertice_head);
	// 	test->vertice_head = temp;
	// }

	// while (test->edge_head != NULL)
	// {
	// 	edge *temp = test->edge_head->next_edge;
	// 	free(test->edge_head);
	// 	test->edge_head = temp;
	// }

	// free(test);

	return 0;
}
