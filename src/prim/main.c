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

void print_mst(float *graph, int *v_t, int graph_size, int graph_root);
char get_letter(int number);

int main(int argc, char *argv[])
{

    srand(time NULL);
    struct timeval start, end;
    int *v_t;
    float *graph;

    char *graph_filename;
    int opt;
    int threads = 1; // por omissão correr apenas numa thread
    bool print_agm = false;

    while ((opt = getopt(argc, argv, "f:t:p")) != -1)
    {
        switch (opt)
        {
        case 'f':
            graph_filename = optarg;
            break;
        case 't':
            threads = atoi(optarg);
            break;
        case 'p':
            print_agm = true;
            break;
        default:
            fprintf(stderr, "Algoritmo de Prim\n");
            fprintf(stderr, "Usage: %s [-f filename] [-t number of threads] [-p print graph]\n", argv[0]);
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

#ifdef LOCK
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

    // emissão de relatório e gravação do mst no grafo
    printf("Grafo -> %s\n", graph_filename);
    printf("Raiz -> %d\n", graph_root);
    printf("Tamanho -> %d\n", graph_header->graph_size);

    gettimeofday(&start, NULL);

    // se receber a opção, simplesmente ler a AGM e sair
    if (print_agm)
    {
        if (graph_header->graph_size <= 20)
            print_graph(graph, graph_header->graph_size);

        // se tiver mst, imprimir
        if (graph_header->vt_size > 0)
        {
            v_t = read_mst(graph_filename);

            if (v_t == NULL)
            {
                printf("Erro ao ler a AGM\n");
                return -1;
            }

            print_mst(graph, v_t, graph_header->vt_size, graph_header->graph_root);
        }
        else
        {
            printf("\nSem AGM em ficheiro.\n");
        }

        gettimeofday(&end, NULL);

        // terminar a execução
        exit(0);
    }

    // execução normal
    if (threads <= 1)
    {
        // lançar single thread
        printf("Lançando Algoritmo de Prim em tarefa simples\n");
        v_t = prim_mst(graph, graph_header->graph_size, graph_root);
    }
    else
    {

#ifdef LOCK
        threads = 1;
#endif

        printf("Lançando Algoritmo de Prim com %d threads\n", threads);
        v_t = prim_mt_mst(graph, graph_header->graph_size, graph_root, threads);
    }

    // emitir o tempo de execução do algoritmo
    gettimeofday(&end, NULL);

    double seconds = (double)(end.tv_sec - start.tv_sec);
    double microseconds = (double)(end.tv_usec - start.tv_usec);
    double elapsed_time = seconds + microseconds / 1e6;

    printf("Execution time: %.6f seconds\n", elapsed_time);

#ifdef DEBUG
    // acima dum certo tamanho é inútil
    if (graph_header->graph_size <= 20)
        print_graph(graph, graph_header->graph_size);

    print_mst(graph, v_t, graph_header->graph_size + 1, graph_header->graph_root);
#endif

    // actualizar o ficheiro do grafo
    write_mst(v_t, graph_header->graph_size, graph_root, graph_filename);

    free(graph);
    free(graph_header);
}

/*
    print_mst

    imprime a versão textual da árvore mínima
*/
void print_mst(float *graph, int *v_t, int graph_size, int graph_root)
{
    printf("\n");

    for (int i = 1; i < graph_size; i++)
    {
        if ((graph_size > 50) && (i > 25) && (i < graph_size - 20))
            continue;

        if (i == graph_root)
            continue;

        printf("(%c)-[%0.2f]-(%c)", get_letter(i), get_edge(graph, v_t[i], i), get_letter(v_t[i]));

        // omitir na última iteração
        if (i < graph_size - 1)
            printf("||");
    }

    printf("\n");
}

char get_letter(int number)
{
    if (number >= 1 && number <= 26)
    {
        return 'a' + (number - 1);
    }
    else
        return 'A';
}