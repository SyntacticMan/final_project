#ifndef FILE_MODULE
#define FILE_MODULE
#include "file_module.h"
#endif

void write_file(header *file_header, int **graph, char *filename)
{
    // abrir o ficheiro em modo de escrita binária
    // assim certifico-me que, se ele já existir, é apagado
    FILE *graph_file = fopen(filename, "wb");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir %s.\n", filename);
        return;
    }

    // guardar o tamanho no cabeçalho e escrevê-lo no ficheiro
    fwrite(file_header, sizeof(header), 1, graph_file);

    // para a adição do grafo é necessário reabrir o ficheiro
    // em modo de apensar para não destruir o cabeçalho
    graph_file = freopen(filename, "ab", graph_file);

    if (graph_file == NULL)
    {
        printf("Nao foi possivel abrir %s.\n", filename);
        return;
    }

    for (int i = 0; i < file_header->array_size; i++)
    {
        fwrite(graph[i], sizeof(int *), 1, graph_file);
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
        graph[i] = (int *)malloc(sizeof(int *));

        fread(graph[i], sizeof(int *), 1, graph_file);
    }

    fclose(graph_file);
    return graph;
}
