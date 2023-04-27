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
        // exit(-1);
    }

    // guardar o tamanho no cabeçalho e escrevê-lo no ficheiro
    fwrite(file_header, sizeof(header), 1, graph_file);

    for (int i = 0; i < file_header->size; i++)
    {
        fwrite(&graph[i], sizeof(int *), 1, graph_file);
    }

    fclose(graph_file);
}

void read_file(char *filename)
{
}
