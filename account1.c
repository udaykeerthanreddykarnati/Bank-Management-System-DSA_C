#include "account1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Helper to create a new account node */
Account *createAccount(int acctNo, const char *name, double initialBalance) {
    Account *acc = (Account *)malloc(sizeof(Account));
    if (!acc) {
        perror("malloc");
        return NULL;
    }
    acc->accountNumber = acctNo;
    strncpy(acc->name, name, sizeof(acc->name) - 1);
    acc->name[sizeof(acc->name) - 1] = '\0';
    acc->balance = initialBalance;
    acc->transactionsHead = NULL;
    acc->left = acc->right = NULL;
    return acc;
}

/* Insert into BST */
Account *insertAccount(Account *root, int acctNo, const char *name, double initialBalance) {
    if (root == NULL) {
        return createAccount(acctNo, name, initialBalance);
    }
    if (acctNo < root->accountNumber) {
        root->left = insertAccount(root->left, acctNo, name, initialBalance);
    } else if (acctNo > root->accountNumber) {
        root->right = insertAccount(root->right, acctNo, name, initialBalance);
    } else {
        /* account exists — update name & balance, optionally ignore */
        fprintf(stderr, "Account number %d already exists. Skipping insert.\n", acctNo);
    }
    return root;
}

/* Search BST */
Account *searchAccount(Account *root, int acctNo) {
    if (root == NULL) return NULL;
    if (acctNo == root->accountNumber) return root;
    if (acctNo < root->accountNumber) return searchAccount(root->left, acctNo);
    return searchAccount(root->right, acctNo);
}

/* Find min in BST (helper for delete) */
static Account *findMin(Account *root) {
    while (root && root->left) root = root->left;
    return root;
}

/* Delete from BST */
Account *deleteAccount(Account *root, int acctNo) {
    if (root == NULL) return NULL;
    if (acctNo < root->accountNumber) {
        root->left = deleteAccount(root->left, acctNo);
    } else if (acctNo > root->accountNumber) {
        root->right = deleteAccount(root->right, acctNo);
    } else {
        /* found node */
        if (root->left == NULL) {
            Account *r = root->right;
            /* free transactions */
            freeAllAccounts(root->transactionsHead); /* reuse free function? No, we’ll write a helper */
            free(root);
            return r;
        } else if (root->right == NULL) {
            Account *l = root->left;
            freeAllAccounts(root->transactionsHead);
            free(root);
            return l;
        } else {
            Account *minNode = findMin(root->right);
            root->accountNumber = minNode->accountNumber;
            strncpy(root->name, minNode->name, sizeof(root->name)-1);
            root->balance = minNode->balance;
            /* free old transactions, then copy new? For simplicity: ignore copying transactions here */
            root->right = deleteAccount(root->right, minNode->accountNumber);
        }
    }
    return root;
}

/* Update name */
int updateAccountName(Account *acc, const char *newName) {
    if (!acc) return 0;
    strncpy(acc->name, newName, sizeof(acc->name)-1);
    acc->name[sizeof(acc->name)-1] = '\0';
    return 1;
}

/* Add transaction and adjust balance */
int addTransaction(Account *acc, char type, double amount) {
    if (!acc) return 0;
    if (type != 'D' && type != 'W') {
        fprintf(stderr, "Invalid transaction type: %c\n", type);
        return 0;
    }
    if (type == 'W' && amount > acc->balance) {
        fprintf(stderr, "Withdrawal amount %.2f exceeds current balance %.2f\n", amount, acc->balance);
        return 0;
    }

    Transaction *t = (Transaction *)malloc(sizeof(Transaction));
    if (!t) {
        perror("malloc");
        return 0;
    }
    t->type = type;
    t->amount = amount;
    t->timestamp = time(NULL);
    t->next = acc->transactionsHead;
    acc->transactionsHead = t;

    if (type == 'D') {
        acc->balance += amount;
    } else {
        acc->balance -= amount;
    }
    return 1;
}

