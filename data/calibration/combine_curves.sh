
for line in $(readline)
do
    echo \




fd=3
while IFS='' read -u $fd -r line || [[ -n "$line" ]]; do
    echo "Text read from file: $line"
done $fd< "$1"


