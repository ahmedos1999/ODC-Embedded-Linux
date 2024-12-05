#!/bin/bash

# Define directory for storing client data
BANK_DIR="bank_data"
LOG_FILE="error.log"
mkdir -p "$BANK_DIR"

# Log errors to a file
log_error() {
  echo "$(date '+%Y-%m-%d %H:%M:%S') - ERROR: $1" >> "$LOG_FILE"
}

# Validate phone number format (10-11 digits)
validate_phone() {
  if [[ ! "$1" =~ ^[0-9]{10,11}$ ]]; then
    log_error "Invalid phone number format: $1"
    echo "Invalid phone number. Must be 10-11 digits."
    return 1
  fi
  return 0
}

# Validate balance as numeric
validate_balance() {
  if [[ ! "$1" =~ ^[0-9]+(\.[0-9]{1,2})?$ ]]; then
    log_error "Invalid balance: $1"
    echo "Invalid balance. Must be a numeric value."
    return 1
  fi
  return 0
}

# Function to search for a client by name or phone
search_client() {
  read -p "Search by (name/phone): " criteria
  read -p "Enter value to search: " value
  matches=()
  
  for file in "$BANK_DIR"/*.txt; do
    if grep -iq "$criteria: $value" "$file"; then
      matches+=("$file")
    fi
  done
  
  if [ ${#matches[@]} -eq 0 ]; then
    echo "No matching clients found."
  else
    echo "Matching clients:"
    for match in "${matches[@]}"; do
      echo "----"
      cat "$match"
      echo "----"
    done
  fi
}

# Function to create a new client record
create_client() {
  read -p "Enter client ID: " id
  read -p "Enter client name: " name
  read -p "Enter client phone: " phone
  read -p "Enter client balance: " balance
  
  client_file="$BANK_DIR/$id.txt"
  
  if [ -f "$client_file" ]; then
    echo "Client with this ID already exists."
    log_error "Client creation failed: Duplicate ID $id"
    return
  fi
  
  if ! validate_phone "$phone" || ! validate_balance "$balance"; then
    return
  fi
  
  echo "ID: $id" > "$client_file"
  echo "Name: $name" >> "$client_file"
  echo "Phone: $phone" >> "$client_file"
  echo "Balance: $balance" >> "$client_file"
  echo "Client record created successfully."
}

# Function to update an existing client's data
update_client() {
  read -p "Enter client ID to update: " id
  client_file="$BANK_DIR/$id.txt"
  
  if [ ! -f "$client_file" ]; then
    echo "Client not found."
    log_error "Update failed: Client ID $id not found"
    return
  fi
  
  name=$(grep "Name:" "$client_file" | cut -d: -f2)
  phone=$(grep "Phone:" "$client_file" | cut -d: -f2)
  balance=$(grep "Balance:" "$client_file" | cut -d: -f2)
  
  echo "Current data for client ID $id:"
  echo "Name: $name"
  echo "Phone: $phone"
  echo "Balance: $balance"
  
  read -p "Enter new name (leave blank to keep current): " new_name
  read -p "Enter new phone (leave blank to keep current): " new_phone
  read -p "Enter new balance (leave blank to keep current): " new_balance
  
  new_name=${new_name:-$name}
  new_phone=${new_phone:-$phone}
  new_balance=${new_balance:-$balance}
  
  if ! validate_phone "$new_phone" || ! validate_balance "$new_balance"; then
    return
  fi
  
  echo "ID: $id" > "$client_file"
  echo "Name: $new_name" >> "$client_file"
  echo "Phone: $new_phone" >> "$client_file"
  echo "Balance: $new_balance" >> "$client_file"
  echo "Client data updated successfully."
}

# Function to delete a client record
delete_client() {
  read -p "Enter client ID to delete: " id
  client_file="$BANK_DIR/$id.txt"
  
  if [ ! -f "$client_file" ]; then
    echo "Client not found."
    log_error "Delete failed: Client ID $id not found"
    return
  fi
  
  rm "$client_file"
  echo "Client record deleted successfully."
}

# Function to view a client's data
view_client() {
  read -p "Enter client ID to view: " id
  client_file="$BANK_DIR/$id.txt"
  
  if [ ! -f "$client_file" ]; then
    echo "Client not found."
    log_error "View failed: Client ID $id not found"
    return
  fi
  
  cat "$client_file"
}

# Function to display the menu
show_menu() {
  clear
  echo -e "\n**********************"
  echo "Bank Client Management"
  echo "**********************"
  echo "1. Create new client"
  echo "2. Update existing client"
  echo "3. Delete client"
  echo "4. View client data"
  echo "5. Search client"
  echo "6. Exit"
  
  read -p "Choose an option (1-6): " option
  
  case $option in
    1) create_client ;;
    2) update_client ;;
    3) delete_client ;;
    4) view_client ;;
    5) search_client ;;
    6) echo "Exiting..."; sleep 3; exit 0 ;;
    *) echo "Invalid option. Please try again." ;;
  esac
  sleep 3
}

