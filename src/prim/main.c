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

int main(int argc, char *argv[])
{

    srand(time NULL);
    struct timeval start, end;
    int *v_t;
    float *graph;

    char *graph_filename;
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

    header *graph_header = read_header(graph_filename);

    if (graph_header == NULL)
    {
        printf("Nao foi possivel carregar o cabecalho do grafo\n");
        return -1;
    }

    graph = read_graph(graph_filename, graph_header->graph_size);

    if (graph == NULL)
    {
        return -1;
    }

#ifdef DEBUG
    int graph_root = 2;
    print_graph(graph, graph_header->graph_size);
    printf("\n");
#else
    // se não tiver graph_root no ficheiro, gerar uma
    int graph_root;
    if (graph_header->graph_root > 0)
        graph_root = graph_header->graph_root;
    else
        graph_root = pick_graph_root(graph_header->graph_size);
#endif

    gettimeofday(&start, NULL);
    if (threads <= 1)
    {
        // lançar single thread
        printf("Lançando Algoritmo de Prim em tarefa simples\n");
        v_t = prim_mst(graph, graph_header->graph_size, graph_root);
    }
    else
    {
        threads = 1;
        printf("Lançando Algoritmo de Prim com %d threads\n", threads);
        prim_mt_mst(graph, graph_header->graph_size, graph_root, threads);
    }
    gettimeofday(&end, NULL);

    printf("grafo-> %s\n", graph_filename);
    printf("graph root-> %d\n", graph_root);
    printf("graph size-> %d\n", graph_header->graph_size);

#ifdef DEBUG
    // acima dum certo tamanho é inútil
    if (graph_header->graph_size <= 50)
        print_graph(graph, graph_header->graph_size);

#endif
    // print_mst(d, graph_header->graph_size);

    // Calculate the elapsed time in seconds and microseconds
    double seconds = (double)(end.tv_sec - start.tv_sec);
    double microseconds = (double)(end.tv_usec - start.tv_usec);
    double elapsed_time = seconds + microseconds / 1e6;

    printf("Execution time: %.6f seconds\n", elapsed_time);

    // actualizar o ficheiro do grafo
    write_mst(v_t, (graph_header->graph_size * sizeof(int)), graph_root, graph_filename);

    free(graph);
    free(graph_header);
}
