#!/bin/bash


result_log="calculator_results.log"
error_log="calculator_errors.log"

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
            echo "Result: $result"
            echo "$(date) - $operand1 $operator $operand2 = $result" >> "$result_log"
            ;;
        -)
            result=$(echo "$operand1 - $operand2" | bc)
            echo "Result: $result"
            echo "$(date) - $operand1 $operator $operand2 = $result" >> "$result_log"
            ;;
        \*)
            result=$(echo "$operand1 * $operand2" | bc)
            echo "Result: $result"
            echo "$(date) - $operand1 $operator $operand2 = $result" >> "$result_log"
            ;;
        /)
           
            if [ "$operand2" -eq 0 ]; then
                echo "Error: Division by zero!"
                echo "$(date) - Error: Division by zero for $operand1 $operator $operand2" >> "$error_log"
                continue
            else
                result=$(echo "scale=2; $operand1 / $operand2" | bc)
                echo "Result: $result"
                echo "$(date) - $operand1 $operator $operand2 = $result" >> "$result_log"
            fi
            ;;
        *)
            echo "Invalid operator. Please enter one of +, -, *, or /."
            echo "$(date) - Error: Invalid operator entered: $operator" >> "$error_log"
            continue
            ;;
    esac

    echo "Do you want to perform another calculation? (yes/no)"
    read response
    if [[ "$response" != "yes" ]]; then
        echo "Exiting calculator."
        break
    fi
done

