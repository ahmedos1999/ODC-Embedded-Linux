#!/bin/bash

echo "Enter a number:"
read num

if ! [[ $num -lt 0 ]]; then
    echo "Please enter a valid non-negative integer."
    exit 1
fi

for ((i=0; i<=num; i++)); do
    echo $i
done

