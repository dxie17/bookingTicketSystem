#ifndef ACCOUNT_H_
#define ACCOUNT_H_

// structures
typedef struct Demographic
{
	int year;
	double income;
	char country[100];
} Demographic;

typedef struct userLogin
{
	char name[100];
	char user[100];
	char password[100];
} UserLogin;

typedef struct Account
{
	int account_num;
	char customer;
	UserLogin login;
	Demographic demographic;
} Account;

typedef struct AccountTicketingData
{
	struct Account* accounts;   // array of accounts
	const int ACCOUNT_MAX_SIZE; // maximum elements for account array
	struct Ticket* tickets;     // array of tickets
	const int TICKET_MAX_SIZE;  // maximum elements for ticket array
}AccountTicketingData;

// Receives a modifiable Account pointer argument
void getAccount(Account* account);

// Receives a modifiable UserLogin pointer argument.
// Does not return anything but does return data for a UserLogin type.
void getUserLogin(UserLogin* userlogin);

// Receives a modifiable Demographic pointer argument.
// Does not return anything but does return data for a Demographic type.
void getDemographic(Demographic* demographic);

// Display's the update menu options for the account until the user wishes to exit.
void updateAccount(Account* account);

// Display's the update menu options for the user login until the user wishes to exit.
void updateUserLogin(UserLogin* login);

// Display's a menu with options to modifying specific members of a Demographic.
void updateDemographic(Demographic* demographic);

// Load accounts information.
int loadAccounts(Account* account, const int accountsize);

// Save all accounts' information.
int saveAccounts(Account* account, const int accountsize);

// Load all archived accounts'(removed) information.
int loadArchivedAccounts(Account* account, const int accountsize);

// Save all archived accounts' information.
int saveAchivedAccounts(Account* account, const int accountsize);

// Copy an Account structure A from structure B.
void CpAccountsAFromB(Account* accountsA, Account* accountsB, int accounts_size);
#endif