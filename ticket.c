
#define _CRT_SECURE_NO_WARNINGS
#define ACCOUNT_SIZE 15
#define TICKET_SIZE 17

#include <stdio.h>
#include <string.h>
#include "commonHelpers.h"
#include "account.h"
#include "accountTicketingUI.h"

// Agents display a detailed view of tickets based on different status.
// ticketshow=0 print all the records.
// ticketshow > 0 print the record which the ticket number equals ticketshow.
// active=1 print active records.
// num_messages=1 print the new records.
void ShowTicketsRecords_A(Ticket* tickets, int ticketnumber, int active, int num_messages) {
    int i;
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
    printf("Ticket Acct# Display Name    Status Subject                        Messages\n");
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
    for (i = 0; i < TICKET_SIZE; i++) {
        if (tickets[i].ticket_num > 0) {
            if (ticketnumber == 0 && num_messages < 0 && active < 0) {
                printf("%06d %5d ", tickets[i].ticket_num, tickets[i].ticket_C_num);
                printf("%-15s ", tickets[i].message->mes_showname);
                printf("%-6s ", tickets[i].ticket_status == 0 ? "CLOSED" : "ACTIVE");
                printf("%-30s  %4d\n", tickets[i].ticket_line, tickets[i].ticket_counter);
            }
            else if (ticketnumber == tickets[i].ticket_num && num_messages < 0 && active < 0) {
                printf("%06d %5d ", tickets[i].ticket_num, tickets[i].ticket_C_num);
                printf("%-15s ", tickets[i].message->mes_showname);
                printf("%-6s ", tickets[i].ticket_status == 0 ? "CLOSED" : "ACTIVE");
                printf("%-30s  %4d\n", tickets[i].ticket_line, tickets[i].ticket_counter);
                break;
            }
            else if (ticketnumber < 0 && active == tickets[i].ticket_status && num_messages < 0)
            {
                printf("%06d %5d ", tickets[i].ticket_num, tickets[i].ticket_C_num);
                printf("%-15s ", tickets[i].message->mes_showname);
                printf("%-6s ", tickets[i].ticket_status == 0 ? "CLOSED" : "ACTIVE");
                printf("%-30s  %4d\n", tickets[i].ticket_line, tickets[i].ticket_counter);
            }
            else if (ticketnumber < 0 && num_messages == tickets[i].ticket_counter && active < 0) {
                printf("%06d %5d ", tickets[i].ticket_num, tickets[i].ticket_C_num);
                printf("%-15s ", tickets[i].message->mes_showname);
                printf("%-6s ", tickets[i].ticket_status == 0 ? "CLOSED" : "ACTIVE");
                printf("%-30s  %4d\n", tickets[i].ticket_line, tickets[i].ticket_counter);
            }

        }
    }
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
    return;
}

//  Customers display a detailed view of tickets based on different status.
void ShowTicketsRecords_C(Ticket* tickets, const int c_number) {
    int i;
    printf("------ ------ ------------------------------ --------\n");
    printf("Ticket Status Subject                        Messages\n");
    printf("------ ------ ------------------------------ --------\n");
    for (i = 0; i < TICKET_SIZE; i++) {
        if (c_number == tickets[i].ticket_C_num) {
            printf("%06d %-6s ", tickets[i].ticket_num,
                tickets[i].ticket_status == 1 ? "ACTIVE" : "CLOSED");
            printf("%-30s  %4d\n", tickets[i].ticket_line,
                tickets[i].ticket_counter);
        }
    }
    printf("------ ------ ------------------------------ --------\n");
    return;
}

