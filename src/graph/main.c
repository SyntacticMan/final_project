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

	int graph_size, requested_edge_percentage;
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
			requested_edge_percentage = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Usage: %s [-s graph_size] [-f filename] [-p requested_edge_percentage]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	create_graph(graph_size, requested_edge_percentage);

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

	int max_edge_count = get_max_edge_count(graph_size);
	int edge_count = get_edge_count(graph_size);
	int actual_edge_percentage = (int)get_edge_percentage(graph_size);

	printf("maximo arestas: %d\narestas existentes: %d\n", max_edge_count, edge_count);

	printf("percentagem de arestas: %d%%\n", actual_edge_percentage);
	write_file(graph_header, graph, graph_filename);

	print_graph(graph_header->graph_size);
	return 0;
}
