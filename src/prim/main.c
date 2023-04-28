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

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("prim \n Implementação do Algoritmo de Prim\n-f <nome> : carrega grafo do ficheiro");
        return 0;
    }

    char *graph_filename;

    if (strcmp(argv[1], "-f") == 0)
    {
        graph_filename = malloc(sizeof(char) * (strlen(argv[2]) + 1));
        strcpy(graph_filename, argv[2]);
    }

    printf("grafo-> %s\n", graph_filename);

    header *graph_header = malloc(sizeof(header));

    int **graph = read_file(graph_filename, graph_header);

    if (graph != NULL)
        printf("file loaded\nGraph size-> %d\nArray size-> %d\n", graph_header->graph_size, graph_header->array_size);

    for (int i = 0; i < graph_header->array_size; i++)
    {
        if (graph[i] != NULL)
            printf("linha: %d / vertice: %d\n", i, *graph[i]);
        else
            printf("linha: %d NULL\n", i);
    }

    return 0;
}
