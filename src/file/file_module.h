/********************************************************************
 *  Módulo file_module                                              *
 *                                                                  *
 *  Biblioteca para ler e gravar os grafos em disco 				*
 *                                                                  *
 * ******************************************************************/

#include <stdio.h>

typedef struct file_header
{
    unsigned int long size;
} header;

void write_file(header *file_header, int **graph, char *filename);
void read_file(char *filename);
