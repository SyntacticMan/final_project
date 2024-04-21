#!/bin/bash

# Definição das opções padrão do comando
initial_size=6
prefix="graph"
extension=".grf"
percentage=40

# Número de grafos padrão
default_num_graphs=5

# Função para exibir o uso do script
usage() {
    echo "Uso: $0 [-n <num_graphs>]"
    echo "   -n <num_graphs>: Número de grafos a serem gerados (padrão: $default_num_graphs)"
    exit 1
}

# Parsing dos argumentos usando getopts
while getopts ":n:" opt; do
    case ${opt} in
        n )
            num_graphs=$OPTARG
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

# Incremento do tamanho do grafo a cada iteração
size_increment=2

# Diretório para armazenar os grafos
output_directory="graphs"

# Cria o diretório se ele não existir
mkdir -p "$output_directory"

# Inicialize o tamanho com o tamanho inicial
size=$initial_size

# Loop para gerar múltiplos grafos
for ((i=1; i<=num_graphs; i++)); do
    # Construindo o nome do arquivo de saída
    output_file="$output_directory/${prefix}_s${size}_p${percentage}${extension}"

    echo "Geração do Grafo $i:"
    ./bin/gengraph -s $size -f $output_file -p $percentage
    echo "-----------------------------------"

    # Aumentando o tamanho para a próxima iteração
    size=$((size + size_increment))
done