// DisplayS a detailed messages of a certain ticket.
void ShowMessageInformation(Ticket* tickets, int ticketnumber) {
    int message_num, i = 0;
    for (i = 0; i < TICKET_SIZE; i++) {
        if (ticketnumber == tickets[i].ticket_num)
            break;
    }
    printf("\n=======================================");
    printf("=========================================\n");
    printf("%06d (%s)",
        ticketnumber, tickets[i].ticket_status == 1 ? "ACTIVE" : "CLOSED");
    printf(" Re: %s\n", tickets[i].ticket_line);
    printf("=======================================");
    printf("=========================================\n");
    for (message_num = 0; message_num < tickets[i].ticket_counter; message_num++) {
        if (tickets->message[message_num].mes_type != '\0') {
            printf("%s", tickets->message[message_num].mes_type == 'C' ? "CUSTOMER" : "AGENT");
            printf(" (%s):\n", tickets->message[message_num].mes_showname);
            printf("   %s\n\n", tickets->message[message_num].mes_detail);
            if ((message_num + 1) % 5 == 0)
                pauseExecution();
        }
    }
    if (message_num % 5 > 0)
        pauseExecution();
    return;
}

// Create a new ticket by the customer,return >0 sucessful, return 0 unsucessful.
int CreateNewTicket(Ticket* tickets, const int c_number) {
    int i = 0, nullornot = 0, ticketnumber = 0;
    for (i = 0; i < TICKET_SIZE; i++) {
        if (tickets[i].ticket_num > ticketnumber)
            ticketnumber = tickets[i].ticket_num;
    }
    for (i = 0; i < TICKET_SIZE; i++) {
        if (tickets[i].ticket_num == 0) {
            nullornot = 1;
            break;
        }
    }
    if (nullornot == 1) {
        ticketnumber++;
        tickets[i].ticket_num = ticketnumber;
        printf("New Ticket (Ticket#:%06d)\n", tickets[i].ticket_num);
        printf("----------------------------------------\n");
        printf("Enter the ticket SUBJECT (30 chars. maximum): ");
        getCString(tickets[i].ticket_line, 1, 30);
        printf("\nEnter the ticket message details (150 chars. maximum)."
            " Press the ENTER key to submit:\n");
        getCString(tickets[i].message[0].mes_detail, 1, 150);
        tickets[i].ticket_C_num = c_number;
        tickets[i].ticket_status = 1;
        tickets[i].ticket_counter = 1;
        tickets[i].message[0].mes_type = 'C';
        printf("\n*** New ticket created! ***\n\n");
    }
    else if (nullornot == 0) {
        printf("ERROR: Ticket listing is FULL, call ITS Support!\n\n");
        ticketnumber = 0;
    }
    pauseExecution();
    return ticketnumber;
}

// Customer modifies an active ticket.
void CustomerModifyTicket(Ticket* tickets, const int ticketnumber) {
    int i = 0, p, choice = -1, num_quit = 1, max_message = 20, testnummessage;
    char str1[50] = { 0 }, str0[200] = { 0 }, str2[200], yn_message, yn_close;
    for (i = 0; i < TICKET_SIZE; i++) {
        if (tickets[i].ticket_num == ticketnumber)
            break;
    }
    while (num_quit != 0) {
        testnummessage = 0;
        switch (choice) {
        case -1:
            printf("\n----------------------------------------\n");
            printf("Ticket %06d - Update Options\n", ticketnumber);
            printf("----------------------------------------\n");
            printf("Status  : %s\n", tickets[i].ticket_status == 1 ? "ACTIVE" : "CLOSED");
            printf("Subject : %s\n", tickets[i].ticket_line);
            printf("----------------------------------------\n");
            printf("1) Modify the subject\n");
            printf("2) Add a message\n");
            printf("3) Close ticket\n");
            printf("0) Done\n");
            printf("Selection: ");
            choice = getIntFromRange(0, 3);
            printf("\n");
            num_quit = 1;
            break;
        case 0:
            num_quit = 0;
            pauseExecution();
            break;
        case 1:
            printf("Enter the revised ticket SUBJECT (30 chars. maximum): ");
            getCString(str1, 1, 30);
            strcpy(tickets[i].ticket_line, str1);
            choice = -1;
            break;
        case 2:
            strcpy(str2, str0);
            for (p = 0; p < max_message; p++) {
                if (tickets[i].message[p].mes_type == '\0') {
                    testnummessage = 1;
                    break;
                }
            }
            if (testnummessage == 1) {
                printf("Enter the ticket message details "
                    "(150 chars. maximum). Press the ENTER key to submit:\n");
                getCString(str2, 1, 150);
                strcpy(tickets[i].message[p].mes_detail, str2);
                tickets[i].ticket_counter++;
                tickets[i].message[p].mes_type = 'C';
                strcpy(tickets[i].message[p].mes_showname, tickets[i].message[0].mes_showname);
            }
            else
                printf("ERROR: Message limit has been reached, call ITS Support!\n");
            choice = -1;
            break;
        case 3:
            yn_message = '\0', yn_close = '\0';
            strcpy(str2, str0);
            printf("Are you sure you CLOSE this ticket? ([Y]es|[N]o): ");
            yn_close = getCharOption("yYnN");
            if (yn_close == 'y' || yn_close == 'Y') {
                if (tickets[i].ticket_counter < max_message) {
                    printf("\nDo you want to leave a closing message? ([Y]es|[N]o): ");
                    yn_message = getCharOption("yYnN");
                    if (yn_message == 'y' || yn_message == 'Y') {
                        printf("\nEnter the ticket message details "
                            "(150 chars. maximum). Press the ENTER key to submit:\n");
                        getCString(str2, 1, 150);
                        for (p = 0; p < max_message; p++) {
                            if (tickets[i].message[p].mes_type == '\0') {
                                testnummessage = 1;
                                break;
                            }
                        }
                        if (testnummessage == 1) {
                            strcpy(tickets[i].message[p].mes_detail, str2);
                            tickets[i].ticket_counter++;
                            tickets[i].message[p].mes_type = 'C';
                            strcpy(tickets[i].message[p].mes_showname, tickets[i].message[0].mes_showname);
                        }
                    }
                }
                printf("\n*** Ticket closed! ***\n\n");
                tickets[i].ticket_status = 0;
                pauseExecution();
            }
            num_quit = 0;
            break;
        }
    }
    return;
}

