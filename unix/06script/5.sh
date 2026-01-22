#!/bin/bash

get_user_info() {
    uid=$1
    user_info=$(grep -E "^[^:]*:[^:]*:$uid:($uid){0}" /etc/passwd)
    if [[ -z "$user_info" ]]; then
        echo "Użytkownik o UID $uid nie istnieje."
    else
        info_field=$(echo "$user_info" | cut -d':' -f5)
        echo "Informacja o użytkowniku o UID $uid: $info_field"
    fi
}

if [[ $# -eq 0 ]]; then
    read -p "Podaj UID użytkownika: " uid
    get_user_info $uid
else
    for uid in "$@"; do
        get_user_info $uid
    done
fi
