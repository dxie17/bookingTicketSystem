#define _CRT_SECURE_NO_WARNINGS
#define ACCOUNT_SIZE 15
#define TICKET_SIZE 17

#include <stdio.h>
#include <string.h>
#include "commonHelpers.h"
#include "account.h"
#include "ticket.h"
#include "accountTicketingUI.h"

// Receives a modifiable Account pointer argument

void getAccount(Account* account) {
    int i, account_MAX = 0, len = 0;
    for (i = 0; i < ACCOUNT_SIZE; i++) {
        if (account[i].account_num > account_MAX)
            account_MAX = account[i].account_num;
    }
    for (len = 0; len < ACCOUNT_SIZE; len++) {
        if (account[len].account_num == 0)
            break;
    }
    account[len].account_num = account_MAX + 1;
    printf("New Account Data (Account#:%d)\n", account[len].account_num);
    printf("----------------------------------------\n");
    printf("Enter the account type (A=Agent | C=Customer): ");
    account[len].customer = getCharOption("AC");
    printf("\n");
    return;
}

// Receives a modifiable UserLogin pointer argument.
// Does not return anything but does return data for a UserLogin type
void getUserLogin(UserLogin* login) {
    int max_user = 10, max_name = 30, len_password = 8;
    printf("User Login Data Input\n");
    printf("----------------------------------------\n");
    printf("Enter user login (%d chars max): ", max_user);
    getCString_userlogin(login->user, 1, max_user);
    printf("Enter the display name (%d chars max): ", max_name);
    getCString(login->name, 1, max_name);
    printf("Enter the password (must be %d chars in length): ", len_password);
    getCString_password(login->password, len_password, len_password);
    printf("\n");
    return;
}

// Receives a modifiable Demographic pointer argument.
// Does not return anything but does return data for a Demographic type.
void getDemographic(Demographic* demographic) {
    int max_country = 30, min_age = 18, max_age = 110, min_year, max_year;
    printf("Demographic Data Input\n");
    printf("----------------------------------------\n");
    min_year = currentYear() - 110;
    max_year = currentYear() - 18;
    printf("Enter birth year (current age must be between %d and %d): ",
        min_age, max_age);
    demographic->year = getIntFromRange(min_year, max_year);
    printf("Enter the household Income: $");
    demographic->income = getPositiveDouble();
    printf("Enter the country (%d chars max.): ", max_country);
    getCString_country(demographic->country, 1, max_country);
    printf("\n");
    return;
}

// Display's the update menu options for the account until the user wishes to exit.
void updateAccount(Account* account) {
    int choice = -1, num_quit = 1;
    while (num_quit != 0) {
        switch (choice) {
        case -1:
            printf("Account: %d - Update Options\n", account->account_num);
            printf("----------------------------------------\n");
            printf("1) Update account type (current value: %c)\n", account->customer);
            printf("2) Login\n");
            printf("3) Demographics\n");
            printf("0) Done\n");
            printf("Selection: ");
            choice = getIntFromRange(0, 3);
            printf("\n");
            break;
        case 0:
            num_quit = 0;
            break;
        case 1:
            printf("Enter the account type (A=Agent | C=Customer): ");
            account->customer = getCharOption("AC");
            choice = -1;
            num_quit = 1;
            break;
        case 2:
            updateUserLogin(&account->login);
            choice = -1;
            num_quit = 1;
            break;
        case 3:
            updateDemographic(&account->demographic);
            choice = -1;
            num_quit = 1;
            break;
        }
    }
}

// Display's the update menu options for the user login until the user wishes to exit.
void updateUserLogin(UserLogin* login) {
    int choice = -1, num_quit = 1, MAX_name = 30, len_password = 8;
    while (num_quit != 0) {
        switch (choice) {
        case -1:
            printf("User Login: %s - Update Options\n", login->user);
            printf("----------------------------------------\n");
            printf("1) Display name (current value: %s)\n", login->name);
            printf("2) Password\n");
            printf("0) Done\n");
            printf("Selection: ");
            choice = getIntFromRange(0, 2);
            num_quit = 1;
            break;
        case 0:
            printf("\n");
            num_quit = 0;
            break;
        case 1:
            printf("\nEnter the display name (%d chars max): ", MAX_name);
            getCString(login->name, 1, MAX_name);
            printf("\n");
            choice = -1;
            num_quit = 1;
            break;
        case 2:
            printf("\nEnter the password (must be %d chars in length): ",
                len_password);
            getCString_password(login->password, len_password, len_password);
            printf("\n");
            choice = -1;
            num_quit = 1;
            break;
        }
    }
}

