#include <stdlib.h>

#define MAX_EDGE 3
#define MIN_EDGE 1

#define MAX_WEIGHT 15
#define MIN_WEIGHT 1

#define GRAPH_SIZE_MIN 10
#define GRAPH_SIZE_MAX 50

typedef struct edge
{
    int u, v;
    int weight;
    struct edge *next_edge;
} edge;

typedef struct vertice
{
    int id;
    struct vertice *next_vertice;
} vertice;

typedef struct graph
{
    struct vertice *vertice_head;
    struct edge *edge_head;
} graph;

graph *create_graph();
vertice *add_vertice(int n);

edge *add_edge(int u, int v);
int check_edge(edge *edge_head, int vertice);

int random_generator(int max, int min);