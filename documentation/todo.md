# TODO List

## Gerador de grafos
* Restruturar o grafo para uma representação em matriz de adjacência
* tamanho do grafo introduzido pelo utilizador
* Mudar o algoritmo de geração para criar uma matriz de adjacência superior
* Arestas geradas tem de ter a percentagem relativa ao total máximo para não a exceder
* Apresentar pequeno relatório no fim a indicar o número de vértices, arestas e percentagem relativa gerada e pedida

## Módulo de ficheiros
* Determinar o formato do ficheiro do grafo
* Criar módulo de ficheiros
* Implementar tradução do array do grafo para o ficheiro
* Disponibilizar função para gravar o ficheiro
* Disponibilizar função para ler o ficheiro

## Algoritmo Prim
* Implementar módulo singlethread
* Implementar módulo multithread
    * Função para separar o array do grafo em blocos
    * Função para separar o array da árvore em blocos
    * Função para consolidar os resultados dos processos individuais
* Gerar um .png do grafo
* Gerar um .png da árvore mínima