#include <iostream>
#include "Customer.h"
#include <string>
#include <vector>
#include "Ticket.h"
#include "PerformanceTimer.h"
using namespace std;

Customer::Customer(string passport1, string name1, vector<Ticket*> tickets1)
{
	passport = passport1;
	name = name1;
	tickets = tickets1;
}

// Getters
string Customer::getPassport() const
{
	return passport;
}
string Customer::getName() const
{
	return name;
}
void Customer::addTicket(Ticket* ticket)
{
	tickets.push_back(ticket);
}
void Customer::showTickets() const
{
	PerformanceTimer timer("Show Customer Tickets");

	// Display all tickets for this customer
	if (tickets.empty())
	{
		cout << "No tickets booked." << endl;
		return;
	}
	cout << "Your tickets:" << endl << "==============";
	for (auto& ticket : tickets)
	{
		if (ticket->getStatus() != "cancelled")
			ticket->display();
	}
}

void Customer::removeTicket(const string& ticketId)
{
	// Remove ticket with the given ID from the customer's tickets
	for (auto it = tickets.begin(); it != tickets.end(); ++it)
	{
		if ((*it)->getId() == ticketId)
		{
			tickets.erase(it);
			return;
		}
	}
}