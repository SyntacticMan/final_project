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
    int array_size;
    int edge_percentage;
} header;

void write_file(header *file_header, int **graph, char *filename);
int **read_file(char *filename, header *file_header);
