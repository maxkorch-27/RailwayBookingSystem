#include "BookingSystem.h"
#include "EconomyTicket.h"
#include "BusinessTicket.h"
#include "Report.h"
#include <iostream>
#include <string>
#include <sstream>
#include "InputValidator.h"
#include "DatabaseManager.h"
using namespace std;

BookingSystem::BookingSystem(vector<Train> trains1, const string& customersFile, const string& ticketsFile)
{
    trains = std::move(trains1);
    try {
        customers = DatabaseManager::loadCustomers(customersFile);
    }
    catch (const exception& e) {
        cerr << "Error loading customers: " << e.what() << endl;
    }

    try {
    tickets = DatabaseManager::loadTickets(ticketsFile, customers, trains);
    }
    catch (const exception& e) {
        cerr << "Error loading tickets: " << e.what() << endl;
    }
}

vector<Train>& BookingSystem::getTrains()
{
    return trains;
}

void BookingSystem::buyTicket(Train& train, Station fromStation, Station toStation, Coach& coach, Seat& seat, int routeIndex)
{
    Ticket::create(customers, tickets, train, fromStation, toStation, coach, seat, routeIndex);
}

void BookingSystem::searchTickets(string fromStation, string toStation, string date, string coachType)
{
    cout << "=== Available options ===" << endl << endl;

    bool anyFound = false;

	vector <string> availableTrainNumbers;
    // Search for trains matching criteria
    for (auto& train : trains) {
        if (train.getDate() != date) continue;

        const vector<Station>& route = train.getRoute();
        int fromIndex = -1, toIndex = -1;
        for (int i = 0; i < route.size(); i++) {
            if (route[i].getName() == fromStation) fromIndex = i;
            if (route[i].getName() == toStation)   toIndex = i;
        }
        if (fromIndex == -1 || toIndex == -1 || fromIndex >= toIndex) continue;

        vector<pair<int, vector<string>>> coachOptions;

        vector<Coach>& coaches = train.getCoaches();
        for (int c = 0; c < coaches.size(); c++) {
            auto& coach = coaches[c];
            if (coach.getType() != coachType) continue;

            vector<string> availableSeats;
            for (auto& seat : coach.getSeats())
                if (seat.isAvailable(fromIndex, toIndex))
                    availableSeats.push_back(seat.getNumber());

            if (!availableSeats.empty())
                coachOptions.push_back({ c + 1, availableSeats });
        }

        // Display available options
        if (!coachOptions.empty()) {
			availableTrainNumbers.push_back(train.getNumber());
            cout << "Train " << train.getNumber() << ": "
                << route[fromIndex].getName() << " (" << route[fromIndex].getDepartureTime() << ")"
                << " --> "
                << route[toIndex].getName() << " (" << route[toIndex].getArrivalTime() << ")"
                << endl;

            cout << "Seats available:" << endl;
            for (auto& co : coachOptions) {
                cout << "  Coach " << co.first << ": ";
                for (int i = 0; i < co.second.size(); i++) {
                    cout << co.second[i];
                    if (i < co.second.size() - 1) cout << ", ";
                }
                cout << endl;
            }
            cout << endl;
            anyFound = true;
        }
    }

    // If options found, prompt for booking
    if (anyFound) {
        string response = InputValidator::getValidatedInput("Do you want to book a ticket? (yes/no): ",
            InputValidator::isValidYesNo, "Please enter yes or no!");

        if (response == "yes")
        {
            // Prompt for train number choice
            string trainNumber = InputValidator::getValidatedInput("Enter train number from the above options: ",
                [&availableTrainNumbers](const string& input) { return InputValidator::isValidTrainNumber(input, availableTrainNumbers); },
				"Invalid train number. Please choose from the available options.");
            // Auto-assign first available seat in chosen train
            for (auto& train : trains) {
                if (train.getDate() != date || train.getNumber() != trainNumber) continue;
                const vector<Station>& route = train.getRoute();
                int fromIndex = -1, toIndex = -1;
                for (int i = 0; i < route.size(); i++) {
                    if (route[i].getName() == fromStation) fromIndex = i;
                    if (route[i].getName() == toStation)   toIndex = i;
                }
                if (fromIndex == -1 || toIndex == -1 || fromIndex >= toIndex) continue;

                for (auto& coach : train.getCoaches()) {
                    for (auto& seat : coach.getSeats()) {
                        if (coach.getType() == coachType && seat.isAvailable(fromIndex, toIndex)) {
                            cout << "SEAT (" << seat.getNumber()
                                << ") in COACH (" << coach.getNumber()
                                << ") has been automatically chosen." << endl;
                            cout << "Finish your booking to get a ticket" << endl;
                            buyTicket(train, route[fromIndex], route[toIndex], coach, seat, toIndex - fromIndex);
                            seat.addReservation(fromIndex, toIndex);
                            return;
                        }
                    }
                }
            }
        }
    }
    else {
        //If no options found
        cout << "No available options found." << endl;
    }
}

void BookingSystem::searchCustomerTickets(const string& passport)
{
    // Search for customer by passport and display their tickets
    for (auto& customer : customers) {
        if (customer.getPassport() == passport) {
            customer.showTickets();
            return;
        }
    }
    cout << "Customer with passport " << passport << " not found." << endl;
}

int daysBetween(const string& today, const string& trip)
{
    int d1, m1, y1;
    int d2, m2, y2;
    char sep;

    stringstream ss1(today);
    ss1 >> d1 >> sep >> m1 >> sep >> y1;

    stringstream ss2(trip);
    ss2 >> d2 >> sep >> m2 >> sep >> y2;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    auto daysFromStart = [&](int d, int m, int y) {
        int total = y * 365;
        for (int i = 0; i < m - 1; ++i)
            total += daysInMonth[i];
        total += d;
        return total;
        };

    int totalToday = daysFromStart(d1, m1, y1);
    int totalTrip = daysFromStart(d2, m2, y2);

    if (totalTrip >= totalToday)
        return totalTrip - totalToday;
    else
        return -1; // Invalid case: trip date is before today's date
}


void BookingSystem::returnTicket(const string& ticketId, string today) 
{
    for (auto& uptr : tickets)
    {
        if (uptr->getId() == ticketId)
        {
			uptr->cancel(today);
            return;
        }
    }
	cout << "Ticket with ID " << ticketId << " not found." << endl;
}

void BookingSystem::cashierReport()
{
	Report::generateReport(tickets);
}

void BookingSystem::saveAll()
{
    try { DatabaseManager::saveTickets("tickets.csv", tickets); }
    catch (const std::exception& e) { cerr << "saveTickets failed: " << e.what() << endl; }
    try { DatabaseManager::saveCustomers("customers.csv", customers); }
	catch (const std::exception& e) { cerr << "saveCustomers failed: " << e.what() << endl; }
}