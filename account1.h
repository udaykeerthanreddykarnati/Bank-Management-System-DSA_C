#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <time.h>

typedef struct Transaction {
    char type;            /* 'D' for deposit, 'W' for withdrawal */
    double amount;
    time_t timestamp;
    struct Transaction *next;
} Transaction;

typedef struct Account {
    int accountNumber;
    char name[100];
    double balance;
    Transaction *transactionsHead;
    struct Account *left;
    struct Account *right;
} Account;

/* BST operations */
Account *createAccount(int acctNo, const char *name, double initialBalance);
Account *insertAccount(Account *root, int acctNo, const char *name, double initialBalance);
Account *searchAccount(Account *root, int acctNo);
Account *deleteAccount(Account *root, int acctNo);
int updateAccountName(Account *acc, const char *newName);
int addTransaction(Account *acc, char type, double amount);
void printAccount(const Account *acc);
void inorderPrint(const Account *root);
void freeAllAccounts(Account *root);

/* CSV persistence */
void saveAccountsToCSV(const Account *root, const char *accFile, const char *txnFile);
Account *loadAccountsFromCSV(const char *accFile, const char *txnFile);

#endif /* ACCOUNT_H */
