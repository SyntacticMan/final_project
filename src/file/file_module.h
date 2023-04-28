/********************************************************************
 *  Módulo file_module                                              *
 *                                                                  *
 *  Biblioteca para ler e gravar os grafos em disco 				*
 *                                                                  *
 * ******************************************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct file_header
{
    int graph_size;
    int array_size;
} header;

void write_file(header *file_header, int **graph, char *filename);
int **read_file(char *filename, header *file_header);
