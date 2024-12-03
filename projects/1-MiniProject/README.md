# Bank Client Management System

A simple Bash script-based project to manage bank client data. The system allows storing, updating, deleting, searching, and viewing client data in individual files. This project is modular, with a separate entry point and library for better organization and reusability.

---

## **Features**
- Create a new client with details: `ID`, `Name`, `Phone`, and `Balance`.
- Update an existing client's data.
- Delete a client's record.
- View a client's data.
- Search for clients by name or phone number.
- Modular design: functions and logic separated into a reusable library.
- Validates phone numbers and balances to ensure data correctness.
- Logs errors to a `error.log` file for debugging purposes.

---

### Menu Options:
1. **Create New Client**  
   Enter client details, and the system will create a file for the client under the `bank_data` directory.  
   - **Validations**:  
     - Phone number must be between 10 and 15 digits.
     - Balance must be numeric (with up to 2 decimal places).  
   - Example:
     ```bash
     Enter client ID: 101
     Enter client name: John Doe
     Enter client phone: 1234567890
     Enter client balance: 500.75
     ```

2. **Update Existing Client**  
   Update any detail of an existing client by entering the client’s ID. The system will show the current data and allow modifications. Leave a field blank to keep the existing value.

3. **Delete Client**  
   Remove a client's file from the system by providing their ID.

4. **View Client Data**  
   Display the information of a client by providing their ID.

5. **Search Client**  
   Search for clients by their name or phone number. The system will display all matches found.

6. **Exit**  
   Safely exit the system.

---

## **Error Logging**
Errors (e.g., invalid input or missing files) are recorded in `error.log` with timestamps for debugging purposes.  
Example log entry:
```
2024-12-04 15:30:12 - ERROR: Invalid phone number format: 12345
```

---

## **Code Organization**

- **`entry_point.sh`**  
  Acts as the entry point to the program. Sources the `bank_library.sh` and runs the main loop to display the menu.

- **`bank_library.sh`**  
  Contains all reusable functions:
  - `create_client()`
  - `update_client()`
  - `delete_client()`
  - `view_client()`
  - `search_client()`
  - `show_menu()`
  - `log_error()`
  - `validate_phone()`
  - `validate_balance()`
