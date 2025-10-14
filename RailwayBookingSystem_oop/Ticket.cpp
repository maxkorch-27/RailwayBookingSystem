#include "Ticket.h"
#include <string>
#include <iostream>
#include "EconomyTicket.h"
#include "BusinessTicket.h"
#include "InputValidator.h"
#include "Customer.h"
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

void Ticket::create(vector<Customer>& customers, vector <unique_ptr<Ticket>>& tickets, Train& train, Station fromStation, Station toStation, Coach& coach, Seat& seat, int routeIndex)
{
    cout << "===== Ticket Booking =====" << endl << endl;
    string passportNumber = InputValidator::getValidatedInput("Enter your passport number (6 digits): ",
        InputValidator::isValidPassport,
        "Invalid passport format. Please enter a 6-digit number.");

    string ticketId = "T0" + to_string(tickets.size() + 1) + passportNumber;

    bool isNewCustomer = true;
    Customer* custPtr = nullptr;

    // Check if customer already exists
    for (auto& customer : customers) {
        if (customer.getPassport() == passportNumber)
        {
            isNewCustomer = false;
            cout << "Welcome back, " << customer.getName() << "!" << endl << endl;
            custPtr = &customer;
            break;
        }
    }

    // If new customer, get name and add to customers list
    if (isNewCustomer) {
        string name = InputValidator::getValidatedInput("Enter your name: ",
            InputValidator::isValidName, "Incorrect name");
        cout << "Welcome, " << name << "!" << endl << endl;

        customers.emplace_back(passportNumber, name, vector<Ticket*>{});
        custPtr = &customers.back();
    }

    // create proper concrete Ticket and push into unique_ptr vector
    unique_ptr<Ticket> uptr;
    if (coach.getType() == "Business") {
        uptr = make_unique<BusinessTicket>(ticketId, &train, custPtr, fromStation, toStation, &coach, &seat, 0, "booked", 0);
    }
    else {
        uptr = make_unique<EconomyTicket>(ticketId, &train, custPtr, fromStation, toStation, &coach, &seat, 0, "booked", 0);
    }

    // compute price via polymorphic call
    int price = uptr->calculatePrice(routeIndex);
    uptr->setPrice(price);

    // keep non-owning pointer for customer and move ownership into system vector
    Ticket* rawPtr = uptr.get();
    tickets.push_back(move(uptr));
    custPtr->addTicket(rawPtr);
    cout << "TICKET BOOKED!" << endl;
    rawPtr->display();
}

void Ticket::display() const
{
	cout << endl << "=============================" << endl;
	cout << "Ticket " << id<< endl;
	cout << "Train: " << train->getNumber() << " on " << train->getDate() << endl;
	cout << "From: " << departureStation.getName() << " (" << departureStation.getDepartureTime() << ")" << endl;
	cout << "To: " << arrivalStation.getName() << " (" << arrivalStation.getArrivalTime() << ")" << endl;
	cout << "Coach: " << coach->getNumber() << " (" << coach->getType() << ")" << endl;
	cout << "Seat: " << seat->getNumber() << endl;
	cout << "Price: " << price << " euro" << endl;
	cout << "=============================" << endl << endl;
}