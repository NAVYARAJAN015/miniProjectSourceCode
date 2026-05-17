#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "credit.dat"
#define MAX_ACCOUNTS 100

// structure
struct clientData
{
    unsigned int acctNum;
    char firstName[30];
    char lastName[30];
    double balance;
};

int transactionCount = 0;

// function prototypes
void initializeFile();
unsigned int menu();

void addAccount(FILE *fPtr);
void displayAccounts(FILE *fPtr);
void searchAccount(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void transferMoney(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void exportTextFile(FILE *fPtr);

// main
int main()
{
    FILE *fPtr;
    int choice;

    initializeFile();

    fPtr = fopen(FILE_NAME, "rb+");

    if (fPtr == NULL)
    {
        printf("File could not be opened.\n");
        return 1;
    }

    while ((choice = menu()) != 8)
    {
        switch (choice)
        {
        case 1:
            addAccount(fPtr);
            break;

        case 2:
            displayAccounts(fPtr);
            break;

        case 3:
            searchAccount(fPtr);
            break;

        case 4:
            depositMoney(fPtr);
            break;

        case 5:
            withdrawMoney(fPtr);
            break;

        case 6:
            transferMoney(fPtr);
            break;

        case 7:
            deleteAccount(fPtr);
            break;

        case 9:
            exportTextFile(fPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(fPtr);

    printf("\nTotal Transactions: %d\n", transactionCount);
    printf("Thank You...\n");

    return 0;
}

// initialize file
void initializeFile()
{
    FILE *fPtr;

    if ((fPtr = fopen(FILE_NAME, "rb")) == NULL)
    {
        struct clientData blank = {0, "", "", 0.0};

        fPtr = fopen(FILE_NAME, "wb");

        for (int i = 0; i < MAX_ACCOUNTS; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, fPtr);
        }

        fclose(fPtr);
    }
}

// menu
unsigned int menu()
{
    int choice;

    printf("\n========== BANK MANAGEMENT ==========\n");
    printf("1. Add Account\n");
    printf("2. Display All Accounts\n");
    printf("3. Search Account\n");
    printf("4. Deposit Money\n");
    printf("5. Withdraw Money\n");
    printf("6. Transfer Money\n");
    printf("7. Delete Account\n");
    printf("8. Exit\n");
    printf("9. Export To Text File\n");

    printf("Enter Choice: ");
    scanf("%d", &choice);

    return choice;
}

// add account
void addAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    int acc;

    printf("Enter Account Number (1-100): ");
    scanf("%d", &acc);

    if (acc < 1 || acc > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (acc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    client.acctNum = acc;

    printf("Enter First Name: ");
    scanf("%s", client.firstName);

    printf("Enter Last Name: ");
    scanf("%s", client.lastName);

    printf("Enter Balance: ");
    scanf("%lf", &client.balance);

    fseek(fPtr,
          (acc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    transactionCount++;

    printf("Account Added Successfully.\n");
}

// display all
void displayAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-10s %-15s %-15s %-10s\n",
           "Account",
           "FirstName",
           "LastName",
           "Balance");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10u %-15s %-15s %-10.2lf",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance);

            // innovative feature
            if (client.balance < 500)
            {
                printf("  LOW BALANCE!");
            }

            printf("\n");
        }
    }
}

// search account
void searchAccount(FILE *fPtr)
{
    struct clientData client;

    int acc;

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    fseek(fPtr,
          (acc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("\nAccount Found\n");
    printf("Account No : %u\n", client.acctNum);
    printf("Name       : %s %s\n",
           client.firstName,
           client.lastName);
    printf("Balance    : %.2lf\n",
           client.balance);
}

// deposit
void depositMoney(FILE *fPtr)
{
    struct clientData client;

    int acc;
    double amount;

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }

    fseek(fPtr,
          (acc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    client.balance += amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    transactionCount++;

    printf("Deposit Successful.\n");
}

// withdraw
void withdrawMoney(FILE *fPtr)
{
    struct clientData client;

    int acc;
    double amount;

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    printf("Enter Withdraw Amount: ");
    scanf("%lf", &amount);

    fseek(fPtr,
          (acc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    if (client.balance < amount)
    {
        printf("Insufficient balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    transactionCount++;

    printf("Withdrawal Successful.\n");
}

// transfer money
void transferMoney(FILE *fPtr)
{
    struct clientData sender, receiver;

    int senderAcc, receiverAcc;
    double amount;

    printf("Enter Sender Account: ");
    scanf("%d", &senderAcc);

    printf("Enter Receiver Account: ");
    scanf("%d", &receiverAcc);

    printf("Enter Amount: ");
    scanf("%lf", &amount);

    // sender
    fseek(fPtr,
          (senderAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&sender,
          sizeof(struct clientData),
          1,
          fPtr);

    // receiver
    fseek(fPtr,
          (receiverAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&receiver,
          sizeof(struct clientData),
          1,
          fPtr);

    if (sender.acctNum == 0 || receiver.acctNum == 0)
    {
        printf("Invalid account.\n");
        return;
    }

    if (sender.balance < amount)
    {
        printf("Insufficient balance.\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    // update sender
    fseek(fPtr,
          (senderAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&sender,
           sizeof(struct clientData),
           1,
           fPtr);

    // update receiver
    fseek(fPtr,
          (receiverAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&receiver,
           sizeof(struct clientData),
           1,
           fPtr);

    transactionCount++;

    printf("Transfer Successful.\n");
}

// delete account
void deleteAccount(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0.0};
    struct clientData client;

    int acc;

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    fseek(fPtr,
          (acc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr,
          (acc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blank,
           sizeof(struct clientData),
           1,
           fPtr);

    transactionCount++;

    printf("Account Deleted Successfully.\n");
}

// export text file
void exportTextFile(FILE *fPtr)
{
    FILE *txtPtr;

    struct clientData client;

    txtPtr = fopen("accounts.txt", "w");

    if (txtPtr == NULL)
    {
        printf("Unable to create text file.\n");
        return;
    }

    rewind(fPtr);

    fprintf(txtPtr,
            "%-10s %-15s %-15s %-10s\n",
            "Account",
            "FirstName",
            "LastName",
            "Balance");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(txtPtr,
                    "%-10u %-15s %-15s %-10.2lf\n",
                    client.acctNum,
                    client.firstName,
                    client.lastName,
                    client.balance);
        }
    }

    fclose(txtPtr);

    printf("accounts.txt created successfully.\n");
}