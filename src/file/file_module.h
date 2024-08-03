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

#ifndef LIBS
#include <stdio.h>
#include <string.h>
#define LIBS
#endif

#include <stdlib.h>

typedef struct _file_header
{
    int graph_size;
    int graph_root;
    int edge_percentage;
    int vt_size;
} header;

void write_file(header *file_header, unsigned short int *graph, char *filename);
header *read_header(char *filename);
unsigned short int *read_graph(char *filename, int graph_size);
void write_mst(int *v_t, int vt_size, int graph_root, char *filename);
int *read_mst(char *filename);
void write_result(char *graph_name, int graph_size, double elapsed_time, int edge_percentage, int num_threads);