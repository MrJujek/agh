#1
awk -F':' '{print $1, length($1)}' /etc/passwd
#2
find /etc -maxdepth 1 -type f -exec wc -l {} \; 2>/dev/null
#3
who | grep -E ^r | awk '{print $1}'
#4
awk -F: '{if ($7 == "/bin/bash") {print $1, $7}}' /etc/passwd | head -n 3
#5
find ~ -type f -name "*.sh" -exec chmod 764 {} \;