#1
find $HOME -type f | xargs ls -lath 2>/dev/null | tail -n 1\
#2 
find /usr/include -type d | wc -l
#3
find /usr/include -maxdepth 1 -type f -name "m?????.h" | wc -l
#4
find /usr/include -type f -size +12000c | wc -l
#5
find /usr/bin -type f -size -1048576c | wc -l