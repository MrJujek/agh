#1
set -o noclobber   
set +o noclobber   
unset VARIABLE_NAME
unset -f FUNCTION_NAME
env
env VARIABLE=value COMMAND 
echo $noclobber
echo "Initial content" > testfile
echo "New content" > testfile
echo "Additional content" >> testfile
echo "Forced new content" >| testfile
#2
./wyjscia > std.txt 2> err.txt
./wyjscia > razem.txt 2>&1
#3
echo $HISTSIZE
export HISTSIZE=20000
#4
echo $HISTFILE
#5
export TMOUT=300 # 300 seconds
#6
export IGNOREEOF=5
#7
export PATH=$PATH:.
#8
export PS1='\u@\h \t \w \$ '
export PS2='>_ '
#9
alias
alias ll='ls -al --color=always | less -R'
#10
nano ~/.profile # only for this session
source ~/.profile
nano ~/.bashrc # for all sessions
source ~/.bashrc

echo $- == *i* # interactive shell

nano ~/.bash_logout # before logout
clear