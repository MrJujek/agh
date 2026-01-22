#1
cat /etc/passwd | awk '{FS=":"} {print $7}' | sort | uniq
#2
cat /etc/protocols | grep -vE ^# | awk '{print $1}' | sort
#3
chmod 4755 ~/test.txt
#4
cp source/* dest/ 2>/dev/null
#5
cat /etc/php/8.3/cli/php.ini | grep -v '^$' | grep -v '^[\[]' | wc -l
#6
awk '{FS=":"} {if (($3 >= 0 && $3 <= 999) || $3 == 1001 ) {print $4, $1}}' /etc/passwd
#7
umask 0177 # 0600
#8
# --x
#9
who | awk '{FS=" "} {if (substr($5, 2, 2) == "10") {print $1}}' | grep -E s[0-9]{6}
#10
grep -E '^[0-9A-Z-a-z\_\+\.]{1,}@[A-Za-z\+\_]{1,}(\.[A-Za-z]{1,}){1,}'