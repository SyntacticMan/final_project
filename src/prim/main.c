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

#ifndef PRIM_MT
#define PRIM_MT
#include "prim_mt.h"
#endif

#ifndef FILE_MODULE
#define FILE_MODULE
#include "../file/file_module.h"
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

void print_mst(float *d, int graph_size);

int main(int argc, char *argv[])
{

    srand(time NULL);
    struct timeval start, end;

    char *graph_filename;
    float *d;
    int opt;
    int threads = 1; // por omissão correr apenas numa thread

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

#ifdef DEBUG
    int graph_root = 1;
    print_graph(graph_header->graph_size);
    printf("\n");
#else
    int graph_root = pick_graph_root(graph_header->graph_size);
#endif

    gettimeofday(&start, NULL);
    if (threads <= 1)
    {
        // lançar single thread
        printf("Lançando Algoritmo de Prim em tarefa simples\n");
        d = prim_mst(get_matrix_size(graph_header->graph_size), graph_header->graph_size, graph_root);
    }
    else
    {
        printf("Lançando Algoritmo de Prim com %d threads\n", threads);
        d = prim_mt_mst(get_matrix_size(graph_header->graph_size), graph_header->graph_size, graph_root, threads);
    }
    gettimeofday(&end, NULL);

    printf("grafo-> %s\n", graph_filename);
    printf("graph root-> %d\n", graph_root);

    print_graph(graph_header->graph_size);

    print_mst(d, graph_header->graph_size);

    // Calculate the elapsed time in seconds and microseconds
    double seconds = (double)(end.tv_sec - start.tv_sec);
    double microseconds = (double)(end.tv_usec - start.tv_usec);
    double elapsed_time = seconds + microseconds / 1e6;

    printf("Execution time: %.6f seconds\n", elapsed_time);
}

void print_mst(float *d, int graph_size)
{
    printf("\n");
    printf("    ");
    for (int i = 1; i <= graph_size; i++)
    {
        printf("%2d|", i);
    }
    printf("\nd[] ");
    for (int i = 1; i <= graph_size; i++)
    {
        printf("%2f|", d[i]);
    }
    printf("\n");
}