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

int **prim_mst(int array_size, int graph_size, int graph_root)
{
    // vetor d e v_t são inicializados com o mesmo tamanho que o grafo
    // int **v_t = (int **)malloc(get_array_size(graph_size) * sizeof(int *));
    int *v_t = malloc(graph_size * sizeof(int *));
    int *d = malloc(graph_size * sizeof(int *));

    // inicializar a árvore mínima
    for (int v = 0; v < graph_size; v++)
    {
        // inicializar os vértices visitados
        v_t[v] = false;
        if (v == graph_root)
        {
            v_t[v] = true;
            d[v] = 0;
        }
        else
            d[v] = INFINITE;
    }

    // processar as arestas da raíz
    for (int v = 0; v < graph_size; v++)
    {
        int edge_weight = get_edge(graph, graph_root, v);
        if (edge_weight != INFINITE)
        {
            d[v] = edge_weight;
        }
        // não é preciso aqui pq já foi inicializado
    }

    // int min_edge_weight = MAX_WEIGHT + 1;
    // int u = -1;

    // for (int i = 0; i < graph_size; i++)
    // {
    //     if (!v_t[v] && d[i] < min_edge_weight)
    //     {
    //         min_edge_weight =
    //             int edge_weight = get_edge(graph, graph_root, v);

    //         // se tiver uma ligação, adicionar
    //         if (edge_weight > 0)
    //         {
    //             d[v] = edge_weight;
    //             // add_edge(v_t, graph_root, v, edge_weight);
    //         }
    //         else
    //         {
    //             d[v] = INFINITE;
    //             // add_null_edge(v_t, graph_root, v);
    //         }
    //     }
    // }
    // }

    // for (int u = v; u < graph_size; u++)
    // {
    //     // passar a raíz à frente
    //     if (u == graph_root)
    //         continue;

    //     // if (u == v)
    //     // {
    //     //     // add_edge(v_t, u, v, 0);
    //     //     continue;
    //     // }

    //     // if (visited[u] == 1)
    //     //     continue;

    //     int edge_weight = get_edge(graph, v, u);

    //     if (edge_weight != INFINITE)
    //     {
    //         if (edge_weight > 0 && edge_weight < d[u])
    //         {
    //             min_vertex = u;
    //             d[u] = edge_weight;
    //         }
    //     }
    //     else
    //     {
    //         add_null_edge(v_t, v, u);
    //     }
    // }
    // visited[v] = 1;
    // add_edge(v_t, min_vertex, v, d[min_vertex]);

    // emitir a árvore geradora mínima
    printf("\nÁrvore mínima \n");

    // graph = v_t;
    // print_graph(graph_size);
    printf("    ");

    for (int i = 0; i < graph_size; i++)
    {
        // if (*d[i] > 0)
        printf("%2d|", i);
    }

    printf("\nd[] ");

    for (int i = 0; i < graph_size; i++)
    {
        printf("%2d|", d[i]); // debug
    }
    printf("\n");

    return v_t;
}