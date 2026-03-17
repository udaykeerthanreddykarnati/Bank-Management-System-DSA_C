#include <stdio.h>
#include <stdlib.h>
#include "account1.h"

void showMenu(void) {
    printf("\n=== Bank Account Management ===\n");
    printf("1. Insert new account\n");
    printf("2. Delete account\n");
    printf("3. Search account\n");
    printf("4. Update account name\n");
    printf("5. Add transaction (Deposit/Withdrawal)\n");
    printf("6. Print all accounts\n");
    printf("7. Save & Exit\n");
    printf("Enter your choice: ");
}

int main(void) {
    const char *accountsCSV = "accounts.csv";
    const char *transCSV    = "transactions.csv";

    Account *root = loadAccountsFromCSV(accountsCSV, transCSV);
    if (root) {
        printf("✅ Data loaded successfully from CSV files.\n");
    } else {
        printf("⚠️ No existing data found — starting fresh.\n");
    }

    int choice;
    while (1) {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        if (choice == 1) {
            int acctNo;
            char name[100];
            double initialBalance;
            printf("Enter account number: ");
            scanf("%d", &acctNo);
            printf("Enter name: ");
            scanf(" %99[^\n]", name);
            printf("Enter initial balance: ");
            scanf("%lf", &initialBalance);
            root = insertAccount(root, acctNo, name, initialBalance);
        }
        else if (choice == 2) {
            int acctNo;
            printf("Enter account number to delete: ");
            scanf("%d", &acctNo);
            root = deleteAccount(root, acctNo);
        }
        else if (choice == 3) {
            int acctNo;
            printf("Enter account number to search: ");
            scanf("%d", &acctNo);
            Account *acc = searchAccount(root, acctNo);
            if (acc) {
                printAccount(acc);
            } else {
                printf("Account not found.\n");
            }
        }
        else if (choice == 4) {
            int acctNo;
            char newName[100];
            printf("Enter account number: ");
            scanf("%d", &acctNo);
            Account *acc = searchAccount(root, acctNo);
            if (acc) {
                printf("Enter new name: ");
                scanf(" %99[^\n]", newName);
                if (updateAccountName(acc, newName)) {
                    printf("Name updated successfully.\n");
                } else {
                    printf("Failed to update name.\n");
                }
            } else {
                printf("Account not found.\n");
            }
        }
        else if (choice == 5) {
            int acctNo;
            char type;
            double amount;
            printf("Enter account number: ");
            scanf("%d", &acctNo);
            Account *acc = searchAccount(root, acctNo);
            if (!acc) {
                printf("Account not found.\n");
                continue;
            }
            printf("Enter transaction type (D for deposit, W for withdrawal): ");
            scanf(" %c", &type);
            printf("Enter amount: ");
            scanf("%lf", &amount);
            if (addTransaction(acc, type, amount)) {
                printf("Transaction added successfully.\n");
            } else {
                printf("Failed to add transaction.\n");
            }
        }
        else if (choice == 6) {
            inorderPrint(root);
        }
        else if (choice == 7) {
            saveAccountsToCSV(root, accountsCSV, transCSV);
            printf("💾 Data saved to CSV. Exiting program.\n");
            break;
        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    freeAllAccounts(root);
    return 0;
}
