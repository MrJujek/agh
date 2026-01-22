#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Użycie: $0 <plik_zrodlowy> <rozmiar_czesci_w_bajtach> <katalog_docelowy>"
    exit 1
fi

plik_zrodlowy=$1
rozmiar_czesci=$2
katalog_docelowy=$3

if [ ! -f "$plik_zrodlowy" ]; then
    echo "Błąd: Plik źródłowy nie istnieje."
    exit 1
fi

if [ ! -d "$katalog_docelowy" ]; then
    mkdir -p "$katalog_docelowy"
fi

rozmiar_pliku=$(stat --format=%s "$plik_zrodlowy")
liczba_kawalkow=$(( (rozmiar_pliku + rozmiar_czesci - 1) / rozmiar_czesci ))

for ((pass=1; pass<=liczba_kawalkow; pass++)); do
    dd if="$plik_zrodlowy" of="$katalog_docelowy/$(basename "$plik_zrodlowy").part$pass" bs="$rozmiar_czesci" skip=$((pass - 1)) count=1 conv=noerror 2>/dev/null
done

echo "Podział pliku zakończony. Kawałki zapisane w: $katalog_docelowy"
