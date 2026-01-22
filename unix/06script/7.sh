#!/bin/bash

check_permissions() {
    file=$1

    if [[ -r "$file" ]]; then
        echo "Plik $file: masz prawo do odczytu."
    else
        echo "Plik $file: nie masz prawa do odczytu."
    fi

    if [[ -w "$file" ]]; then
        echo "Plik $file: masz prawo do zapisu."
    else
        echo "Plik $file: nie masz prawa do zapisu."
    fi

    if [[ -x "$file" ]]; then
        echo "Plik $file: masz prawo do wykonywania."
    else
        echo "Plik $file: nie masz prawa do wykonywania."
    fi
}

if [[ $# -eq 0 ]]; then
    read -p "Podaj ścieżkę do pliku: " file
else
    file=$1
fi

if [[ ! -e "$file" ]]; then
    echo "Plik $file nie istnieje."
    exit 1
fi

check_permissions "$file"
