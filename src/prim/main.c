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

#ifndef LIBS
#include <stdio.h>
#include <string.h>
#endif
#include <unistd.h>
#include <sys/time.h>

void print_mst(float *graph, int *v_t, int graph_size, int graph_root, bool with_weights);
void print_banner(void);
void print_usage(void);

int main(int argc, char *argv[])
{

    srand(time NULL);
    struct timeval start, end;
    int *v_t;
    float *graph;

    char *graph_filename = NULL;
    int opt;
    int threads = 1;
    bool print_agm = false;
    bool print_results = false;

    print_banner();

    while ((opt = getopt(argc, argv, "f:t:pr")) != -1)
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
        case 'r':
            print_results = true;
            break;
        default:
            break;
        }
    }

    // se não tiver recebido o nome do ficheiro com o grafo
    // imprimir as opções de utilização e sair
    if (graph_filename == NULL)
    {
        print_usage();
    }

    printf("Carregando grafo %s\n", graph_filename);

    header *graph_header = read_header(graph_filename);

    if (graph_header == NULL)
    {
        printf("Nao foi possivel carregar o cabecalho do grafo\n");
        return EXIT_FAILURE;
    }

    graph = read_graph(graph_filename, graph_header->graph_size);

    if (graph == NULL)
    {
        return EXIT_FAILURE;
    }

    // se não tiver raíz no ficheiro, escolher uma aleatóriamente
    int graph_root;
    if (graph_header->graph_root > 0)
        graph_root = graph_header->graph_root;
    else
        graph_root = pick_graph_root(graph_header->graph_size);

    // emissão de relatório e gravação do mst no grafo
    printf("Raiz: %d\tVértices:%d\tArestas:%d\n", graph_root, graph_header->graph_size, get_edge_count(graph, graph_header->graph_size));

    // se receber a opção, simplesmente ler a AGM e sair
    if (print_agm)
    {
        // if (graph_header->graph_size <= 20)
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

            print_mst(graph, v_t, graph_header->vt_size, graph_header->graph_root, false);
        }
        else
        {
            printf("\nSem AGM em ficheiro.\n");
        }

        gettimeofday(&end, NULL);

        // terminar a execução
        exit(EXIT_SUCCESS);
    }

    // começar a cronometrar a execução da implementação do algoritmo
    gettimeofday(&start, NULL);

#ifdef LOCK_MT
    // este define obriga a executar prim_mt com apenas 1 processo
    // de modo a avaliar e validar a correcção de execução
    if (threads <= 1)
    {
        // lançar single thread
        printf("Lançando Algoritmo de Prim em tarefa simples\n");
        v_t = prim_mst(graph, graph_header->graph_size, graph_root);
        implementation_type = "ST";
    }
    else
    {
        printf("Lançando Algoritmo de Prim MT com 1 thread fixa\n");
        v_t = prim_mt_mst(graph, graph_header->graph_size, graph_root, 1);
        implementation_type = "MT";
    }
#else
    // execução normal
    if (threads <= 1)
    {
        // lançar em tarefa simples
        printf("Procurando a AGM usando tarefa simples\n");
        v_t = prim_mst(graph, graph_header->graph_size, graph_root);
    }
    else
    {
        // lançar em multi-tarefa
        printf("Procurando a AGM com %d processos\n", threads);
        v_t = prim_mt_mst(graph, graph_header->graph_size, graph_root, threads);
    }
#endif

    // emitir o tempo de execução do algoritmo, em segundos
    gettimeofday(&end, NULL);
    double seconds = (double)(end.tv_sec - start.tv_sec);
    double microseconds = (double)(end.tv_usec - start.tv_usec);
    double elapsed_time = seconds + microseconds / 1e6;

    printf("\nAGM calculada em %.6f segundos\n", elapsed_time);

    // actualizar o ficheiro do grafo
    write_mst(v_t, graph_header->graph_size, graph_root, graph_filename);

    // guardar os resultados para estatística
    if (print_results)
        write_result(graph_filename, graph_header->graph_size, elapsed_time, graph_header->edge_percentage, threads);

    // libertar a memória alocada
    free(graph);
    free(graph_header);
    free(v_t);
}

/*
    print_mst

    imprime a versão textual da árvore mínima
*/
void print_mst(float *graph, int *v_t, int graph_size, int graph_root, bool with_weights)
{
    printf("\n");

    for (int i = 1; i < graph_size; i++)
    {
        if (with_weights)
            printf("(%d)-[%0.2f]-(%d)", i, get_edge(graph, v_t[i], i), v_t[i]);
        else
            printf("(%d)-(%d)", i, v_t[i]);

        // omitir na última iteração
        if (i < graph_size - 1)
            printf("--");
    }

    printf("\n");
}

/*
    print_banner

    imprime os dados do executável
*/
void print_banner(void)
{
    printf("*****************************************************\n"
           "* Projecto de Final de Curso - Engenharia Informática\n"
           "* Implementação do Algoritmo de Prim\n"
           "* para obter a Árvore Geradora Mínima dum grafo\n"
           "* Nuno Méren\n"
           "* Aluno 1902937\n"
           "*****************************************************\n\n");
}

/*
    print_usage

    imprime as opções de utilização
    e termina a execução do programa
*/
void print_usage(void)
{
    printf("\n\nUtilização:\nprim [-f ficheiro com o grafo] [-t número processos (1 por omissão)] [-p imprimir grafo] [-r imprimir resultados]\n");
    exit(EXIT_SUCCESS);
}
