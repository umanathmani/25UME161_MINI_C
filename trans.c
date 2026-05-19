// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
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

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void viewAccount(FILE *fPtr);
void listAccounts(FILE *fPtr);
void transferFunds(FILE *fPtr);
void searchByName(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        // Try creating the file if it doesn't exist
        if ((cfPtr = fopen("credit.dat", "wb+")) == NULL)
        {
            printf("%s: File could not be opened or created.\n", argv[0]);
            exit(-1);
        }
        else 
        {
            // Initialize the newly created file with 100 blank records
            struct clientData blankClient = {0, "", "", 0.0};
            for (unsigned int i = 1; i <= 100; ++i) 
            {
                fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
            }
            rewind(cfPtr); // Reset pointer to beginning of file
        }
    }

    // enable user to specify action
    while ((choice = enterChoice()) != 9)
    {
        switch (choice)
        {
        // create text file from record file
        case 1:
            textFile(cfPtr);
            break;
        // update record
        case 2:
            updateRecord(cfPtr);
            break;
        // create record
        case 3:
            newRecord(cfPtr);
            break;
        // delete existing record
        case 4:
            deleteRecord(cfPtr);
            break;
        // view an account
        case 5:
            viewAccount(cfPtr);
            break;
        // list all accounts
        case 6:
            listAccounts(cfPtr);
            break;
        // transfer funds
        case 7:
            transferFunds(cfPtr);
            break;
        // search by last name
        case 8:
            searchByName(cfPtr);
            break;
        // display if user does not select valid choice
        default:
            puts("Incorrect choice");
            break;
        } // end switch

        // Pause and clear the screen for a cleaner user experience
        if (choice != 9)
        {
            printf("\n");
            system("pause");
            system("cls");
        }

    }     // end while

    fclose(cfPtr); // fclose closes the file
    return 0;      // indicate successful termination
} // end main

// create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr; // accounts.txt file pointer
    int result;     // used to test whether fread read any bytes
    // create clientData with default information
    struct clientData client = {0, "", "", 0.0};

    // fopen opens the file; exits if file cannot be opened
    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    } // end if
    else
    {
        rewind(readPtr); // sets pointer to beginning of file
        fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

        // copy all records from random-access file into text file
        while (!feof(readPtr))
        {
            result = fread(&client, sizeof(struct clientData), 1, readPtr);

            // write single record to text file
            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr, "%-6u%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName,
                        client.balance);
            } // end if
        }     // end while

        fclose(writePtr); // fclose closes the file
    }                     // end else
} // end function textFile

// update balance in record
void updateRecord(FILE *fPtr)
{
    unsigned int account; // account number
    double transaction;   // transaction amount
    // create clientData with no information
    struct clientData client = {0, "", "", 0.0};

    // obtain number of account to update
    printf("%s", "Enter account to update ( 1 - 100 ): ");
    scanf("%u", &account);

    if (account >= 1 && account <= 100)
    {
        // move file pointer to correct record in file
        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        // read record from file
        fread(&client, sizeof(struct clientData), 1, fPtr);
        // display error if account does not exist
        if (client.acctNum == 0)
        {
            printf("Account #%u has no information.\n", account);
        }
        else
        { // update record
            printf("%-6u%-16s%-11s%10.2f\n\n", client.acctNum, client.lastName, client.firstName, client.balance);

            // request transaction amount from user
            printf("%s", "Enter charge ( + ) or payment ( - ): ");
            scanf("%lf", &transaction);
            client.balance += transaction; // update record balance

            printf("%-6u%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);

            // move file pointer to correct record in file
            // move back by 1 record length
            fseek(fPtr, - (long) sizeof(struct clientData), SEEK_CUR);
            // write updated record over old record in file
            fwrite(&client, sizeof(struct clientData), 1, fPtr);
        } // end else
    }
    else
    {
        printf("Invalid account number.\n");
    }
} // end function updateRecord

// delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;                       // stores record read from file
    struct clientData blankClient = {0, "", "", 0}; // blank client
    unsigned int accountNum;                        // account number

    // obtain number of account to delete
    printf("%s", "Enter account number to delete ( 1 - 100 ): ");
    scanf("%u", &accountNum);

    if (accountNum >= 1 && accountNum <= 100)
    {
        // move file pointer to correct record in file
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        // read record from file
        fread(&client, sizeof(struct clientData), 1, fPtr);
        // display error if record does not exist
        if (client.acctNum == 0)
        {
            printf("Account %u does not exist.\n", accountNum);
        } // end if
        else
        { // delete record
            // move file pointer to correct record in file
            fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
            // replace existing record with blank record
            fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        } // end else
    }
    else
    {
        printf("Invalid account number.\n");
    }
} // end function deleteRecord

