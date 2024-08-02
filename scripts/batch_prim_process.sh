#!/bin/bash

# script para processar os grafos de teste e gerar os dados para o relatório

# Pasta contendo os arquivos .grf
input_directory="graphs"

# Verifica se a pasta existe
if [ ! -d "$input_directory" ]; then
    echo "Pasta $input_directory não encontrada."
    exit 1
fi

rm ./results/*

# Loop para percorrer todos os arquivos .grf na pasta
for file in $(find "$input_directory" -name "*.grf" -printf "%C@ %p\n" | sort -n | cut -d' ' -f2-); do
    # Verifica se o arquivo é um arquivo .grf
    if [ -f "$file" ]; then
        for t in 1 2 4 6 8; do
        echo "Analisando $file com $t processos:"
            ./bin/prim -f "$file" -t "$t" -r
        echo "-----------------------------------"
        done
    fi
done

