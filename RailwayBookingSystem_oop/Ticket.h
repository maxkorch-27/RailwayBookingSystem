#pragma once
#include <string>
#include "Train.h"
#include "Coach.h"
#include "Station.h"
#include "Seat.h"
#include "IDisplay.h"
using namespace std;

class Customer;

class Ticket : public IDisplay
{
protected:
	string id;
	Train* train;
	Customer* customer;
	Station departureStation;
	Station arrivalStation;
	Coach* coach;
	Seat* seat;
	int price;
	string status; // booked, cancelled
	int moneyReturned;
public:
	Ticket() = default;
	Ticket(string id, Train* train, Customer* customer, Station departureStation, Station arrivalStation, Coach* coach, Seat* seat, int price, string status, int moneyReturned);

	virtual ~Ticket() = default;

	string getId() const;
	Train* getTrain() const;
	Customer* getCustomer() const;
	Station getDepartureStation() const;
	Station getArrivalStation() const;
	Coach* getCoach() const;
	Seat* getSeat() const;
	int getPrice() const;
	string getStatus() const;
	void setStatus(const string& newStatus);
	int getMoneyReturned() const;
	void setMoneyReturned(int amount);
	void setPrice(int amount);
	virtual int calculatePrice(int routeIndex) const = 0;
	void display() const override;
};