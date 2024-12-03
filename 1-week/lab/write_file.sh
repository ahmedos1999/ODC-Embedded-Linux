#!/bin/bash


echo "Enter the file name:"
read filename

if [[ -f "$filename" ]]; then
  
    while IFS= read -r line; do
        echo "$line"
    done < "$filename"
else
    echo "File not found!"
fi

