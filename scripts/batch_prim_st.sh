#!/bin/bash

# Pasta contendo os arquivos .grf
input_directory="graphs"

# Verifica se a pasta existe
if [ ! -d "$input_directory" ]; then
    echo "Pasta $input_directory não encontrada."
    exit 1
fi

# Loop para percorrer todos os arquivos .grf na pasta
for file in "$input_directory"/*.grf; do
    # Verifica se o arquivo é um arquivo .grf
    if [ -f "$file" ]; then
        echo "Executando comando para $file:"
        ./bin/prim -f "$file"
        echo "-----------------------------------"
    fi
done
