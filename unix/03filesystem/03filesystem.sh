#1
ls -R
#2
ls -a
#3
mkdir $HOME/c2 | touch $HOME/c2/text | man ls > $HOME/c2/text
#4
chmod 600 c2/text
chmod +t c2
#5
cp
#6
umask 0007
#7
find ~ -type f -exec du -h {} + | sort -rh | head -n 1
find ~ -maxdepth 2 -type f -exec du -h {} + | sort -rh | head -n 1
#8
find ~ -maxdepth 2 -type f -exec stat --format "%b %n" {} + | sort -nr | head -n 1
ls -lahR $PWD | sort -k5 -rh | head -n 1 | awk '{print $8}'
#9
find /usr/include -type f -name "c*" -exec ls -lh {} +
#10
find ~ -type f -mtime +2 -exec ls -lh {} +
find ~ -maxdepth 1 -type f -mtime +2 -exec ls -lh {} +
#11
find /usr/include -maxdepth 1 -type f -name "m*.h" -size -12k | wc -l
#12
tar -cvzf c2.tar.gz c2/
rm -r c2
tar -xvzf c2.tar.gz
#13
du -sh ~
#14
df -h
#15
lsblk -f
#16
du -sh /usr/* | sort -rh
#17
ln -s $HOME/c2/bin/moj_ls $HOME/c2/text/symbol
ln $HOME/c2/text/ls.txt $HOME/c2/bin/twardy
ls -lRi $HOME/c2

