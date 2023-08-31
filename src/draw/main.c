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

int main(int argc, char *argv[])
{
    char *graph_filename;
    int opt;
    char *graph_title;
    float *graph;
    int *v_t;

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
            fprintf(stderr, "Draw Graph\n");
            fprintf(stderr, "Usage: %s [-f filename] [-t graph title]\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    // carregar o ficheiro
    header *graph_header = read_header(graph_filename);

    if (graph_header == NULL)
        return -1;

    graph = read_graph(graph_filename, graph_header->graph_size);

    if (graph == NULL)
    {
        printf("Nao foi possivel carregar o grafo\n");
        return -1;
    }
    printf("%d\n", graph_header->graph_root);

    if (graph_header->vt_size > 0)
    {
        v_t = read_mst(graph_filename);

        printf("v_t: %d\n", v_t[0]);
    }

    printf("root: %d\n", graph_header->graph_root);

    // preparar o título do grafo
    char string_temp[50];
    int graph_size = graph_header->graph_size;

    sprintf(string_temp, " (%d ", graph_size);
    strcat(graph_title, string_temp);
    strcat(graph_title, "vertices)");

    // emitir o relatório do grafo carregado
    printf("Grafo carregado\n");
    printf("Tamanho do grafo: %d\n", graph_size);
    printf("Numero arestas: %d\n", get_edge_count(graph, graph_header->graph_size));

    draw_graph(graph, graph_size, "graph_draw.png", graph_title);

    free(graph);
    free(graph_header);
    return 0;
}
