#!/bin/bash

# Definição das opções padrão do comando
prefix="graph"
extension=".grf"

# valores iniciais para percentagem de arestas e número de vértices
percentage=25
size=500

# Definindo o número de grafos como o padrão se não fornecido
num_graphs=150
size_increment=500
percentage_increment=25

# Diretório para armazenar os grafos
output_directory="graphs"

# Cria o diretório se ele não existir
mkdir -p "$output_directory"

# Loop para gerar múltiplos grafos
for ((i=1; i<=num_graphs; i++)); do
    for ((j=1; j<=3; j++)); do
        output_file="$output_directory/${prefix}_s${size}_p${percentage}${extension}"

        echo "Geração do Grafo $i de $num_graphs:"
        ./bin/gengraph -s $size -f $output_file -p $percentage
        echo "-----------------------------------"
        percentage=$((percentage + percentage_increment));
    done
    size=$((size + size_increment))
    percentage=25
done
