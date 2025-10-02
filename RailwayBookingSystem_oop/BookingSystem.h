#pragma once
#include "Customer.h"
#include "Ticket.h"
#include "Train.h"
#include <vector>
#include <memory>
using namespace std;

class BookingSystem
{
private:
    vector<Train> trains;
    vector<Customer> customers;
    // BookingSystem owns all tickets
    vector<unique_ptr<Ticket>> tickets;

public:
    BookingSystem(vector<Train> trains1, vector<Customer> customers1);
    void searchTickets(string fromStation, string toStation, string date, string coachType);
    void buyTicket(Train& train, Station fromStation, Station toStation, Coach& coach, Seat& seat, int routeIndex);
    void returnTicket(const string& ticketId);
    void cashierReport();
    void searchCustomerTickets(const string& passport);
};
