#define _CRT_SECURE_NO_WARNINGS
#define ACCOUNT_SIZE 15
#define TICKET_SIZE 17

#include <stdio.h>
#include <string.h>
#include "commonHelpers.h"
#include "account.h"
#include "accountTicketingUI.h"
#include "ticket.h"

// Displays a formatted table header.
void displayAccountSummaryHeader(void) {
    printf("Acct# Acct.Type Birth\n");
    printf("----- --------- -----\n");
}

// Displays a formatted table header.
void displayAccountDetailHeader(void) {
    printf("Acct# Acct.Type Birth Income      ");
    printf("Country    Disp.Name       Login      Password\n");
    printf("----- --------- ----- ----------- ");
    printf("---------- --------------- ---------- --------\n");
}

// Receives a unmodifiable Account pointer argument.
void displayAccountSummaryRecord(const Account* account) {
    if (account->customer == 'C' && account->account_num > 0) {
        printf("%05d %-9s ", account->account_num, "CUSTOMER");
        printf("%5d\n", account->demographic.year);
    }
    else if (account->customer == 'A' && account->account_num > 0) {
        printf("%05d %-9s ", account->account_num, "AGENT");
        printf("%5d\n", account->demographic.year);
    }
    return;
}

// Receives a unmodifiable Account pointer argument.
void displayAccountDetailRecord(const Account* account) {
    int len = 0, i = 1;
    if (account->account_num > 0) {
        if (account->customer == 'C')
            printf("%05d %-9s ", account->account_num, "CUSTOMER");
        else if (account->customer == 'A')
            printf("%05d %-9s ", account->account_num, "AGENT");
        printf("%5d ", account->demographic.year);
        printf("$%10.2lf %-10s %-15.15s", account->demographic.income,
            account->demographic.country, account->login.name);
        printf(" %-10s ", account->login.user);
        while (account->login.password[len] != '\0')
            len++;
        for (i = 1; i <= len; i++) {
            if (i % 2 > 0)
                printf("%c", account->login.password[i - 1]);
            else if (i % 2 == 0)
                printf("%c", '*');
        }
        printf("\n");
    }
    return;
}

// The purpose of this function is to be the entry point to the application logic.
void applicationStartup(AccountTicketingData* data) {
    int num_menulogin = 0, quit_num = 1;
    while (quit_num != 0) {
        num_menulogin = menuLogin(data);
        if (num_menulogin == -1) {
            printf("\n==============================================\n");
            printf("Account Ticketing System - Terminated\n");
            printf("==============================================\n");
            break;
        }
        else if (num_menulogin >= 0) {
            if (data->accounts[num_menulogin].customer == 'C')
                menuCustomer(data, &data->accounts[num_menulogin]);
            else
                menuAgent(data, &data->accounts[num_menulogin]);
        }
    }
    return;
}

