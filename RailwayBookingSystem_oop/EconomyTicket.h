#pragma once
#include "Ticket.h"

class EconomyTicket : public Ticket
{
public:
	EconomyTicket() = default;
	EconomyTicket(string id, Train* train, Customer* customer, Station departureStation, Station arrivalStation, Coach* coach, Seat* seat, int price, string status, int moneyReturned)
		: Ticket(id, train, customer, departureStation, arrivalStation, coach, seat, price, status, moneyReturned) {
	}

	int calculatePrice(int routeIndex) const override
	{
		int price = 10 + 5 * routeIndex; // Base price + 5 euro per station
		return price;
	}
};
