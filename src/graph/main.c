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
#include <string.h>
#include <time.h>

#ifndef GRAPH
#define GRAPH
#include "graph.h"
#endif

#ifndef FILE_MODULE
#define FILE_MODULE
#include "../file/file_module.h"
#endif

int main(int argc, char const *argv[])
{
	srand(time NULL);
	int graph_size;

	// necessário indicar o tamanho do grafo
	if (argc < 3)
	{
		printf("Utilização: %s -s <int>\n", argv[0]);
		return 1;
	}

	// processar as opções
	if (strcmp(argv[1], "-s") == 0)
	{
		graph_size = atoi(argv[2]);
	}
	else
	{
		printf("Opção %s desconhecida\n", argv[2]);
		return 1;
	}

	int **graph = create_graph(graph_size);

	header *graph_header = malloc(sizeof(header));

	graph_header->array_size = get_array_size(graph_size);
	graph_header->graph_size = graph_size;

	printf("%d\n%d\n", graph_header->graph_size, graph_header->array_size);
	write_file(graph_header, graph, "graph.grf");

	// impressão do grafo de teste
	// while (test->edge_head != NULL)
	// {
	// 	printf("aresta: %d->%d (%d)\n", test->edge_head->u, test->edge_head->v, test->edge_head->weight);
	// 	test->edge_head = test->edge_head->next_edge;
	// }
	// int array_size = get_array_size(graph_size);

	// for (int i = 0; i < array_size; i++)
	// {
	// 	if (graph[i] != NULL)
	// 		printf("linha: %d / vertice: %d\n", i, *graph[i]);
	// 	else
	// 		printf("linha: %d NULL\n", i);
	// }

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
