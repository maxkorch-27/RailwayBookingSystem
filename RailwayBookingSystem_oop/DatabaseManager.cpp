#include "DatabaseManager.h"
#include "Station.h"
#include "Coach.h"
#include "Seat.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iostream>
#include <unordered_map>
#include "Ticket.h"
#include "EconomyTicket.h"
#include "BusinessTicket.h"
using namespace std;

vector<Train> DatabaseManager::loadTrainsFromCSV(const string& trainsFile, const string& stationsFile, const string& coachesFile)
{
    vector<Train> trains;
    unordered_map<string, vector<Station>> trainStations;
    unordered_map<string, vector<Coach>> trainCoaches;

    // Load stations
    ifstream stFile(stationsFile);
    string line;
    getline(stFile, line);
    while (getline(stFile, line))
    {
        stringstream ss(line);
        string trainNum, name, arr, dep;
        getline(ss, trainNum, ',');
        getline(ss, name, ',');
        getline(ss, arr, ',');
        getline(ss, dep, ',');
        trainStations[trainNum].emplace_back(name, arr, dep);
    }

    // Load coaches
    ifstream cFile(coachesFile);
    getline(cFile, line);
    while (getline(cFile, line))
    {
        stringstream ss(line);
        string trainNum, coachNum, type;
        getline(ss, trainNum, ',');
        getline(ss, coachNum, ',');
        getline(ss, type, ',');

        vector<Seat> seats;
        for (int i = 1; i <= 10; ++i)
            seats.emplace_back(to_string(i));
        trainCoaches[trainNum].emplace_back(coachNum, type, seats);
    }

    // Load trains
    ifstream tFile(trainsFile);
    getline(tFile, line);
    while (getline(tFile, line))
    {
        stringstream ss(line);
        string number, date, route;
        getline(ss, number, ',');
        getline(ss, date, ',');
        getline(ss, route, ',');
        trains.emplace_back(number, date, trainStations[number], trainCoaches[number]);
    }

    return trains;
}


vector<string> DatabaseManager::getAllStations(const vector<Train>& trains)
{
    set<string> uniqueStations;
    for (auto& train : trains)
    {
        for (auto& station : train.getRoute())
        {
            uniqueStations.insert(station.getName());
        }
    }
    return vector<string>(uniqueStations.begin(), uniqueStations.end());
}

vector<Customer> DatabaseManager::loadCustomers(const string& filename) {
    vector<Customer> customers;
    ifstream file(filename);
    if (!file.is_open()) return customers;

    string line;
    getline(file, line); // skip header if present
    while (getline(file, line)) {
        stringstream ss(line);
        string passport, name;
        getline(ss, passport, ',');
        getline(ss, name, ',');
        if (!passport.empty() && !name.empty())
            customers.emplace_back(passport, name, vector<Ticket*>{});
    }
    return customers;
}

void DatabaseManager::saveCustomers(const string& filename, const vector<Customer>& customers) {
    ofstream file(filename);
    if (!file.is_open()) return;
    file << "passport,name\n";
    for (auto& c : customers)
        file << c.getPassport() << "," << c.getName() << "\n";
}

vector<unique_ptr<Ticket>> DatabaseManager::loadTickets(
    const string& filename, vector<Customer>& customers, vector<Train>& trains)
{
    vector<unique_ptr<Ticket>> tickets;
    ifstream file(filename);
    if (!file.is_open()) return tickets;

    string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, passport, trainNum, trainDate;
        string from, to, depTime, arrTime, coachNum, seatNum, type, priceStr, refundStr, status;

        getline(ss, id, ',');
        getline(ss, passport, ',');
        getline(ss, trainNum, ',');
        getline(ss, trainDate, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, depTime, ',');
        getline(ss, arrTime, ',');
        getline(ss, coachNum, ',');
        getline(ss, seatNum, ',');
        getline(ss, type, ',');
        getline(ss, priceStr, ',');
        getline(ss, refundStr, ',');
        getline(ss, status, ',');

        int price = stoi(priceStr);
        int refund = stoi(refundStr);

        Train* trainPtr = nullptr;
        for (auto& t : trains)
            if (t.getNumber() == trainNum && t.getDate() == trainDate)
                trainPtr = &t;

        if (!trainPtr) continue;

        Coach* coachPtr = nullptr;
        Seat* seatPtr = nullptr;
        for (auto& coach : trainPtr->getCoaches()) {
            if (coach.getNumber() == coachNum) {
                coachPtr = &coach;
                for (auto& seat : coach.getSeats())
                    if (seat.getNumber() == seatNum)
                        seatPtr = &seat;
            }
        }
        if (!coachPtr || !seatPtr) continue;

        Customer* custPtr = nullptr;
        for (auto& c : customers)
            if (c.getPassport() == passport)
                custPtr = &c;

        Station fromStation(from, depTime, depTime);
        Station toStation(to, arrTime, arrTime);

        unique_ptr<Ticket> ticket;
        if (type == "Business")
            ticket = make_unique<BusinessTicket>(id, trainPtr, custPtr, fromStation, toStation, coachPtr, seatPtr, price, status, refund);
        else
            ticket = make_unique<EconomyTicket>(id, trainPtr, custPtr, fromStation, toStation, coachPtr, seatPtr, price, status, refund);

        ticket->setCustomerPassport(passport);
        if (custPtr) custPtr->addTicket(ticket.get());

        if (status != "cancelled") {
            const vector<Station>& route = trainPtr->getRoute();
            int fromIndex = -1, toIndex = -1;
            for (int i = 0; i < route.size(); i++) {
                if (route[i].getName() == from) fromIndex = i;
                if (route[i].getName() == to) toIndex = i;
            }
            if (fromIndex != -1 && toIndex != -1)
                seatPtr->addReservation(fromIndex, toIndex);
        }

        tickets.push_back(move(ticket));
    }

    return tickets;
}



void DatabaseManager::saveTickets(const string& filename, const vector<unique_ptr<Ticket>>& tickets)
{
    ofstream file(filename);
    if (!file.is_open()) return;

    file << "id,passport,trainNum,trainDate,from,to,fromTime,toTime,coachNum,seatNum,type,price,moneyReturned,status\n";

    for (const auto& t : tickets)
    {
        file << t->getId() << ","
            << t->getCustomerPassport() << ","
            << t->getTrain()->getNumber() << ","
            << t->getTrain()->getDate() << ","
            << t->getDepartureStation().getName() << ","
            << t->getArrivalStation().getName() << ","
            << t->getDepartureStation().getDepartureTime() << ","
            << t->getArrivalStation().getArrivalTime() << ","
            << t->getCoach()->getNumber() << ","
            << t->getSeat()->getNumber() << ","
            << (dynamic_cast<BusinessTicket*>(t.get()) ? "Business" : "Economy") << ","
            << t->getPrice() << ","
            << t->getMoneyReturned() << ","
            << t->getStatus() << "\n";
    }
}