//  Load the agent main menu.
int menuLogin(AccountTicketingData* data) {
    int min_num = 0, max_num = 1, num_quit = 1, choice = -1, p = 0, i;
    int user_num, password_num, input_account, error_num;
    char yesorno, str_user[50] = { 0 }, str_password[50] = { 0 }, str0[50] = { 0 };
    while (num_quit != 0) {
        switch (choice) {
        case -1:
            printf("==============================================\n");
            printf("Account Ticketing System - Login\n");
            printf("==============================================\n");
            printf("1) Login to the system\n");
            printf("0) Exit application\n");
            printf("----------------------------------------------\n\n");
            printf("Selection: ");
            choice = getIntFromRange(min_num, max_num);
            break;
        case 0:
            printf("\nAre you sure you want to exit? ([Y]es|[N]o): ");
            yesorno = getCharOption("yYnN");
            if (yesorno == 'y' || yesorno == 'Y') {
                num_quit = 0;
                p = -1;
                break;
            }
            else
                choice = -1;
            printf("\n");
            break;

        case 1:
            error_num = 0;
            printf("\n");
            while (error_num < 3) {
                i = 0, input_account = 0, user_num = 1, password_num = 1;
                strcpy(str_password, str0);
                strcpy(str_user, str0);
                input_account = findAccountIndexByAcctNum(data);
                printf("User Login    : ");
                scanf("%[^\n]", str_user);
                for (i = 0; i < 50; i++) {
                    if (input_account >= 0) {
                        if (str_user[i] != '0') {
                            if (str_user[i] != data->accounts[input_account].login.user[i])
                                user_num = 0;
                        }
                        else
                            break;
                    }
                }
                while (getchar() != '\n');
                printf("Password      : ");
                scanf("%[^\n]", str_password);
                for (i = 0; i < 50; i++) {
                    if (input_account >= 0) {
                        if (str_password[i] != '0') {
                            if (str_password[i] != data->accounts[input_account].login.password[i])
                                password_num = 0;
                        }
                        else
                            break;
                    }
                }
                error_num++;
                while (getchar() != '\n');
                if ((input_account < 0 || user_num != 1 || password_num != 1) && error_num < 3)
                    printf("INVALID user login/password combination! [attempts remaining:%d]\n\n",
                        3 - error_num);
                else if ((input_account < 0 || user_num != 1 || password_num != 1)
                    && error_num == 3) {
                    printf("INVALID user login/password combination! [attempts remaining:%d]\n\n",
                        3 - error_num);
                    printf("ERROR:  Login failed!\n\n");
                    choice = -1;
                    num_quit = 1;
                    pauseExecution();
                    break;
                }
                else if (input_account >= 0 && user_num == 1 && password_num == 1
                    && error_num <= 3) {
                    p = input_account;
                    num_quit = 0;
                    choice = -2;
                    printf("\n");
                    break;
                }
            }
            break;
        }
    }
    return p;
}

//  Display's the customer's main menu options until the user wishes to logout.
void menuCustomer(AccountTicketingData* data, const Account* account) {
    int i = 0, choice = -1, num_quit = 1, loop4, ticket_num = 0, ticket_counter;
    int save_tickets_num = 0;
    while (num_quit != 0) {
        switch (choice) {
        case -1:
            printf("CUSTOMER: %s (%d)\n", account->login.name, account->account_num);
            printf("==============================================\n");
            printf("Customer Main Menu\n");
            printf("==============================================\n");
            printf("1) View your account detail\n");
            printf("2) Create a new ticket\n");
            printf("3) Modify an active ticket\n");
            printf("4) List my tickets\n");
            printf("----------------------------------------------\n");
            printf("0) Logout\n\n");
            printf("Selection: ");
            choice = getIntFromRange(0, 4);
            printf("\n");
            num_quit = 1;
            break;
        case 0:
            save_tickets_num = saveTicketsInformation(data->tickets, TICKET_SIZE);
            printf("Saving session modifications...\n");
            printf("   %d tickets saved.\n", save_tickets_num);
            printf("### LOGGED OUT ###\n\n");
            num_quit = 0;
            break;
        case 1:
            displayAccountDetailHeader();
            displayAccountDetailRecord(account);
            choice = -1;
            printf("\n");
            pauseExecution();
            break;
        case 2:
            ticket_num = CreateNewTicket(data->tickets, account->account_num);
            if (ticket_num > 0) {
                for (i = 0; i < TICKET_SIZE; i++) {
                    if (data->tickets[i].ticket_num == ticket_num)
                        break;
                }
                strcpy(data->tickets[i].message->mes_showname, account->login.name);
            }
            choice = -1;
            break;
        case 3:
            ticket_num = 0, ticket_counter = 0;
            printf("Enter ticket number: ");
            ticket_num = getNotNegativeInteger();
            if (ticket_num > 0) {
                for (i = 0; i < data->TICKET_MAX_SIZE; i++) {
                    if (data->tickets[i].ticket_num == ticket_num) {
                        ticket_counter = 1;
                        break;
                    }
                }
                if (ticket_counter == 0 ||
                    (data->tickets[i].ticket_C_num != account->account_num)) {
                    printf("\nERROR: Invalid ticket number - you may only"
                        " modify your own ticket.\n\n");
                    pauseExecution();
                }

                else if (data->tickets[i].ticket_status == 0) {
                    printf("\nERROR: Ticket is closed - changes are not permitted.\n\n");
                    pauseExecution();
                }
                else
                    CustomerModifyTicket(&data->tickets[i], ticket_num);
                choice = -1;
            }
            break;
        case 4:
            loop4 = 1, ticket_num = -1, ticket_counter = 0;
            while (loop4) {
                ShowTicketsRecords_C(data->tickets, account->account_num);
                printf("\nEnter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");
                ticket_num = getNotNegativeInteger();
                if (ticket_num > 0) {
                    for (i = 0; i < data->TICKET_MAX_SIZE; i++) {
                        if (data->tickets[i].ticket_num == ticket_num) {
                            ticket_counter = 1;
                            break;
                        }
                    }
                    if (ticket_counter == 0) {
                        printf("\nERROR: Invalid ticket number.\n\n");
                        pauseExecution();
                        continue;
                    }
                    else if (ticket_counter == 1 && data->tickets[i].ticket_C_num != account->account_num) {
                        printf("\nERROR: Invalid ticket number - you may only access your own tickets.\n\n");
                        pauseExecution();
                        loop4 = 1;
                    }
                    else if (ticket_counter == 1 && data->tickets[i].ticket_C_num == account->account_num)
                        ShowMessageInformation(&data->tickets[i], ticket_num);
                }
                else if (ticket_num == 0)
                {
                    printf("\n");
                    choice = -1;
                    break;
                }
            }
            break;
        }
    }
    return;
}

