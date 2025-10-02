#include "Ticket.h"
#include <string>
using namespace std;
Ticket::Ticket(string id1, Train* train1, Customer* customer1, Station departureStation1, Station arrivalStation1, Coach* coach1, Seat* seat1, int price1, string status1, int moneyReturned1)
{
	id = id1;
	train = train1;
	customer = customer1;
	departureStation = departureStation1;
	arrivalStation = arrivalStation1;
	coach = coach1;
	seat = seat1;
	price = price1;
	status = status1;
	moneyReturned = moneyReturned1;
}

// Getters and Setters
string Ticket::getId() const
{
	return id;
}
Train* Ticket::getTrain() const
{
	return train;
}
Customer* Ticket::getCustomer() const
{
	return customer;
}
Station Ticket::getDepartureStation() const
{
	return departureStation;
}
Station Ticket::getArrivalStation() const
{
	return arrivalStation;
}
Coach* Ticket::getCoach() const
{
	return coach;
}
Seat* Ticket::getSeat() const
{
	return seat;
}
int Ticket::getPrice() const
{
	return price;
}
string Ticket::getStatus() const
{
	return status;
}
void Ticket::setStatus(const string& newStatus)
{
	status = newStatus;
}
int Ticket::getMoneyReturned() const
{
	return moneyReturned;
}
void Ticket::setMoneyReturned(int amount)
{
	moneyReturned = amount;
}
void Ticket::setPrice(int amount)
{
	price = amount;
}