tar -cf $4 $(find "$1" -type f -name "*.$2" -mtime -$3 -readable)

