#!/bin/bash

# Pasta de entrada contendo os ficheiros a processar
input_directory="results"

# Pasta para os ficheiros de saída temporários
output_directory="output"

# Pasta de destino final
final_output_directory="../../relatorio_final/resultados"

# Verifica se a pasta de saída temporária existe; se não, cria a pasta
if [ ! -d "$output_directory" ]; then
    mkdir -p "$output_directory"
fi

# Verifica se a pasta de destino final existe; se não, cria a pasta
if [ ! -d "$final_output_directory" ]; then
    mkdir -p "$final_output_directory"
fi

# Processa todos os ficheiros na pasta de entrada
for input_file in "$input_directory"/*.csv; do
    sorted_file="$output_directory/sorted_$(basename "$input_file")"

    # Ordena o ficheiro de entrada por graph_size e edge_percentage
    sort -t ';' -k2,2n -k3,3n "$input_file" > "$sorted_file"

    # Lê o ficheiro ordenado linha por linha
    while IFS=';' read -r graph_name graph_size edge_percentage elapsed_time num_threads; do
        # Ignora a linha do cabeçalho
        if [ "$graph_name" == "graph_name" ]; then
            continue
        fi

        # Extrai o nome base do ficheiro de entrada
        base_name=$(basename "$input_file" .csv)

        # Cria um ficheiro de saída com o nome do ficheiro de entrada e a percentagem acrescentada
        output_file="$output_directory/${base_name}_${edge_percentage}.csv"

        # Se o ficheiro de saída não existe, adiciona o cabeçalho
        if [ ! -f "$output_file" ]; then
            echo "graph_name;graph_size;edge_percentage;elapsed_time;num_threads" > "$output_file"
        fi

        # Adiciona a linha ao ficheiro de saída
        echo "$graph_name;$graph_size;$edge_percentage;$elapsed_time;$num_threads" >> "$output_file"
    done < "$sorted_file"

    # Remove o ficheiro ordenado temporário
    rm "$sorted_file"
done

# Move os ficheiros ordenados e separados para a pasta de destino final
mv "$output_directory"/* "$final_output_directory"

# Remove a pasta temporária
rmdir "$output_directory"