//  Display's the agent's main menu options until the user wishes to logout.
void menuAgent(AccountTicketingData* data, const Account* account) {
    int i = 0, choice = -1, num_quit = 1, account_num, ticket_num, ticket_counter;
    int loop, account_counter, num_arc_accounts, arc_tick;
    int save_accounts_num = 0, save_tickets_num = 0;
    Account accounts_arc[ACCOUNT_SIZE] = { {0} }, accounts_NULL[ACCOUNT_SIZE] = { {0} };
    Counter counter_num[1] = { {0} };
    Ticket tickets_arc[TICKET_SIZE] = { {0} }, tickets_NULL[TICKET_SIZE] = { {0} };
    char yesorno;
    while (num_quit != 0)
    {
        account_num = 0, ticket_counter = 0, ticket_num = 0;
        switch (choice) {
        case -1:
            printf("AGENT: %s (%d)\n", account->login.name, account->account_num);
            printf("==============================================\n");
            printf("Account Ticketing System - Agent Menu\n");
            printf("==============================================\n");
            printf(" 1) Add a new account\n");
            printf(" 2) Modify an existing account\n");
            printf(" 3) Remove an account\n");
            printf(" 4) List accounts: summary view\n");
            printf(" 5) List accounts: detailed view\n");
            printf("----------------------------------------------\n");
            printf(" 6) List new tickets\n");
            printf(" 7) List active tickets\n");
            printf(" 8) List closed tickets\n");
            printf(" 9) Manage a ticket\n");
            printf("10) Archive closed tickets\n");
            printf("----------------------------------------------\n");
            printf("11) View archived account statistics\n");
            printf("12) View archived tickets statistics\n");
            printf("----------------------------------------------\n");
            printf("0) Logout\n\n");
            printf("Selection: ");
            choice = getIntFromRange(0, 12);
            printf("\n");
            num_quit = 1;
            break;
        case 0:
            save_accounts_num = saveAccounts(data->accounts, ACCOUNT_SIZE);
            save_tickets_num = saveTicketsInformation(data->tickets, TICKET_SIZE);
            printf("Saving session modifications...\n");
            printf("   %d account saved.\n", save_accounts_num);
            printf("   %d tickets saved.\n", save_tickets_num);
            printf("### LOGGED OUT ###\n\n");
            num_quit = 0;
            break;
        case 1:
            account_counter = 0;
            for (i = 0; i < ACCOUNT_SIZE; i++) {
                if (data->accounts[i].account_num == 0) {
                    account_counter = 1;
                    break;
                }
            }
            if (account_counter == 1) {
                getAccount(data->accounts);
                getUserLogin(&data->accounts[i].login);
                getDemographic(&data->accounts[i].demographic);
                printf("*** New account added! ***\n\n");
                saveAccounts(data->accounts, ACCOUNT_SIZE);
            }
            else
                printf("ERROR: Account listing is FULL, call ITS Support!\n\n");
            choice = -1;
            pauseExecution();
            break;
        case 2:
            account_num = findAccountIndexByAcctNum(data);
            if (account_num >= 0) {
                printf("\n");
                updateAccount(&data->accounts[account_num]);
                saveAccounts(data->accounts, ACCOUNT_SIZE);
            }
            else if (account_num < 0)
                printf("ERROR, NO THIS ACCOUNT\n");
            choice = -1;
            break;
        case 3:
            account_num = findAccountIndexByAcctNum(data);
            if (account_num < 0)
                printf("NO THIS ACCOUNT!!!\n");
            else {
                if (account->account_num == data->accounts[account_num].account_num) {
                    printf("\nERROR: You can't remove your own account!\n\n");
                    pauseExecution();
                }
                else if (account_num != data->accounts[account_num].account_num) {
                    displayAccountDetailHeader();
                    displayAccountDetailRecord(&data->accounts[account_num]);
                    printf("\nAre you sure you want to remove this record? ([Y]es|[N]o): ");
                    yesorno = getCharOption("YN");
                    if (yesorno == 'N') {
                        printf("\n*** No changes made! ***\n\n");
                        pauseExecution();
                    }
                    else if (yesorno == 'Y') {
                        printf("\n*** Account Removed! ***\n\n");
                        saveAchivedAccounts(&data->accounts[account_num], 1);
                        for (i = 0; i < TICKET_SIZE; i++) {
                            if (data->accounts[account_num].account_num == data->tickets[i].ticket_C_num) {
                                if (data->tickets[i].ticket_status == 0)
                                    saveArchivedTicketsInformation(&data->tickets[i], 1);
                                CpTicketAFromB(&data->tickets[i], &tickets_NULL[0], 1);
                            }
                        }
                        CpAccountsAFromB(&data->accounts[account_num], &accounts_NULL[0], 1);
                        saveAccounts(data->accounts, ACCOUNT_SIZE);
                        saveTicketsInformation(data->tickets, TICKET_SIZE);
                        pauseExecution();
                    }
                }
            }
            choice = -1;
            num_quit = 1;
            break;
        case 4:
            displayAllAccountSummaryRecords(data);
            choice = -1;
            printf("\n");
            pauseExecution();
            break;
        case 5:
            displayAllAccountDetailRecords(data);
            choice = -1;
            printf("\n");
            pauseExecution();
            break;
        case 6:
            loop = 1, ticket_num = -1;
            while (loop) {
                ShowTicketsRecords_A(data->tickets, -1, -1, 1);
                printf("\nEnter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");
                ticket_num = getNotNegativeInteger();
                if (ticket_num > 0) {
                    for (i = 0; i < data->TICKET_MAX_SIZE; i++) {
                        if (data->tickets[i].ticket_num == ticket_num) {
                            ticket_counter = 1;
                            break;
                        }
                    }
                    if (ticket_counter == 0) {
                        printf("\nERROR: Invalid ticket number.\n\n");
                        pauseExecution();
                    }
                    else if (ticket_counter == 1)
                        ShowMessageInformation(&data->tickets[i], ticket_num);
                }
                else if (ticket_num == 0) {
                    printf("\n");
                    choice = -1;
                    break;
                }
            }
            break;
        case 7:
            loop = 1, ticket_num = -1;
            while (loop) {
                ShowTicketsRecords_A(data->tickets, -1, 1, -1);
                printf("\nEnter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");
                ticket_num = getNotNegativeInteger();
                if (ticket_num > 0) {
                    for (i = 0; i < data->TICKET_MAX_SIZE; i++) {
                        if (data->tickets[i].ticket_num == ticket_num) {
                            ticket_counter = 1;
                            break;
                        }
                    }
                    if (ticket_counter == 0) {
                        printf("\nERROR: Invalid ticket number.\n\n");
                        pauseExecution();
                    }
                    else if (ticket_counter == 1)
                        ShowMessageInformation(&data->tickets[i], ticket_num);
                }
                else if (ticket_num == 0) {
                    printf("\n");
                    choice = -1;
                    break;
                }
            }
            break;
        case 8:
            loop = 1, ticket_num = -1;
            while (loop) {
                ShowTicketsRecords_A(data->tickets, -1, 0, -1);
                printf("\nEnter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");
                ticket_num = getNotNegativeInteger();
                if (ticket_num > 0) {
                    for (i = 0; i < data->TICKET_MAX_SIZE; i++) {
                        if (data->tickets[i].ticket_num == ticket_num) {
                            ticket_counter = 1;
                            break;
                        }
                    }
                    if (ticket_counter == 0) {
                        printf("\nERROR: Invalid ticket number.\n\n");
                        pauseExecution();
                    }
                    else if (ticket_counter == 1)
                        ShowMessageInformation(&data->tickets[i], ticket_num);
                }
                else if (ticket_num == 0) {
                    printf("\n");
                    choice = -1;
                    break;
                }
            }
            break;
        case 9:
            ticket_num = 0, ticket_counter = 0;
            printf("Enter ticket number: ");
            ticket_num = getPositiveInteger();
            for (i = 0; i < data->TICKET_MAX_SIZE; i++) {
                if (data->tickets[i].ticket_num == ticket_num) {
                    ticket_counter = 1;
                    break;
                }
            }
            if (ticket_counter == 0)
                printf("\nERROR: Invalid ticket number.\n\n");
            else
                AgentManageTickets(&data->tickets[i], ticket_num, account);
            choice = -1;
            break;
        case 10:
            yesorno = '\0', arc_tick = 0;
            printf("Are you sure? This action cannot be reversed. ([Y]es|[N]o): ");
            yesorno = getCharOption("YyNn");
            if (yesorno == 'Y' || yesorno == 'y') {
                for (i = 0; i < TICKET_SIZE; i++) {
                    if (data->tickets[i].ticket_num > 0 && data->tickets[i].ticket_status == 0) {
                        saveArchivedTicketsInformation(&data->tickets[i], 1);
                        CpTicketAFromB(&data->tickets[i], &tickets_NULL[0], 1);
                        arc_tick++;
                    }
                }
                printf("\n*** %d tickets archived ***\n\n", arc_tick);
            }
            choice = -1;
            pauseExecution();
            break;
        case 11:
            num_arc_accounts = 0;
            CpAccountsAFromB(accounts_arc, accounts_NULL, ACCOUNT_SIZE);
            num_arc_accounts = loadArchivedAccounts(accounts_arc, ACCOUNT_SIZE);
            printf("There are %d account(s) currently archived.\n\n", num_arc_accounts);
            choice = -1;
            pauseExecution();
            break;
        case 12:
            counter_num[0].number_messages = 0;
            counter_num[0].number_tickets = 0;
            CpTicketAFromB(tickets_arc, tickets_NULL, 17);
            counter_num[0] = loadArchivedTickets(tickets_arc, TICKET_SIZE);
            printf("There are %d ticket(s) and a total of %d message(s) archived.\n\n",
                counter_num[0].number_tickets, counter_num[0].number_messages);
            choice = -1;
            pauseExecution();
            break;
        }
    }
}