// Agent modifies tickets.
void AgentManageTickets(Ticket* tickets, const int ticketnumber, const Account* account) {
    int i = 0, p, choice = -1, num_quit = 1, max_message = 20, testnummessage;
    char str0[200] = { 0 }, str1[200];
    char yn_message, yn_close, yn_reopen;
    for (i = 0; i < TICKET_SIZE; i++) {
        if (tickets[i].ticket_num == ticketnumber)
            break;
    }
    while (num_quit != 0) {
        testnummessage = 0;
        switch (choice) {
        case -1:
            printf("\n----------------------------------------\n");
            printf("Ticket %06d - Update Options\n", ticketnumber);
            printf("----------------------------------------\n");
            printf("Status  : %s\n", tickets[i].ticket_status == 1 ? "ACTIVE" : "CLOSED");
            printf("Subject : %s\n", tickets[i].ticket_line);
            printf("Acct#   : %d\n", tickets[i].ticket_C_num);
            printf("Customer: %s\n", tickets[i].message[0].mes_showname);
            printf("----------------------------------------\n");
            printf("1) Add a message\n");
            printf("2) Close ticket\n");
            printf("3) Re-open ticket\n");
            printf("0) Done\n");
            printf("Selection: ");
            choice = getIntFromRange(0, 3);
            printf("\n");
            num_quit = 1;
            break;
        case 0:
            num_quit = 0;
            break;
        case 1:
            strcpy(str1, str0);
            for (p = 0; p < max_message; p++) {
                if (tickets[i].message[p].mes_type == '\0') {
                    testnummessage = 1;
                    break;
                }
            }
            if (testnummessage == 1 && tickets[i].ticket_status == 1) {
                printf("Enter the ticket message details "
                    "(150 chars. maximum). Press the ENTER key to submit:\n");
                getCString(str1, 1, 150);
                strcpy(tickets[i].message[p].mes_detail, str1);
                tickets[i].ticket_counter++;
                tickets[i].message[p].mes_type = 'A';
                strcpy(tickets[i].message[p].mes_showname, account->login.name);
            }
            else if (testnummessage == 1 && tickets[i].ticket_status == 0)
                printf("ERROR: Ticket is closed - new messages are not permitted.\n");
            else
                printf("ERROR: Message limit has been reached, call ITS Support!\n");
            choice = -1;
            break;
        case 2:
            yn_message = '\0', yn_close = '\0';
            strcpy(str1, str0);
            if (tickets[i].ticket_status == 0)
                printf("ERROR: Ticket is already closed!\n");
            else {
                printf("Are you sure you CLOSE this ticket? ([Y]es|[N]o): ");
                yn_close = getCharOption("yYnN");
                if ((yn_close == 'y' || yn_close == 'Y') && tickets[i].ticket_counter < max_message) {
                    printf("\nDo you want to leave a closing message? ([Y]es|[N]o): ");
                    yn_message = getCharOption("yYnN");
                    if (yn_message == 'y' || yn_message == 'Y') {
                        printf("\nEnter the ticket message details "
                            "(150 chars. maximum). Press the ENTER key to submit:\n");
                        getCString(str1, 1, 150);
                        for (p = 0; p < max_message; p++) {
                            if (tickets[i].message[p].mes_type == '\0') {
                                testnummessage = 1;
                                break;
                            }
                        }
                        if (testnummessage == 1) {
                            strcpy(tickets[i].message[p].mes_detail, str1);
                            tickets[i].ticket_counter++;
                            tickets[i].message[p].mes_type = 'A';
                            strcpy(tickets[i].message[p].mes_showname, account->login.name);
                        }
                        else
                            printf("ERROR: Message limit has been reached, call ITS Support!\n");
                    }
                }
                printf("\n*** Ticket closed! ***\n");
                tickets[i].ticket_status = 0;
            }
            choice = -1;
            break;
        case 3:
            yn_reopen = '\0';
            if (tickets[i].ticket_status == 1)
                printf("ERROR: Ticket is already active!\n");
            else {
                printf("Are you sure you RE-OPEN this closed ticket? ([Y]es|[N]o): ");
                yn_reopen = getCharOption("yYnN");
                if (yn_reopen == 'y' || yn_reopen == 'Y') {
                    printf("\n*** Ticket re-opened! ***\n");
                    tickets[i].ticket_status = 1;
                }
            }
            choice = -1;
            break;
        }

    }
    return;
}

