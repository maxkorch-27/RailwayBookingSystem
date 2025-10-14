#include "Ticket.h"
#include <string>
#include <iostream>
#include "EconomyTicket.h"
#include "BusinessTicket.h"
#include "InputValidator.h"
#include "Customer.h"
#include <sstream>
#include "Station.h"
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

bool Ticket::cancel(const string& today)
{
    if (status == "cancelled") {
        cout << "Ticket already cancelled." << endl;
        return false;
    }

    // parse dates
    int d1, m1, y1, d2, m2, y2;
    char sep;
    string tripDate = train->getDate();

    stringstream ss1(today);
    ss1 >> d1 >> sep >> m1 >> sep >> y1;

    stringstream ss2(tripDate);
    ss2 >> d2 >> sep >> m2 >> sep >> y2;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    auto daysFromStart = [&](int d, int m, int y) 
        {
        int total = y * 365;
        for (int i = 0; i < m - 1; ++i)
            total += daysInMonth[i];
        total += d;
        return total;
        };

    int totalToday = daysFromStart(d1, m1, y1);
    int totalTrip = daysFromStart(d2, m2, y2);
    int daysBefore = totalTrip - totalToday;

    if (daysBefore < 0) {
        cout << "Invalid return date. Trip already passed." << endl;
        return false;
    }

    // Calculate penalty
    double penaltyRate = 0.0;
    if (daysBefore >= 30) penaltyRate = 0.01;
    else if (daysBefore >= 15) penaltyRate = 0.05;
    else if (daysBefore >= 3)  penaltyRate = 0.10;
    else penaltyRate = 0.30;

    int refund = static_cast<int>(price * (1.0 - penaltyRate));

    // free seat
    const vector<Station>& route = train->getRoute();
    int fromIndex = -1, toIndex = -1;
    for (int i = 0; i < route.size(); i++) {
        if (route[i].getName() == departureStation.getName()) fromIndex = i;
        if (route[i].getName() == arrivalStation.getName())   toIndex = i;
    }

    if (fromIndex != -1 && toIndex != -1)
        seat->removeReservation(fromIndex, toIndex);

    // mark cancelled
    status = "cancelled";
    moneyReturned = refund;
    customer->removeTicket(id);

    cout << endl << "===== TICKET CANCELLED =====" << endl;
    cout << "Ticket ID: " << id << endl;
    cout << "Days before trip: " << daysBefore << endl;
    cout << "Penalty: " << (penaltyRate * 100) << "%" << endl;
    cout << "Refund: " << refund << " euro" << endl;
    cout << "=============================" << endl;

    return true;
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