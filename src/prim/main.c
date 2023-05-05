/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Implementação do Algoritmo de Prim                               *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef PRIM_ST
#define PRIM_ST
#include "prim_st.h"
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.h"
#endif

#ifndef FILE_MODULE
#define FILE_MODULE
#include "../file/file_module.h"
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    char *graph_filename;
    int opt, threads;

    while ((opt = getopt(argc, argv, "f:t")) != -1)
    {
        switch (opt)
        {
        case 'f':
            graph_filename = optarg;
            break;
        case 't':
            threads = atoi(optarg);
        default:
            fprintf(stderr, "Algoritmo de Prim\n");
            fprintf(stderr, "Usage: %s [-f filename] [-t number of threads]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    printf("grafo-> %s\n", graph_filename);

    header *graph_header = malloc(sizeof(header));

    graph = read_file(graph_filename, graph_header);
}