// create and insert record
void newRecord(FILE *fPtr)
{
    // create clientData with default information
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum; // account number

    // obtain number of account to create
    printf("%s", "Enter new account number ( 1 - 100 ): ");
    scanf("%u", &accountNum);

    if (accountNum >= 1 && accountNum <= 100)
    {
        // move file pointer to correct record in file
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        // read record from file
        fread(&client, sizeof(struct clientData), 1, fPtr);
        // display error if account already exists
        if (client.acctNum != 0)
        {
            printf("Account #%u already contains information.\n", client.acctNum);
        } // end if
        else
        { // create record
            // user enters last name, first name and balance
            printf("%s", "Enter lastname, firstname, balance\n? ");
            scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);

            client.acctNum = accountNum;
            // move file pointer to correct record in file
            fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
            // insert record in file
            fwrite(&client, sizeof(struct clientData), 1, fPtr);
        } // end else
    }
    else
    {
        printf("Invalid account number.\n");
    }
} // end function newRecord

// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice; // variable to store user's choice
    int status;
    // display available options
    printf("%s", "\nEnter your choice\n"
                 "1 - store a formatted text file of accounts called\n"
                 "    \"accounts.txt\" for printing\n"
                 "2 - update an account\n"
                 "3 - add a new account\n"
                 "4 - delete an account\n"
                 "5 - view specific account details\n"
                 "6 - list all active accounts\n"
                 "7 - transfer funds between accounts\n"
                 "8 - search accounts by last name\n"
                 "9 - end program\n? ");

    status = scanf("%u", &menuChoice); // receive choice from user
    
    // Clear input buffer if the user enters a non-integer (prevents infinite loop)
    while (status != 1)
    {
        while (getchar() != '\n'); // discard remaining characters in the buffer
        printf("Invalid input. Please enter a number: ");
        status = scanf("%u", &menuChoice);
    }

    return menuChoice;
} // end function enterChoice

// view specific account details
void viewAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("%s", "Enter account number to view ( 1 - 100 ): ");
    scanf("%u", &account);

    if (account >= 1 && account <= 100)
    {
        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fread(&client, sizeof(struct clientData), 1, fPtr);

        if (client.acctNum == 0)
        {
            printf("Account #%u has no information.\n", account);
        }
        else
        {
            printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
            printf("%-6u%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
        }
    }
    else
    {
        printf("Invalid account number.\n");
    }
} // end function viewAccount

// list all active accounts to the screen
void listAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    int result;

    printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
    rewind(fPtr); // start from beginning of file

    while (!feof(fPtr))
    {
        result = fread(&client, sizeof(struct clientData), 1, fPtr);

        if (result != 0 && client.acctNum != 0)
        {
            printf("%-6u%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
        }
    }
} // end function listAccounts

// transfer funds between two accounts
void transferFunds(FILE *fPtr)
{
    struct clientData srcClient = {0, "", "", 0.0};
    struct clientData destClient = {0, "", "", 0.0};
    unsigned int srcAccount, destAccount;
    double amount;

    printf("%s", "Enter sender account number ( 1 - 100 ): ");
    scanf("%u", &srcAccount);

    if (srcAccount < 1 || srcAccount > 100)
    {
        printf("Invalid sender account number.\n");
        return;
    }

    fseek(fPtr, (srcAccount - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&srcClient, sizeof(struct clientData), 1, fPtr);

    if (srcClient.acctNum == 0)
    {
        printf("Sender account #%u has no information.\n", srcAccount);
        return;
    }

    printf("%s", "Enter receiver account number ( 1 - 100 ): ");
    scanf("%u", &destAccount);

    if (destAccount < 1 || destAccount > 100)
    {
        printf("Invalid receiver account number.\n");
        return;
    }

    fseek(fPtr, (destAccount - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&destClient, sizeof(struct clientData), 1, fPtr);

    if (destClient.acctNum == 0)
    {
        printf("Receiver account #%u has no information.\n", destAccount);
        return;
    }

    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    if (amount <= 0 || amount > srcClient.balance)
    {
        printf("Invalid or insufficient funds for transfer.\n");
        return;
    }

    // Process the transfer
    srcClient.balance -= amount;
    destClient.balance += amount;

    // Save sender record
    fseek(fPtr, (srcAccount - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&srcClient, sizeof(struct clientData), 1, fPtr);

    // Save receiver record
    fseek(fPtr, (destAccount - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&destClient, sizeof(struct clientData), 1, fPtr);

    printf("Transfer successful! Sender New Balance: %.2f | Receiver New Balance: %.2f\n", 
           srcClient.balance, destClient.balance);
} // end function transferFunds

// search for accounts by last name
void searchByName(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    char searchName[15];
    int found = 0;
    int result;

    printf("Enter last name to search for: ");
    scanf("%14s", searchName);

    printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
    rewind(fPtr); // start from beginning of file

    while (!feof(fPtr))
    {
        result = fread(&client, sizeof(struct clientData), 1, fPtr);

        if (result != 0 && client.acctNum != 0)
        {
            // Using strcmp to find exact matches
            if (strcmp(client.lastName, searchName) == 0)
            {
                printf("%-6u%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
                found = 1;
            }
        }
    }

    if (!found)
    {
        printf("No accounts found with last name: %s\n", searchName);
    }
} // end function searchByName