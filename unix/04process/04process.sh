#1
ps -o %mem,%cpu,tty,pid,cmd -e
#2
ps -eo user,pid,pri,nice,tty
#3
gcc -o prog prog.c
./prog &
ps aux | grep ./prog | grep -v grep
ps -o pid,pmem,pri,nice,cmd -p 2127
sudo renice -n 10 -p 12345
#4 5
ps aux | grep ./prog | grep -v grep
#6
nohup ./prog > prog_output.log 2>&1 &
#7
kill -SIGSTOP 2127
kill -SIGCONT 2127
#8
kill -SIGTERM 2376
kill -SIGKILL 2376
#9
ps aux | grep ' T ' | sort -k5 -nr | head -n 1
#10
ps -eo user,nice | awk '$2 == 0 {print $1}' | sort | uniq -c | sort -nr | head -n 1
#11
ps -eo pid,vsz --sort=-vsz | head -n 4
ps -eo pid,vsz | sort -k2 -nr | head -n 3