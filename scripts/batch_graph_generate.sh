#!/bin/bash

# Definição das opções padrão do comando
prefix="graph"
extension=".grf"

# Diretório para armazenar os grafos
output_directory="graphs"

# Cria o diretório se ele não existir
mkdir -p "$output_directory"

size=500
# Loop para gerar múltiplos grafos
for ((i=1; i<=3; i++)); do
	percentage=25
	for ((j=1; j<=3; j++)); do
		
	    output_file="$output_directory/${prefix}_s${size}_p${percentage}${extension}"

	    echo "Geração do Grafo $i de 250:"
	    ./bin/gengraph -s $size -f $output_file -p $percentage
	    echo "-----------------------------------"

	    size=$((size + 500))
	    percentage=$((percentage + 25))
    done
done
