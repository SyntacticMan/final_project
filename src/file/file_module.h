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

#include <stdio.h>
#include <stdlib.h>

typedef struct file_header
{
    int graph_size;
    int graph_root;
    int edge_percentage;
    int vt_size;
} header;

void write_file(header *file_header, float *graph, char *filename);
float *read_file(char *filename, header *file_header);
void write_mst(int *v_t, int vt_size, int graph_root, char *filename);
int *read_mst(char *filename);
