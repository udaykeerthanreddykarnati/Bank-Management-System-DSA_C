# Bank Account Management System (C)

A menu-driven **Bank Account Management System implemented in C** using **Binary Search Trees (BST)** and **Linked Lists**.
The system allows users to create accounts, perform deposits/withdrawals, maintain transaction history, and store data persistently using CSV files.

---

# Features

* Create new bank accounts
* Delete existing accounts
* Search accounts by account number
* Update account holder name
* Deposit money
* Withdraw money
* Maintain transaction history
* Print all accounts
* Save accounts and transactions to CSV
* Load saved data on program startup

---

# Data Structures Used

## Binary Search Tree (BST)

Accounts are stored in a **Binary Search Tree** sorted by account number.

Benefits:

* Fast account lookup
* Efficient insert and delete operations

Each node represents an account containing:

```
Account Number
Account Holder Name
Balance
Transaction List
Left / Right child pointers
```

---

## Linked List

Each account maintains a **linked list of transactions**.

Each transaction stores:

```
Transaction Type (Deposit / Withdrawal)
Amount
Timestamp
Pointer to next transaction
```

New transactions are inserted at the **head of the list**.

---

# File Persistence

The program stores data in two CSV files.

## accounts.csv

Stores account information.

```
accountNumber,name,balance
```

Example:

```
1001,Alice,5000.00
1002,Bob,3000.00
```

---

## transactions.csv

Stores all transaction history.

```
accountNumber,type,amount,timestamp
```

Example:

```
1001,D,1000.00,1700000000
1001,W,500.00,1700000300
```

---

# Project Structure

```
bank-account-system
│
├── account1.c
├── account1.h
├── main1.c
├── accounts.csv
├── transactions.csv
├── README.md
└── .gitignore
```

---

# Compilation

Compile using GCC:

```
gcc main1.c account1.c -o bankapp
```

---

# Run the Program

```
./bankapp
```

---

# Menu Options

```
1 Insert new account
2 Delete account
3 Search account
4 Update account name
5 Add transaction (Deposit/Withdrawal)
6 Print all accounts
7 Save & Exit
```

---

# Example Workflow

1. Start program
2. Create accounts
3. Perform deposits or withdrawals
4. View account details
5. Exit using **Save & Exit**

All data will be stored in CSV files and loaded automatically the next time the program runs.

---

# Concepts Demonstrated

* Binary Search Trees
* Linked Lists
* Dynamic memory allocation
* File handling in C
* Modular programming
* Structs and pointers
* Time handling using `time.h`

---

# Author

Uday Keerthan Reddy
