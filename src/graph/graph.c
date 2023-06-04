/********************************************************************
 *  Projecto Final em Engenharia Informática                        *
 *                                                                  *
 * Programação Paralela Multi tarefa								*
 * Algoritmo de Prim para obter uma árvore geradora mínima			*
 * 																	*
 * Biblioteca de grafos												*
 * 																	*
 * Nuno Méren														*
 * 1902937															*
 * 																	*
 * Orientador: Paulo Shirley										*
 * ******************************************************************/

#ifndef GRAPH
#define GRAPH
#include "graph.h"
#endif

int **graph;

static void add_edge(int u, int v);
static void add_null_edge(int u, int v);

static int get_index(int col, int row);
static double get_edge_probability(int graph_size, double requested_edge_percentage);

static int random_generator(int max, int min);

/*
 *   random_generator

 *   gera um inteiro entre min e max
 */
int random_generator(int max, int min)
{
    return (rand() % (max - min + 1)) + min;
}

/*
 * create_graph

 * cria um novo grafo, com lista de vértices e arestas
 */
void create_graph(int graph_size, int edge_percentage)
{
    // alocar as colunas
    graph = (int **)malloc(get_array_size(graph_size) * sizeof(int *));

    if (graph == NULL)
    {
        return;
    }
    double edge_probability = get_edge_probability(graph_size, edge_percentage);
    printf("Edge probability %f\n", edge_probability);

    // uma primeira passagem para assegurar
    // que todos os vértices têm, pelo menos, uma aresta
    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row <= graph_size; row++)
        {
            add_edge(col, row);
        }
    }

    for (int col = 1; col <= graph_size; col++)
    {
        for (int row = 1; row < graph_size; row++)
        {
            // se linha = coluna o vértice ligar-se-ia a ele mesmo
            // se linha > coluna estou na triangular inferior
            // em ambos os casos passo à frente
            if ((row >= col))
                continue;

            if ((double)random_generator(100, 1) < edge_probability)
                add_edge(col, row);
            else
                add_null_edge(col, row);
        }
    }
}

/*
    get_index

    converte a coluna e linha no índice correspondente
    no vetor que guarda o grafo
*/
int get_index(int col, int row)
{
    // o array apenas guarda colunas a partir da 2
    if (col <= 2)
        return 0;

    int n = col - 2;
    int index = (n * (n + 1)) / 2;

    // adicionar a linha ao índice
    // para obter índice da coluna/linha
    if (row > 1)
        index += row - 1;

    return index;
}

/*
    get_array_size

    calcula o tamanho necessário que o array tem de ter
    com base no índice da última coluna
*/
int get_array_size(int graph_size)
{
    return get_index(graph_size, 0) + (graph_size - 1);
}

/*
 * add_edge

 * cria uma aresta entre os vértices u e v
 */
void add_edge(int u, int v)
{
    int index = get_index(u, v);

    // o peso é atribuído ao acaso
    int *weight = (int *)malloc(sizeof(int));
    *weight = random_generator(MAX_WEIGHT, MIN_WEIGHT);

    graph[index] = weight;
}

/*
    add_null_edge

    adiciona uma "não-ligação" entre os vértices u e v
    representada por um infinito
*/
void add_null_edge(int u, int v)
{
    int index = get_index(u, v);

    // como o vetor é de inteiros
    // uso -1 para representar o infinito
    int *inf = (int *)malloc(sizeof(int));
    *inf = -1;
    graph[index] = inf;
}

int **allocate_v_t(int graph_size)
{
    return NULL; // v_t = calloc(graph_size, sizeof(int *));
}

/*
    get_edge

    obtém a aresta, ou a ausência dela,
    correspondente à coluna e linha indicada
*/
int get_edge(int **graph, int u, int v)
{

    // um vértice não tem ligação com ele mesmo
    // logo a aresta é sempre 0
    if (u == v)
        return 0;

    // se a coluna fôr mais pequena que a linha
    // é porque foi pedida uma aresta da triangular inferior
    // portanto vou inverter coluna e linha para obter a simétrica
    if (u < v)
    {
        int temp = u;
        u = v;
        v = temp;
    }

    int index = get_index(u, v);

    if (graph[index] == NULL)
    {
        return 0;
    }
    else
    {
        return *graph[index];
    }
}

