#!/bin/bash

if (( $# != 3 )); then
    echo "Zła liczba argumentów"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "Plik nie jest regularny"
    exit 1
fi

if [[ ! -d $2 ]]; then
    echo "Podana ścieżka nie jest katalogiem"
    exit 1
fi

if (( $3 <= 0 )); then
    echo "Liczba musi być większa od zera"
    exit 1
fi

file_path=$1
dir_path=$2
num_copies=$3

file_name=$(basename "$file_path")

for (( i=1; i<=num_copies; i++ ))
do
    cp "$file_path" "$dir_path/${file_name}-kopia-$i"
done

echo "Utworzono $num_copies kopii pliku $file_name w katalogu $dir_path"
