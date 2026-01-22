#!/bin/bash

script_name=$(basename "$0")

for item in *; do
    if [[ "$item" == "$script_name" ]]; then
        continue
    fi

    if [[ -f "$item" ]]; then
        permissions=$(ls -l "$item" | awk '{print $1}')
        owner_permissions=${permissions:1:3}
        echo "$item - plik regularny (prawa dostępu właściciela: $owner_permissions)"
    elif [[ -d "$item" ]]; then
        echo "$item - katalog"
    fi
done