// Display's a menu with options to modifying specific members of a Demographic.
void updateDemographic(Demographic* demographic) {
    int choice = -1, num_quit = 1, MAX_country = 30;
    while (num_quit != 0) {
        switch (choice) {
        case -1:
            printf("Demographic Update Options\n");
            printf("----------------------------------------\n");
            printf("1) Household Income (current value: $%.2lf)\n", demographic->income);
            printf("2) Country (current value: %s)\n", demographic->country);
            printf("0) Done\n");
            printf("Selection: ");
            choice = getIntFromRange(0, 2);
            num_quit = 1;
            break;
        case 0:
            num_quit = 0;
            printf("\n");
            break;
        case 1:
            printf("\nEnter the household Income: $");
            demographic->income = getPositiveDouble();
            printf("\n");
            choice = -1;
            num_quit = 1;
            break;
        case 2:
            printf("\nEnter the country (%d chars max.): ", MAX_country);
            getCString_country(demographic->country, 1, 30);
            printf("\n");
            choice = -1;
            num_quit = 1;
            break;
        }
    }
}

// Load all accounts' information.
int loadAccounts(Account* account, const int accountsize) {
    int num_accounts = 0, i = 0;
    const char filename[] = "accounts.txt";
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    if (fp != NULL) {
        while (i < accountsize && (fscanf(fp, "%d~%c~%[^~]~%[^~]~%[^~]~%d~%lf~%[^\n]\n",
            &account[i].account_num, &account[i].customer, account[i].login.name,
            account[i].login.user, account[i].login.password, &account[i].demographic.year,
            &account[i].demographic.income, account[i].demographic.country) != EOF)) {
            if (account[i].account_num > 0)
                num_accounts++;
            i++;
        }
        fclose(fp);
    }
    else
        printf("Failed to open file\n");
    fp = NULL;
    return num_accounts;
}

// Save all accounts' information.
int saveAccounts(Account* account, const int accountsize) {
    int num_accounts = 0, i = 0;
    const char filename[] = "accounts.txt";
    FILE* fp = NULL;
    fp = fopen(filename, "w");
    if (fp != NULL) {
        for (i = 0; i < accountsize; i++) {
            if (account[i].account_num > 0) {
                fprintf(fp, "%d~%c~%s~%s~%s~%d~%lf~%s\n", account[i].account_num,
                    account[i].customer, account[i].login.name, account[i].login.user,
                    account[i].login.password, account[i].demographic.year,
                    account[i].demographic.income, account[i].demographic.country);
                num_accounts++;
            }
        }
        fclose(fp);
    }
    else
        printf("Failed to open file\n");
    fp = NULL;
    return num_accounts;

}

// Load all archived accounts'(removed) information.
int loadArchivedAccounts(Account* account, const int accountsize) {
    int num_accounts = 0, i = 0;
    const char filename[] = "accounts_arc.txt";
    FILE* fp = NULL;
    fp = fopen(filename, "a");
    fclose(fp);
    fp = fopen(filename, "r");
    if (fp != NULL) {
        while (i < accountsize && (fscanf(fp, "%d~%c~%[^~]~%[^~]~%[^~]~%d~%lf~%[^\n]\n",
            &account[i].account_num, &account[i].customer, account[i].login.name,
            account[i].login.user, account[i].login.password, &account[i].demographic.year,
            &account[i].demographic.income, account[i].demographic.country) != EOF)) {
            if (account[i].account_num > 0)
                num_accounts++;
            i++;
        }
        fclose(fp);
    }
    else
        printf("Failed to open file\n");
    fp = NULL;
    return num_accounts;
}

// Save all archived accounts' information.
int saveAchivedAccounts(Account* account, const int accountsize) {
    int num_accounts = 0, i = 0;
    const char filename[] = "accounts_arc.txt";
    FILE* fp = NULL;
    fp = fopen(filename, "a");
    if (fp != NULL) {
        for (i = 0; i < accountsize; i++) {
            if (account[i].account_num > 0) {
                fprintf(fp, "%d~%c~%s~%s~%s~%d~%lf~%s\n",
                    account[i].account_num, account[i].customer, account[i].login.name,
                    account[i].login.user, account[i].login.password,
                    account[i].demographic.year, account[i].demographic.income,
                    account[i].demographic.country);
                num_accounts++;
            }
        }
        fclose(fp);
    }
    else
        printf("Failed to open file\n");
    fp = NULL;
    return num_accounts;
}

// Copy an Account structure A from structure B.
void CpAccountsAFromB(Account* accountsA, Account* accountsB, int accounts_size) {
    int i;
    for (i = 0; i < accounts_size; i++) {
        accountsA[i].account_num = accountsB[i].account_num;
        accountsA[i].customer = accountsB[i].customer;
        accountsA[i].demographic.income = accountsB[i].demographic.income;
        accountsA[i].demographic.year = accountsB[i].demographic.year;
        strcpy(accountsA[i].demographic.country, accountsB[i].demographic.country);
        strcpy(accountsA[i].login.name, accountsB[i].login.name);
        strcpy(accountsA[i].login.password, accountsB[i].login.password);
        strcpy(accountsA[i].login.user, accountsB[i].login.user);
    }
    return;
}