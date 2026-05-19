#ifndef TRANS_H
#define TRANS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Required for strcmp

// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};                        // end structure clientData

// function prototypes
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void viewAccount(FILE *fPtr);
void listAccounts(FILE *fPtr);
void transferFunds(FILE *fPtr);
void searchByName(FILE *fPtr);
unsigned int enterChoice(void);

#endif // TRANS_H
