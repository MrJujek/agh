#!/bin/bash

if (( $# == 0 )); then
    echo "Brak argumentów"
    exit
elif (( $# != 1 )); then
    echo "Zła liczba argumentów"
    exit
elif [[ ! -f $1 ]]; then
    echo "Plik nie jest regularny"
    exit
fi

echo "Podany argument: $1"
