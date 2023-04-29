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
#include <unistd.h>

#ifndef GRAPH
#define GRAPH
#include "graph.h"
#endif

#ifndef FILE_MODULE
#define FILE_MODULE
#include "../file/file_module.h"
#endif

int main(int argc, char *argv[])
{
	srand(time NULL);

	int graph_size, edge_percentage;
	char *graph_filename;

	int opt;
	while ((opt = getopt(argc, argv, "s:f:p:")) != -1)
	{
		switch (opt)
		{
		case 's': // tamanho do grafo
			graph_size = atoi(optarg);
			break;
		case 'f': // nome do ficheiro
			graph_filename = optarg;
			break;
		case 'p': // percentagem do máximo de arestas a adicionar
			edge_percentage = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Usage: %s [-s graph_size] [-f filename] [-p edge_percentage]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	create_graph(graph_size, edge_percentage);

	if (graph == NULL)
	{
		printf("Nao foi possivel criar o grafo.\n");
		return -1;
	}

	// preparar o grafo para ser escrito no ficheiro
	header *graph_header = malloc(sizeof(header));

	graph_header->array_size = get_array_size(graph_size);
	graph_header->graph_size = graph_size;

	printf("%d\n%d\n", graph_header->graph_size, graph_header->array_size);
	write_file(graph_header, graph, graph_filename);

	for (int i = 0; i < graph_header->array_size; i++)
	{
		if (graph[i] != NULL)
			printf("linha: %d / vertice: %d\n", i, *graph[i]);
		else
			printf("linha: %d NULL\n", i);
	}

	return 0;
}
