/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Biblioteca para a gravação e leitura de grafos em ficheiro       *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef FILE_MODULE
#define FILE_MODULE
#include "file_module.h"
#endif

void write_file(header *graph_header, float *graph, char *filename)
{
    // abrir o ficheiro em modo de escrita binária
    // assim certifico-me que, se ele já existir, é apagado
    FILE *graph_file = fopen(filename, "wb");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir %s.\n", filename);
        return;
    }

    fwrite(graph_header, sizeof(header), 1, graph_file);

    // para a adição do grafo é necessário reabrir o ficheiro
    // em modo de apensar para não destruir o cabeçalho
    graph_file = freopen(filename, "ab", graph_file);

    if (graph_file == NULL)
    {
        printf("Nao foi possivel abrir %s.\n", filename);
        return;
    }

    unsigned long int array_size = (graph_header->graph_size * (graph_header->graph_size - 1)) / 2;

    for (unsigned long int i = 0; i < array_size; i++)
    {
        fwrite(&graph[i], sizeof(float), 1, graph_file);
    }

    fclose(graph_file);
}

void write_mst(int *v_t, int graph_size, int graph_root, char *filename)
{
    // abrir o ficheiro em modo de leitura e escrita binária
    FILE *graph_file = fopen(filename, "rb+");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir %s.\n", filename);
    }

    // carregar o cabeçalho
    header *g_header = (header *)malloc(sizeof(header));

    if (g_header == NULL)
    {
        printf("Nao foi possivel alocar graph_header\n");
        return;
    }

    fread(g_header, sizeof(header), 1, graph_file);

#ifdef DEBUG
    printf("graph_root: %d\nvt_size: %d\n", graph_root, graph_size + 1);
#endif

    // registar o tamanho de vt, raíz da mst e atualizar o cabeçalho
    g_header->vt_size = graph_size + 1;
    g_header->graph_root = graph_root;

    // rebobinar o apontador
    fseek(graph_file, 0, SEEK_SET);
    fwrite(g_header, sizeof(header), 1, graph_file);

    //  v_t tem de ser escrito *após* o grafo
    // por isso é necessário fazer avançar o apontador para o fim do grafo
    unsigned long int array_size = (g_header->graph_size * (g_header->graph_size - 1)) / 2;
    fseek(graph_file, array_size, SEEK_CUR);

    v_t[0] = 0;
    for (int i = 0; i < g_header->vt_size; i++)
    {
        fwrite(&v_t[i], sizeof(int), 1, graph_file);
    }

    fclose(graph_file);
}

header *read_header(char *filename)
{
    FILE *graph_file = fopen(filename, "rb");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir o ficheiro.\n");
        return NULL;
    }

    header *graph_header = malloc(sizeof(header));

    fread(graph_header, sizeof(header), 1, graph_file);

    return graph_header;
}

float *read_graph(char *filename, int graph_size)
{
    FILE *graph_file = fopen(filename, "rb");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir o ficheiro.\n");
        return NULL;
    }

    // avançar para lá do cabeçalho
    fseek(graph_file, sizeof(header), SEEK_SET);

    unsigned long int array_size = (graph_size * (graph_size - 1)) / 2;

    // carregar o vetor
    float *graph = malloc(array_size * sizeof(float));

    for (unsigned long int i = 0; i < array_size; i++)
    {
        fread(&graph[i], sizeof(float), 1, graph_file);
    }

    fclose(graph_file);
    return graph;
}

int *read_mst(char *filename)
{
    // aberto em modo de leitura apenas
    FILE *graph_file = fopen(filename, "rb");

    if (!graph_file)
    {
        printf("Nao foi possivel abrir o ficheiro.\n");
        return NULL;
    }

    // carregar o cabeçalho
    header *graph_header = malloc(sizeof(header));
    fread(graph_header, sizeof(header), 1, graph_file);

    // o v_t está a seguir ao grafo, por isso tenho de fazer avançar o ponteiro
    unsigned long int array_size = (graph_header->graph_size * (graph_header->graph_size - 1)) / 2;
    fseek(graph_file, array_size, SEEK_CUR);

    // carregar v_t
    int *v_t = (int *)malloc(graph_header->vt_size * sizeof(int));

    for (int i = 0; i < graph_header->vt_size; i++)
    {
        fread(&v_t[i], sizeof(int), 1, graph_file);
    }

    return v_t;
}
