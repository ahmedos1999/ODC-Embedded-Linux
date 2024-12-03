#!/bin/bash


while true; do

    echo "Enter the first operand:"
    read operand1

    echo "Enter the operator (+, -, *, /):"
    read operator

    echo "Enter the second operand:"
    read operand2

    case "$operator" in
        +)
            result=$(echo "$operand1 + $operand2" | bc)
            ;;
        -)
            result=$(echo "$operand1 - $operand2" | bc)
            ;;
        \*)
            result=$(echo "$operand1 * $operand2" | bc)
            ;;
        /)
            result=$(echo "scale=2; $operand1 / $operand2" | bc)
            ;;
        *)
            echo "Invalid operator. Please enter one of +, -, *, or /."
            continue
            ;;
    esac

    echo "Result: $result"

    echo "Do you want to perform another calculation? (yes/no)"
    read response
    if [[ "$response" != "yes" ]]; then
        echo "Exiting calculator."
        break
    fi
done
