#pragma once
#include "Ticket.h"

class BusinessTicket : public Ticket
{
public:
	BusinessTicket() = default;
	BusinessTicket(string id, Train* train, Customer* customer, Station departureStation, Station arrivalStation, Coach* coach, Seat* seat, int price, string status, int moneyReturned)
		: Ticket(id, train, customer, departureStation, arrivalStation, coach, seat, price, status, moneyReturned) {
	}

	int calculatePrice(int routeIndex) const override
	{
		int price = 20 + 5 * routeIndex;
		return price;
	}
};