// Load all tickets' information.
int loadTickets(Ticket* tickets, const int ticketsize) {
    int num_tickets = 0, i = 0, message_num = 0;
    const char filename[] = "tickets.txt";
    char spacedelete = '0';
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    if (fp != NULL) {
        while (i < ticketsize && (fscanf(fp, "%d|%d|%d|%[^|]|%d|",
            &tickets[i].ticket_num, &tickets[i].ticket_C_num,
            &tickets[i].ticket_status, tickets[i].ticket_line,
            &tickets[i].ticket_counter) != EOF)) {
            for (message_num = 0; message_num < tickets[i].ticket_counter; message_num++) {
                fscanf(fp, "%c|%[^|]|%[^|]|",
                    &tickets[i].message[message_num].mes_type,
                    tickets[i].message[message_num].mes_showname,
                    tickets[i].message[message_num].mes_detail);
            }
            fscanf(fp, "%c", &spacedelete);
            if (tickets[i].ticket_num > 0)
                num_tickets++;
            i++;
        }
        fclose(fp);
    }
    else
        printf("Failed to open file!\n");
    fp = NULL;
    return num_tickets;
}

// Save all tickets' information.
int saveTicketsInformation(Ticket* tickets, const int ticketsize) {
    int num_tickets = 0, i = 0, message_num = 0;
    const char filename[] = "tickets.txt";
    FILE* fp = NULL;
    fp = fopen(filename, "w");
    if (fp != NULL) {
        for (i = 0; i < ticketsize; i++) {
            if (tickets[i].ticket_num > 0) {
                fprintf(fp, "%d|%d|%d|%s|%d|",
                    tickets[i].ticket_num, tickets[i].ticket_C_num,
                    tickets[i].ticket_status, tickets[i].ticket_line,
                    tickets[i].ticket_counter);
                for (message_num = 0; message_num < tickets[i].ticket_counter; message_num++) {
                    if (tickets[i].message[message_num].mes_type != '\0')
                        fprintf(fp, "%c|%s|%s|", tickets[i].message[message_num].mes_type,
                            tickets[i].message[message_num].mes_showname,
                            tickets[i].message[message_num].mes_detail);
                }
                fprintf(fp, "\n");
                num_tickets++;
            }

        }
        fclose(fp);
    }
    else
        printf("Failed to open file!\n");
    fp = NULL;
    return num_tickets;
}

