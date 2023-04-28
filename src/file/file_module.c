#ifndef FILE_MODULE
#define FILE_MODULE
#include "file_module.h"
#endif

void write_file(header *file_header, int **graph, char *filename)
{
    FILE *graph_file = fopen(filename, "ab");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir o ficheiro.\n");
        return;
    }

    // guardar o tamanho no cabeçalho e escrevê-lo no ficheiro
    fwrite(file_header, sizeof(header), 1, graph_file);

    for (int i = 0; i < file_header->array_size; i++)
    {
        fwrite(&graph[i], sizeof(int *), 1, graph_file);
    }

    fclose(graph_file);
}

int **read_file(char *filename, header *file_header)
{
    FILE *graph_file = fopen(filename, "rb");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir o ficheiro.\n");
        return NULL;
    }

    // carregar o cabeçalho
    fread(file_header, sizeof(header), 1, graph_file);

    // carregar o array
    int **graph = (int **)malloc(file_header->array_size * sizeof(int *));

    for (int i = 0; i < file_header->array_size; i++)
    {
        graph[i] = (int *)malloc(sizeof(int));

        fread(graph[i], sizeof(int), 1, graph_file);
    }

    return graph;
}
