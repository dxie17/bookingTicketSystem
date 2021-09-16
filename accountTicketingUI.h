#ifndef ACCOUNTTICKETINGUI_H_
#define ACCOUNTTICKETINGUI_H_

#include "account.h"
#include "ticket.h"

// Displays a formatted table header.
void displayAccountSummaryHeader(void);

// Displays a formatted table header.
void displayAccountDetailHeader(void);

// Receives a unmodifiable Account pointer argument.
void displayAccountSummaryRecord(const Account* account);

// Receives a unmodifiable Account pointer argument.
void displayAccountDetailRecord(const Account* account);

// The purpose of this function is to be the entry point to the application logic.
void applicationStartup(AccountTicketingData* data);

//  Load the agent main menu.
int menuLogin(AccountTicketingData* data);

//  Display's the customer's main menu options until the user wishes to logout.
void menuCustomer(AccountTicketingData* data, const Account* account);

//  Display's the agent's main menu options until the user wishes to logout.
void menuAgent(AccountTicketingData* data, const Account* account);

//  Find a account index.
int findAccountIndexByAcctNum(AccountTicketingData* data);

//  Displays a summary view of all the valid account records.
void displayAllAccountSummaryRecords(const AccountTicketingData* data);

//  Displays a detailed view of all the valid account records.
void displayAllAccountDetailRecords(const AccountTicketingData* data);

//  Pause execution until user enters the enter key.
void pauseExecution(void);

#endif