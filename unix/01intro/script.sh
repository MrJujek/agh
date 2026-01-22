for a in $(cat "/etc/passwd"); do
    echo $a | cut -f4 -d':'
done