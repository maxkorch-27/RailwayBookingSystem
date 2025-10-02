#pragma once
#include <string>
#include <vector>
#include "Ticket.h"
using namespace std;

class Customer
{
private:
	string passport;
	string name;
	vector<Ticket*> tickets;
public:
	Customer() = default;
	Customer(string passport, string name, vector<Ticket*> tickets);
	string getPassport() const;
	string getName() const;
	void addTicket(Ticket* ticket);
	void removeTicket(const string& ticketId);
	void showTickets() const;
};