/*
    get_max_edge_count

    obtém o número máximo de arestas que o grafo pode ter
*/
int get_max_edge_count(int graph_size)
{
    return ((graph_size * (graph_size - 1)) / 2);
}

/*
    get_edge_count

    obtém o número de arestas existentes no grafo
*/
int get_edge_count(int graph_size)
{
    int edge_count = 0;

    for (int i = 0; i < graph_size; i++)
    {
        for (int j = 0; j < graph_size; j++)
        {
            // para evitar contar duas vezes
            // apenas pedir da triangular superior
            // também ignorando i=j
            if (j < i && get_edge(graph, i, j) > 0)
            {
                edge_count++;
            }
        }
    }

    return edge_count;
}

/*
    get_edge_percentage

    calcula a percentagem de arestas existentes no grafo
    contra o número máximo de arestas que pode ter
*/
float get_edge_percentage(int graph_size)
{
    return ((float)get_edge_count(graph_size) / (float)get_max_edge_count(graph_size)) * 100.0;
}

void remove_edge(int u, int v)
{
    int index = get_index(u, v);

    int infinite = -1;
    graph[index] = &infinite;
}

/*
    get_edge_probability

    Calcula a possibilidade de uma aresta ser criada, tendo em conta
    a percentagem de arestas pedidas e o tamanho do grafo
*/
double get_edge_probability(int graph_size, double requested_edge_percentage)
{

    double edge_probability = 2.0 * (requested_edge_percentage * get_max_edge_count((double)graph_size)) / ((double)graph_size * ((double)graph_size - 1.0));

    // se o grafo fôr muito grande e a percentagem muito baixa
    // a probabilidade pode ficar negativa, nesse caso devolvo 1
    if (edge_probability < 0)
        edge_probability = 1.0;

    return edge_probability;
}

/*
    pick_graph_root

    escolhe um vértice ao acaso como raíz do grafo
*/
int pick_graph_root(int graph_size)
{
    return random_generator(graph_size, 1);
}

/*
    print_graph

    imprime a matriz de adjacência do grafo na linha de comandos
*/
void print_graph(int graph_size)
{
    for (int col = 0; col < graph_size; col++)
    {
        // printf("%d\n", col);

        for (int row = 0; row < graph_size; row++)
        {
            int edge = get_edge(graph, col, row);
            // printf("%d", row);
            if (edge <= 0)
                printf("|inf");
            else
                printf("|%3d", edge);
        }

        printf("\n");
    }
}

/*
    draw_graph

    cria uma representação gráfica do grafo em contexto
*/
void draw_graph(int **graph_to_draw, int graph_size, const char *filename, char *graph_title)
{
    Agraph_t *g;
    Agnode_t *n, *m;
    Agedge_t *e;
    char string_temp[50];

    // criar o contexto de visualização
    gvc = gvContext();

    // preparar a opção com o nome do ficheiro a gerar
    char image_name[50] = "-o";
    strcat(image_name, filename);

    // definir algoritmo de colocação, tipo e nome da imagem a gerar
    char *args[] = {
        "dot",
        "-Tpng",
        image_name};

    gvParseArgs(gvc, sizeof(args) / sizeof(char *), args);

    // criar o grafo, com disciplina por omissão
    g = agopen("g", Agundirected, NULL);

    agsafeset(g, "label", graph_title, "");
    agsafeset(g, "labelloc", "t", "");

    for (int col = 0; col < graph_size; col++)
    {
        for (int row = 0; row < graph_size; row++)
        {
            sprintf(string_temp, "%d", col);
            n = agnode(g, string_temp, 1);
            agsafeset(n, "shape", "circle", "");

            sprintf(string_temp, "%d", row);
            m = agnode(g, string_temp, 1);
            agsafeset(m, "shape", "circle", "");

            int weight = get_edge(graph_to_draw, col, row);
            if (col < row && weight > 0)
            {
                e = agedge(g, n, m, 0, 1);
                sprintf(string_temp, "%d", weight);
                agsafeset(e, "label", string_temp, "");
            }
        }
    }

    // calcular disposição
    gvLayoutJobs(gvc, g);

    // criar o grafo
    gvRenderJobs(gvc, g);

    // libertar memória
    gvFreeLayout(gvc, g);
    agclose(g);

    // encerrar e terminar o contexto
    gvFreeContext(gvc);
}