// Load all archived tickets' information.
Counter loadArchivedTickets(Ticket* tickets, const int ticketsize) {
    Counter counter_num[1] = { {0} };
    int i = 0, message_num = 0;
    const char filename[] = "tickets_arc.txt";
    char spacedelete = '0';
    FILE* fp = NULL;
    fp = fopen(filename, "a");
    fclose(fp);
    fp = fopen(filename, "r");
    if (fp != NULL) {
        while (i < ticketsize && (fscanf(fp, "%d|%d|%d|%[^|]|%d|",
            &tickets[i].ticket_num, &tickets[i].ticket_C_num,
            &tickets[i].ticket_status, tickets[i].ticket_line,
            &tickets[i].ticket_counter) != EOF)) {
            for (message_num = 0; message_num < tickets[i].ticket_counter; message_num++) {
                fscanf(fp, "%c|%[^|]|%[^|]|", &tickets[i].message[message_num].mes_type,
                    tickets[i].message[message_num].mes_showname,
                    tickets[i].message[message_num].mes_detail);
                counter_num[0].number_messages++;
            }
            fscanf(fp, "%c", &spacedelete);
            if (tickets[i].ticket_num > 0)
                counter_num[0].number_tickets++;
            i++;
        }
        fclose(fp);
    }
    else
        printf("Failed to open file!\n");
    fp = NULL;
    return counter_num[0];
}

// Save all archived tickets' information.
int saveArchivedTicketsInformation(Ticket* tickets, const int ticketsize) {
    int num_tickets = 0, i = 0, message_num = 0;
    const char filename[] = "tickets_arc.txt";
    FILE* fp = NULL;
    fp = fopen(filename, "a");
    if (fp != NULL) {
        for (i = 0; i < ticketsize; i++) {
            if (tickets[i].ticket_num > 0 && tickets[i].ticket_status == 0) {
                fprintf(fp, "%d|%d|%d|%s|%d|",
                    tickets[i].ticket_num, tickets[i].ticket_C_num,
                    tickets[i].ticket_status, tickets[i].ticket_line,
                    tickets[i].ticket_counter);
                for (message_num = 0; message_num < tickets[i].ticket_counter; message_num++) {
                    if (tickets[i].message[message_num].mes_type != '\0')
                        fprintf(fp, "%c|%s|%s|", tickets[i].message[message_num].mes_type,
                            tickets[i].message[message_num].mes_showname,
                            tickets[i].message[message_num].mes_detail);
                }
                fprintf(fp, "\n");
                num_tickets++;
            }

        }
        fclose(fp);
    }
    else
        printf("Failed to open file!\n");
    fp = NULL;
    return num_tickets;
}

// Copy a Ticket structure A from structure B.
void CpTicketAFromB(Ticket* ticketsA, Ticket* ticketsB, int size_tickets) {
    int i;
    for (i = 0; i < size_tickets; i++) {
        ticketsA[i].ticket_counter = ticketsB[i].ticket_counter;
        ticketsA[i].ticket_C_num = ticketsB[i].ticket_C_num;
        ticketsA[i].ticket_num = ticketsB[i].ticket_num;
        ticketsA[i].ticket_status = ticketsB[i].ticket_status;
        strcpy(ticketsA[i].ticket_line, ticketsB[i].ticket_line);
        CpMessageAFromB(ticketsA[i].message, ticketsB[i].message, 20);
    }
    return;
}

// Copy a Message structure A from structure B.
void CpMessageAFromB(Message* messageA, Message* messageB, int size_message) {
    int i;
    for (i = 0; i < size_message; i++) {
        messageA[i].mes_type = messageB[i].mes_type;
        strcpy(messageA[i].mes_showname, messageB[i].mes_showname);
        strcpy(messageA[i].mes_detail, messageB[i].mes_detail);
    }
    return;
}