//  Find a account index.
int findAccountIndexByAcctNum(AccountTicketingData* data) {
    int i = 0, p = 1, num_account = 0;
    printf("Enter the account#: ");
    num_account = getPositiveInteger();
    for (i = 0; i < data->ACCOUNT_MAX_SIZE; i++) {
        if (num_account == data->accounts[i].account_num) {
            p = 0;
            break;
        }
    }
    if (p == 1)
        i = -1;
    return i;
}

//  Displays a summary view of all the valid account records.
void displayAllAccountSummaryRecords(const AccountTicketingData* data) {
    int i = 0;
    displayAccountSummaryHeader();
    for (i = 0; i < data->ACCOUNT_MAX_SIZE; i++)
        displayAccountSummaryRecord(&data->accounts[i]);
}

//  Displays a detailed view of all the valid account records.
void displayAllAccountDetailRecords(const AccountTicketingData* data) {
    int i = 0;
    displayAccountDetailHeader();
    for (i = 0; i < data->ACCOUNT_MAX_SIZE; i++)
        displayAccountDetailRecord(&data->accounts[i]);
}

//  Pause execution until user enters the enter key.
void pauseExecution(void) {
    printf("<< ENTER key to Continue... >>");
    clearStandardInputBuffer();
    putchar('\n');
}