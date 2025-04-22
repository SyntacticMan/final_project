# TODO List

## Gerador de grafos
* ~~Restruturar o grafo para uma representação em matriz de adjacência~~
* ~~Mudar o algoritmo de geração para criar uma matriz de adjacência triangular superior~~
* ~~tamanho do grafo introduzido pelo utilizador~~
* ~~Arestas geradas tem de ter a percentagem relativa ao total máximo em conta para não a exceder~~
* ~~Apresentar pequeno relatório no fim a indicar o número de vértices, arestas e percentagem relativa gerada e pedida~~
    * ~~contar número máximo de vértices que o grafo pode ter~~
    * ~~contar número de vértices atribuídos~~
    * ~~calcular percentagem de atribuídos vs máxima~~
* ~~Converter o grafo para array simples~~
* ~~__graph__ passa de _int_ para _float_~~
* ~~array_size deve ser calculado e não guardado no cabeçalho do ficheiro~~
* ~~implementar o método Box Muller para gerar números aleatórios~~
* ~~corrigir o não estar a respeitar a percentagem de arestas pedida~~
* ~~iterar n vezes, n = número de arestas a adicionar, adicionado arestas com vértices determinados pelo Box Muller~~
* ~~desenhar os grafos~~
* Adicionar suporte para florestas de grafos
    * Gerar e processar florestas de grafos (nice to have)

## Módulo de ficheiros
* ~~Determinar o formato do ficheiro do grafo~~
* ~~Criar módulo de ficheiros~~
* ~~Implementar tradução do array do grafo para o ficheiro~~
* ~~Disponibilizar função para gravar o ficheiro~~
* ~~Disponibilizar função para ler o ficheiro~~

## Algoritmo Prim
* ~~Implementar módulo singlethread~~
* Implementar módulo multithread
    ~~* Função para separar o array do grafo em blocos~~
    ~~* Função para separar o array da árvore em blocos~~
    ~~* Função para consolidar os resultados dos processos individuais ~~
   ~~ * Comunicação entre threads para determinar o valor mínimo global de __d__~~
* ~~Implementar sistema para cronometrar a execução~~
* ~~Gerar um .png do grafo~~
*~~ Gerar um .png da árvore mínima~~

## Visualizador grafos
* ~~Implementar num binário à parte~~
    * ~~ler o grafo do ficheiro~~
    * ~~ler a árvore mínima do ficheiro~~
* ~~Rever escolha da biblioteca de visualização~~
    * ~~igraph~~
    * ~~graphviz~~


## Resultados
* ~~Preparar maneira para lançar execuções em lote de modo a recolher dados sobre execução~~
* ~~Calcular e exibir dados sobre ganhos no desempenho de processamento~~
    * ~~Calcular tempo de execução sequencial~~
* ~~Descrição da implementação do algoritmo~~
    * ~~Pseudo-código~~
    * ~~Diagramas~~
* ~~Sobrepôr a árvore mínima geradora sobre o grafo~~

