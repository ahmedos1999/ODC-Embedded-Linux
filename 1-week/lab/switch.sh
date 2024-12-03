#!/bin/bash

# Prompt the user for two inputs
echo "Enter the first number:"
read num1

echo "Enter the second number:"
read num2

# Compare the numbers
if [ "$num1" -eq "$num2" ]; then
    echo "The numbers are equal."
elif [ "$num1" -gt "$num2" ]; then
    echo "The first number ($num1) is greater than the second number ($num2)."
else
    echo "The first number ($num1) is smaller than the second number ($num2)."
fi

