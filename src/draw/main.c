/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Ferramenta para criar a representação gráfica do grafo           *
 * e sua Àrvore Geradora Miníma
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef DRAW_GRAPH
#define DRAW_GRAPH
#include "draw_graph.h"
#endif

#ifndef FILE_MODULE
#define FILE_MODULE
#include "../file/file_module.h"
#endif

#include <unistd.h>
#include <string.h>

void print_banner(void);
void print_usage(void);

int main(int argc, char *argv[])
{
    char *graph_filename = NULL;
    int opt;
    char *graph_title;
    float *graph;
    int *v_t;

    print_banner();

    // processar as opções da linha de comandos
    while ((opt = getopt(argc, argv, "f:t:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            graph_filename = optarg;
            break;
        case 't':
            graph_title = optarg;
            break;
        default:
            break;
        }
    }

    // se não houver um ficheiro com o grafo não é possível prosseguir
    if (graph_filename == NULL)
    {
        print_usage();
    }

    // carregar o ficheiro
    header *graph_header = read_header(graph_filename);

    if (graph_header == NULL)
        return -1;

    graph = read_graph(graph_filename, graph_header->graph_size);

    if (graph == NULL)
    {
        printf("Não foi possível carregar o grafo\n");
        exit(EXIT_FAILURE);
    }

    if (graph_header->vt_size > 0)
    {
        v_t = read_mst(graph_filename);

#ifdef DEBUG
        printf("v_t[1]: %d\nv_t[vt_size]: %d\nvt_size:%d\n", v_t[1], v_t[graph_header->vt_size], graph_header->vt_size);
#endif
    }

    // preparar o título do grafo
    char string_temp[50];

    sprintf(string_temp, " (%d ", graph_header->graph_size);
    strcat(graph_title, string_temp);
    strcat(graph_title, "vertices)");

    // emitir o relatório do grafo carregado
    printf("Grafo carregado\n");
    printf("Tamanho do grafo: %d\n", graph_header->graph_size);
    printf("Numero arestas: %d\n", get_edge_count(graph, graph_header->graph_size));

    draw_graph(graph, graph_header->graph_size, graph_header->graph_root, v_t, graph_header->vt_size, "graph_draw.svg", graph_title);

    free(graph);
    free(graph_header);
    return 0;
}

/*
    print_banner

    imprime os dados do executável
*/
void print_banner(void)
{
    printf("*****************************************************\n"
           "* Projecto de Final de Curso - Engenharia Informática\n"
           "* Ferramenta para criar a representação gráfica\n"
           "* do grafo indicado e respetiva AGM\n"
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
    printf("Utilização:\ndrawgraph [-f ficheiro com o grafo] [-t titulo do grafo]\n");
    exit(EXIT_SUCCESS);
}