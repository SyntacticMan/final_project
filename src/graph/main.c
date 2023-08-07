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
#include <unistd.h>
#include <stdbool.h>

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

	int graph_size, requested_edge_percentage;
	char *graph_filename;
	bool print_matrix = false;

	int opt;
	while ((opt = getopt(argc, argv, "s:f:p:m")) != -1)
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
		case 'm': // imprimir a matriz de adjacência
			print_matrix = true;
			break;
		default:
			fprintf(stderr, "Usage: %s [-s graph_size] [-f filename] [-p requested_edge_percentage]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	// no imediato vou limitar o grafo a um máximo de 23000 vértices
	// mais do que isso e começa a ter problemas em alocar memória
	// adicionalmente limitado a 30 vértices pois graphviz tem um máximo de 400 arestas
	// if (graph_size <= 1 || graph_size > 30)
	// {
	// 	printf("Grafo tem de ter um tamanho entre 2 e 30\n");
	// 	return -1;
	// }

	// percentagens a 0 ou negativas
	if (requested_edge_percentage < 0 || requested_edge_percentage > 100)
	{
		printf("Percentagem de arestas deve ser escolhida entre 0 e 100\n");
		return -1;
	}

	printf("Array RAM size: %lu kb (%d)\n", ((get_matrix_size(graph_size) * sizeof(float)) / 1024), get_matrix_size(graph_size));

	// #ifdef DEBUG
	// 	graph_size = 6;
	// 	create_locked_graph(graph_size, requested_edge_percentage);
	// #else
	create_graph(graph_size, requested_edge_percentage);
	// #endif

	if (graph == NULL)
	{
		printf("Nao foi possivel criar o grafo.\n");
		return -1;
	}
	printf("Graph created\n");

	// obter as contagens das arestas
	// número máximo de arestas é simplesmente o número de elementos da matriz de adjacência
	int max_edge_count = get_matrix_size(graph_size);
	int actual_edge_percentage = (int)get_edge_percentage(graph_size);

#ifdef DEBUG
	if (print_matrix)
		print_graph(graph_size);
#endif

	// emitir o relatório de criação
	printf("Grafo criado\n");
	printf("Tamanho do grafo: %d\n", graph_size);
	printf("Numero maximo de arestas: %d\n", max_edge_count);
	printf("Percentagem de arestas pedidas: %d%% | Percentagem de arestas atribuidas: %d%%\n", requested_edge_percentage, actual_edge_percentage);

	// gravar o grafo no ficheiro
	header *graph_header = malloc(sizeof(header));

	graph_header->array_size = get_matrix_size(graph_size);
	graph_header->graph_size = graph_size;
	graph_header->edge_percentage = actual_edge_percentage;

	write_file(graph_header, graph, graph_filename);

	free(graph);

	// // preparar o título do grafo
	// char graph_title[50] = "Grafo (";
	// char string_temp[50];

	// sprintf(string_temp, "%d", graph_size);
	// strcat(graph_title, string_temp);
	// strcat(graph_title, " vértices)");

	return 0;
}
