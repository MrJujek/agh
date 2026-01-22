#!/bin/bash

handle_sigint() {
    echo "Zakończono działanie skryptu."
    exit
}

trap 'handle_sigint' SIGINT

while true; do
    echo $(ps aux | wc -l)
    sleep 3
done