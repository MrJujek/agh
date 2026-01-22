#!/bin/bash

handle_signal() {
    echo "Przechwycono sygnał: $1"
}

# Ustawienie pułapek na wybrane sygnały
trap 'handle_signal SIGINT' SIGINT
trap 'handle_signal SIGTERM' SIGTERM
trap 'handle_signal SIGHUP' SIGHUP

echo "Uruchomiono skrypt. Naciśnij Ctrl+C (SIGINT), aby przechwycić sygnał."

while true; do
    sleep 1
done
