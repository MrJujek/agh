#1
#!/bin/bash
for i in {1..5}; do
    echo $(ps -aux | tail -n +2 | wc -l)
    sleep 3
done
#2
kill -SIGUSR1 1000
#3
export PATH=$PATH:$(pwd)
#4
#!/bin/bash
if [ ! $1 ]; then
    read -p "Podaj UID: " uid
    echo $(awk -F: '{if ($3 == '$uid') {print $5}}' /etc/passwd)
else
    for uid in $@; do
        echo $(awk -F: '{if ($3 == '$uid') {print $5}}' /etc/passwd)
    done
fi
exit 1
#5
ps -eo pid,%mem -e | awk -F" " '{if ($2 != "0.0") {print $1"\t"$2}}'