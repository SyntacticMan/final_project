#!/bin/bash

# Definição das opções padrão do comando
prefix="graph"
extension=".grf"
percentage=45
size=10

# Número de grafos padrão
default_num_graphs=5
default_size= 10

# Função para exibir o uso do script
usage() {
    echo "Uso: $0 [-n <num_graphs>] [-s <size_increment>]"
    echo "   -n <num_graphs>: Número de grafos a serem gerados (padrão: $default_num_graphs)"
    echo "   -s <size_increment>: Tamanho dos grafos a serem gerados (padrão: $default_size)"
    exit 1
}

# Parsing dos argumentos usando getopts
while getopts ":n:s:" opt; do
    case ${opt} in
        n )
            num_graphs=$OPTARG
            ;;
        s)
            size_increment=$OPTARG
            ;;
        \? )
            echo "Opção inválida: -$OPTARG" 1>&2
            usage
            ;;
        : )
            echo "Opção -$OPTARG requer um argumento." 1>&2
            usage
            ;;
    esac
done

# Definindo o número de grafos como o padrão se não fornecido
num_graphs=${num_graphs:-$default_num_graphs}
size_increment=${size_increment:-$default_size}

# Diretório para armazenar os grafos
output_directory="graphs"

# Cria o diretório se ele não existir
mkdir -p "$output_directory"

# Loop para gerar múltiplos grafos
for ((i=1; i<=num_graphs; i++)); do

    output_file="$output_directory/${prefix}_s${size}_p${percentage}${extension}"

    echo "Geração do Grafo $i de $num_graphs:"
    ./bin/gengraph -s $size -f $output_file -p $percentage
    echo "-----------------------------------"

    size=$((size + size_increment))
done