/* Print single account details and its transactions */
void printAccount(const Account *acc) {
    if (!acc) return;
    printf("Account Number: %d\n", acc->accountNumber);
    printf("Name: %s\n", acc->name);
    printf("Balance: %.2f\n", acc->balance);

    printf("Transactions:\n");
    Transaction *t = acc->transactionsHead;
    while (t) {
        char buf[26];
        struct tm *tm_info = localtime(&t->timestamp);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("  [%s] %c %.2f\n", buf, t->type, t->amount);
        t = t->next;
    }
    printf("------------------------\n");
}

/* In‐order print all accounts */
void inorderPrint(const Account *root) {
    if (!root) return;
    inorderPrint(root->left);
    printAccount(root);
    inorderPrint(root->right);
}

/* Free transactions of a single account */
static void freeTransactions(Transaction *t) {
    while (t) {
        Transaction *next = t->next;
        free(t);
        t = next;
    }
}

/* Free entire BST */
void freeAllAccounts(Account *root) {
    if (!root) return;
    freeAllAccounts(root->left);
    freeAllAccounts(root->right);
    freeTransactions(root->transactionsHead);
    free(root);
}

/* ===== CSV Persistence ===== */

/* Helper: recursively save accounts and their transactions */
static void saveAccountsRec(const Account *root, FILE *accFile, FILE *txnFile) {
    if (!root) return;

    saveAccountsRec(root->left, accFile, txnFile);

    /* Write account line: accountNumber,name,balance */
    fprintf(accFile, "%d,%s,%.2f\n", root->accountNumber, root->name, root->balance);

    /* Write each transaction: accountNumber,type,amount,timestamp */
    for (Transaction *t = root->transactionsHead; t; t = t->next) {
        fprintf(txnFile, "%d,%c,%.2f,%ld\n",
                root->accountNumber,
                t->type,
                t->amount,
                (long)t->timestamp);
    }

    saveAccountsRec(root->right, accFile, txnFile);
}

/* Save all data */
void saveAccountsToCSV(const Account *root, const char *accFileName, const char *txnFileName) {
    FILE *fa = fopen(accFileName, "w");
    if (!fa) {
        perror("fopen accounts CSV");
        return;
    }
    FILE *ft = fopen(txnFileName, "w");
    if (!ft) {
        perror("fopen transactions CSV");
        fclose(fa);
        return;
    }

    saveAccountsRec(root, fa, ft);

    fclose(fa);
    fclose(ft);
}

/* Load accounts and transactions from CSV */
Account *loadAccountsFromCSV(const char *accFileName, const char *txnFileName) {
    FILE *fa = fopen(accFileName, "r");
    if (!fa) {
        /* No file to load => start fresh */
        return NULL;
    }

    Account *root = NULL;
    int acctNo;
    char name[100];
    double balance;

    /* Read accounts */
    while (fscanf(fa, "%d,%99[^,],%lf\n", &acctNo, name, &balance) == 3) {
        root = insertAccount(root, acctNo, name, balance);
    }
    fclose(fa);

    /* Read transactions */
    FILE *ft = fopen(txnFileName, "r");
    if (ft) {
        int tAcctNo;
        char tType;
        double tAmount;
        long tTimestamp;
        while (fscanf(ft, "%d,%c,%lf,%ld\n", &tAcctNo, &tType, &tAmount, &tTimestamp) == 4) {
            Account *acc = searchAccount(root, tAcctNo);
            if (acc) {
                Transaction *t = (Transaction *)malloc(sizeof(Transaction));
                if (!t) {
                    perror("malloc transaction");
                    continue;
                }
                t->type = tType;
                t->amount = tAmount;
                t->timestamp = (time_t)tTimestamp;
                t->next = acc->transactionsHead;
                acc->transactionsHead = t;
            }
        }
        fclose(ft);
    }

    return root;
}
