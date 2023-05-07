/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Implementação em tarefa simples                                  *
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef PRIM_ST
#define PRIM_ST
#include "prim_st.h"
#endif

void prim_mst()
{
    // v_t = root_node
    // d -> inicializado totalmente a 0, d[r] = 0;

    /*
        graph -> v_t tem de se passar de graph para v_t e libertar para o v_t para não queimar a memória toda

        d retêm o menor peso da aresta para o vértice v

        tem de percorrer todo o grafo para um dado vértice para se obter a aresta com o menor peso desse vértice
        esse vértice depois é removido
    */
}