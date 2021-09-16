#ifndef TICKET_H_
#define TICKET_H_

// structures
typedef struct Message
{
	char mes_type;
	char mes_showname[50];
	char mes_detail[200];
} Message;

typedef struct Ticket
{
	int ticket_num;
	int ticket_C_num;
	int ticket_status;
	char ticket_line[50];
	int ticket_counter;
	Message message[20];
} Ticket;

typedef struct Counter
{
	int number_tickets;
	int number_messages;
} Counter;

// Agents display a detailed view of tickets based on different status.
// ticketshow=0 print all the records.
// ticketshow > 0 print the record which the ticket number equals ticketshow.
// active=1 print active records.
// num_messages=1 print the new records.
void ShowTicketsRecords_A(Ticket* tickets, int ticketnumber, int active, int num_messages);

//Customers display a detailed view of tickets based on different status.
void ShowTicketsRecords_C(Ticket* tickets, const int c_number);

// Displays a detailed messages of a certain ticket.
void ShowMessageInformation(Ticket* tickets, int ticketnumber);

// Create a new ticket by the customer,return >0 sucessful, return 0 unsucessful.
int CreateNewTicket(Ticket* tickets, const int c_number);

// Customer Modifies an active ticket.
void CustomerModifyTicket(Ticket* tickets, const int ticketnumber);

// Agent modifies tickets.
void AgentManageTickets(Ticket* tickets, const int ticketnumber, const Account* account);

// Load all tickets' information.
int loadTickets(Ticket* tickets, const int ticketsize);

// Save all tickets' information.
int saveTicketsInformation(Ticket* tickets, const int ticketsize);

// Load all archived(closed) tickets' information.
Counter loadArchivedTickets(Ticket* tickets, const int ticketsize);

// Save all archived(closed) tickets' information.
int saveArchivedTicketsInformation(Ticket* tickets, const int ticketsize);

// Copy a Ticket structure A from structure B.
void CpTicketAFromB(Ticket* ticketsA, Ticket* ticketsB, int size_tickets);

// Copy a Message structure A from structure B.
void CpMessageAFromB(Message* messageA, Message* messageB, int size_message);
#endif