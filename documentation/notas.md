# Notas do projecto


Um grafo não direccionado é constituído por um par não ordenado de arestas e vértices. 
Vértice é um ponto, uma aresta é composta por um par de vértices o que indica que estão ligados.

Foi especificado que os grafos têm de ser gerados aleatoriamente. Isso implica que o número de vértices é escolhido ao acaso e também o número de ligações que cada um tem e a que vértices estão ligados.


## Gerador aleatório de grafos.

    - Tem de ser indicado um número máximo e mínimo de vértices
    - Tem de ser indicado um número máximo de arestas que cada vértice pode ter, sendo que o mínimo é 1
    - Tamanho do grafo é indicado pelo número de vértices indicado
    - Cada grafo é um vetor de pares (vértices, arestas)
    - Para responder ao facto de que é gerado aleatóriamente o melhor é fazer single linked list e alocar tudo na heap.
    - Não pode haver vértices sem pelo menos uma aresta.


## Notas reunião

 array de pointer para guardar metade da matriz
 uma função que lê e escreve na matriz de adjacência e abstrai o facto de que é apenas triangular superior
 na matriz são guardados os pesos, os vértices são dados pela coordenada linha/coluna

 Binários
 - produzir um gerador de grafos que produz as matrizes em binários
 - segundo binário implementa o algoritmo de prim
 - este binário recebe uma percentagem de ligações, cada vez que uma aresta é gerada essa percentagem é avaliada.
 - essa percentagem é distribuida pelos vértices
 - todos os vértices têm pelo menos uma aresta
 - formato do binário -> começa com o tamanho do grafo, depois o conteúdo da matriz triangular superior
 - tem de aceitar um intervalo para pesos, minímo de 0.

- algoritmo recebe número de threads que deve gerar
- uma, corre o simples
- mais do que uma, corre multi-thread
- máximo de threads é o máximo de colunas - nice to have, partir colunas por mais threads que o número de colunas
- tem de produzir um gráfico com o grafo e a árvore mínima resolvida (sdl2?) bem como os resultados do processamento
- testar o máximo de processamento que o portátil consegue aguentar antes da defesa do projecto


