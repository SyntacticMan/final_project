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

#ifndef FILE_MODULE
#define FILE_MODULE
#include "../file/file_module.h"
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{

    srand(time NULL);

    char *graph_filename;
    int opt;
    int threads = 0;

    while ((opt = getopt(argc, argv, "f:t:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            graph_filename = optarg;
            break;
        case 't':
            threads = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Algoritmo de Prim\n");
            fprintf(stderr, "Usage: %s [-f filename] [-t number of threads]\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    header *graph_header = malloc(sizeof(header));

    graph = read_file(graph_filename, graph_header);

    if (graph == NULL)
    {
        return -1;
    }

    int graph_root = pick_graph_root(graph_header->graph_size);

    if (threads <= 1)
    {
        // lançar single thread
        printf("graph_size -> %d\n", graph_header->graph_size);
    }
    else
    {
        // lançar o número de threads pedidas
        printf("Multi thread\n");
    }
    printf("grafo-> %s\n", graph_filename);
    printf("graph root-> %d\n", graph_root);

    if (graph_header->graph_size <= 30)
        print_graph(graph_header->graph_size);

    prim_mst(graph_header->array_size, graph_header->graph_size, graph_root);
}