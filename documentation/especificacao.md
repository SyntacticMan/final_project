# Especificação do projecto

O projecto é uma implementação do Algoritmo de Prim para obter uma árvore geradora mínima, sobre grafos densos,não direcionados. Esta implementação terá duas vertentes, uma em que é apenas utilizado um processo para executar o algoritmo e obter resultados e outra em que serão utilizados múltiplos processos. No final tanto os grafos como a árvore geradora mínima e os respectivos tempos de execução serão mostrados para serem comparados.

## Componentes
O projecto tem duas componentes, o gerador de grafos e a implementação do algoritmo. Estas componentes serão separadas, sendo que os grafos gerados serão escritos num ficheiro que depois será lido pelo executável do algoritmo.

### Gerador de grafos
Os grafos a ser gerados são densos, ou seja, com múltiplas arestas por vértice. Estes grafos serão representados em matriz de adjacência, mas apenas a triangular superior, uma vez que estas matrizes acabam por ser sempre simétricas. 

A matriz de adjacência será representada por um vetor de apontadores unidimensional, em que a bidimensionalidade é simulada pelas funções de escrita e leitura. Isto porque utilizar um vetor bidimensional iria resultar na ocupação de memória com entradas que nunca seriam utilizadas. Assim as colunas serão tantas quantas os vértices mas cada coluna terá $n-1$ linhas, ou seja, a coluna $1$ terá $0$ linhas, $2$ terá $1$ linhas e por aí adiante. Este esquema omite os $0$ que existem quando $A(i,j), i=j$.

O número de arestas é estipulado dando uma percentagem, como opção de linha de comandos, do número total de arestas. Independentemente da percentagem, todos os vértices terão de ter, pelo menos, uma aresta. Cada aresta criada terá de ser avaliada contra a percentagem total de arestas que ainda faltam preencher de modo a não exceder o limite indicado. O número total de arestas é dado por $\frac{n\times(n-1)}{2}$. Estas serão atribuídas segundo uma distribuição normal.

Os pesos a atribuir a cada aresta serão indicados como um intervalo mínimo e máximo de inteiros positivos, com um mínimo absoluto de 0. Cada vértice sem ligação é representado na matriz como um infinito, recorrento a uma biblioteca em C adequada.

Depois de criado o grafo este é emitido para o disco num ficheiro binário que o executável do algoritmo depois irá carregar.


### Algoritmo de Prim
O algoritmo de Prim, na sua versão de processo único e multi processo, será implementado num único executável. O número de processos será indicado na linha de comando, sendo que por omissão, ou se fôr indicado apenas um, ele executará a versão de processo simples. Quando a árvore geradora mínima fôr encontrada deve ser produzido um relatório com o tempo de execução e o número de processos utilizados, bem como um desenho do grafo e da àrvore geradora mínima. 

Como o grafo vem representado numa matriz de adjacência triangular superior, as chamadas que envolvam a triangular inferior terão de ser simuladas. Por isso a função que vai obter o peso para um dado vértice terá de saber ir buscá-lo à triangular superior quando este pertencer à triangular inferior.

Para a execução em multi-processos, o array é divido em blocos de $\frac{n}{p}$ vértices consecutivos que são depois alimentados aos processos. Cada processo fica responsável por encontrar o mínimo local no bloco do grafo que lhe foi atribuído e coloca o resultado na parte do array de resultados que lhe corresponde. No fim da execução o mínimo global é obtido em todos os arrays de resultados para produzir o array de resultados final, a árvore geradora mínima.

Deste array de resultados será produzida representação gráfica da árvore geradora mínima, bem como uma tabela com os tempos de execução. Se tiverem sido pedidos múltiplos processos, a tabela deve incluir comparações entre ambas as versões.

## Detalhes técnicos

### Formato do ficheiro do grafo
O ficheiro do grafo deve ser binário e ter o tamanho do grafo (número de vértices) no ínicio. O nome é determinado na invocação do gerador de grafos e indicado ao executável do algoritmo quando este é iniciado. A extensão será *.grf*.

### Representação da matriz de adjacência
A matriz de adjacência é representada por um array de *pointers*, em que cada elemento contém o peso da aresta ou **infinito** se não tiver ligação. Apenas é representada a parte triangular superior da matriz, sendo a parte triangular inferior obtida deduzindo qual a aresta que lhe corresponde na parte triangular superior. 

### Repartição do trabalho
A cada processo é entregue um array com os vértices pelos quais é responsável e outro para o qual deve escrever os resultados. No caso do processo único o array contém todos os vértices e não há o passo de união dos vários arrays de resultados.

Todas as alocações de memória para os arrays devem ser feitas dentro do *while* de modo a cada um tenha a sua própria memória e seja possível fazer as comparações necessárias sem ter de declarar um array por cada processo.

### Apresentação dos resultados
No fim do processamento é necessário apresentar o grafo gerado bem como a árvore geradora mínima, o grafo que contém apenas as ligações com o menor peso. Será necessário encontrar ou produzir uma biblioteca gráfica que seja capaz de representar os grafos de um modo automático. Como não foi indicado um tamanho máximo de grafos, terá de ser capaz de representar, pelo menos, grafos com uma centena de vértices. As imagens geradas serão depositadas na pasta de execução como *.png*, uma vez que a análise de desempenho será apresentada como texto no final da execução.

A tabela de resultados terá as seguintes colunas:

* Tamanho Grafo
* Número de arestas processadas
* Tamanho Árvore 
* Tempo Execução