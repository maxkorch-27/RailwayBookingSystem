#include "BookingSystem.h"
#include "EconomyTicket.h"
#include "BusinessTicket.h"
#include <iostream>
#include <string>
using namespace std;

BookingSystem::BookingSystem(vector<Train> trains1, vector<Customer> customers1)
{
    trains = std::move(trains1);
    customers = std::move(customers1);
    // tickets starts empty
}

void BookingSystem::buyTicket(Train& train, Station fromStation, Station toStation, Coach& coach, Seat& seat, int routeIndex)
{
    cout << "===== buying process =====" << endl << endl;
    string passportNumber;
    cout << "Enter your passport number: ";
    cin >> passportNumber;

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
        string name;
        cout << "Enter your name: ";
        cin >> name;

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


void BookingSystem::searchTickets(string fromStation, string toStation, string date, string coachType)
{
    cout << "=== Available options ===" << endl << endl;

    bool anyFound = false;

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
        cout << endl << "Do you want to book a ticket? (yes/no)" << endl;
        string response;
        cin >> response;

        if (response == "yes")
        {
            // Prompt for train number choice
            cout << "Enter train number" << endl;
            string trainNumber;
            cin >> trainNumber;
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

void BookingSystem::returnTicket(const string& ticketId) {
    // Search for ticket by ID
    for (auto it = tickets.begin(); it != tickets.end(); ++it) {
        if ((*it)->getId() == ticketId) {
            Ticket& ticket = *(*it);
            if (ticket.getStatus() == "cancelled") {
                cout << "Ticket already cancelled." << endl;
                return;
            }

            // Calculate penalty and refund
            int daysBefore;
            //Check for valid input
            while (true) {
                cout << "Enter number of days before trip: ";
                cin >> daysBefore;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');// Clear error state
                    cout << "Invalid input! Please enter a number." << endl;
                }
                else {
                    if (daysBefore < 0) {
                        cout << "Number of days cannot be negative. Please try again." << endl;
                        continue;
                    }
                    break;
                }
            }
            double penaltyRate = 0.0;
            if (daysBefore >= 30) penaltyRate = 0.01;
            else if (daysBefore >= 15) penaltyRate = 0.05;
            else if (daysBefore >= 3)  penaltyRate = 0.10;
            else penaltyRate = 0.30;

            int refund = static_cast<int>(ticket.getPrice() * (1.0 - penaltyRate));

            // Free up the reserved seat
            const vector<Station>& route = ticket.getTrain()->getRoute();
            int fromIndex = -1, toIndex = -1;
            for (int i = 0; i < route.size(); i++) {
                if (route[i].getName() == ticket.getDepartureStation().getName()) fromIndex = i;
                if (route[i].getName() == ticket.getArrivalStation().getName())   toIndex = i;
            }
            if (fromIndex != -1 && toIndex != -1) {
                for (auto& coach : ticket.getTrain()->getCoaches()) {
                    if (coach.getNumber() == ticket.getCoach()->getNumber()) {
                        for (auto& seat : coach.getSeats()) {
                            if (seat.getNumber() == ticket.getSeat()->getNumber()) {
                                seat.removeReservation(fromIndex, toIndex);
                                break;
                            }
                        }
                    }
                }
            }
            // Remove ticket from customer's list
            Customer* cust = ticket.getCustomer();
            if (cust)
                cust->removeTicket(ticketId);
            ticket.setStatus("cancelled");
            ticket.setMoneyReturned(refund);
            // Display cancellation details
            cout << endl << "Ticket " << ticketId << " cancelled." << endl;
            cout << "Penalty: " << (penaltyRate * 100) << "%" << endl;
            cout << "Refund: " << refund << " euro" << endl;
            return;
        }
    }
    // If ticket not found
    cout << "Ticket with ID " << ticketId << " not found." << endl;
}

void BookingSystem::cashierReport()
{
    int totalSold = 0;
    int totalPrice = 0;
    int totalCancelled = 0;
    int totalRefund = 0;
    int totalRevenue = 0;
    // Calculate totals for sold tickets, cancellations, refunds, and revenue
    for (auto& ticket : tickets)
    {
        if (ticket->getStatus() == "booked")
        {
            totalSold++;
            totalPrice += ticket->getPrice();
        }
        else if (ticket->getStatus() == "cancelled")
        {
            totalSold++;
            totalCancelled++;
            totalPrice += ticket->getPrice();
            totalRefund += ticket->getMoneyReturned();
        }
    }
    totalRevenue = totalPrice - totalRefund;
    cout << endl << "===== Cashier Report =====" << endl;
    cout << "Total tickets sold: " << totalSold << endl;
    cout << "Total sales amount: " << totalPrice << " euro" << endl;
    cout << "Total tickets cancelled: " << totalCancelled << endl;
    cout << "Total refund amount: " << totalRefund << " euro" << endl;
    cout << "TOTAL REVENUE: " << totalRevenue << " euro" << endl;
    cout << "==========================" << endl;
}