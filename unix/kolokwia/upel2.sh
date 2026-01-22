#1
kill -SIGKILL 1000
#2
#ctrl+c
#3
export PATH="/mnt/test":$PATH
#4
ps aux | tail -n +2 | sort -nr -k3 | head -n 1 | awk -F" " '{print $2}'
#5
# skrypt nie wylaczyl sie i jego rodzicem jest proces o id 1
#6
bash
#7
#!/bin/bash
memory=$(cat /proc/meminfo | grep "MemAvailable" | awk -F" " '{print $2}')
if [ $memory -ge 1000 ]; then
    echo "OK"
else
    echo "ALARM"
fi
#cat /proc/meminfo | grep "MemAvailable" | awk -F" " '{if ($2 >= 1000) {print "OK"} else {print "ALARM"}}'
#8
#!/bin/bash
if [ ! -f "/etc/passwd" ]; then
    echo "Plik nie istnieje"
    exit 1
fi
if [ $1 ]; then
    login=$1
else
    read -p "Podaj login: " login
fi
if [[ ! $login =~ ^[a-zA-Z0-9]+$ ]]; then
    echo "Niepoprawny login"
    exit 2
fi
line=$(cat /etc/passwd | grep "^$login:" | awk -F":" '{print $5}')
if [[ ! $line ]]; then
    echo "Brak komentarza"
    exit 3
else 
    echo $line
fi
