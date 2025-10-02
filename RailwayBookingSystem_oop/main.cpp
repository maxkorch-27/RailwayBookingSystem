#include <iostream>
#include <string>
#include "BookingSystem.h"
#include "TrainsData.h"
using namespace std;

int main()
{
	// Initialize trains and booking system
	vector<Train> trains = createTrains();
	vector<string> stations = getAllStations(trains);
	BookingSystem bookingSystem(trains, {});

	// Main menu loop
	string choice;
	cout << "Hello, customer. This is Railway booking system" << endl;
	do
	{
		cout << endl << "====== MENU  ======" << endl;
		cout << "1. Searching and Buying tickets" << endl;
		cout << "2. My tickets" << endl;
		cout << "3. Return ticket" << endl;
		cout << "4. Cashier report" << endl;
		cout << "0. Exit" << endl << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		if (choice == "1") // Search and Buy tickets
		{
			cout << "Available stations: " << endl; // Display all available stations
			for (auto& station : stations)
				cout << station << " ";
			cout << endl;
			string fromStation, toStation, date, coachType;
			cout << "Enter departure station: ";
			cin >> fromStation;
			cout << "Enter arrival station: ";
			cin >> toStation;
			cout << "Enter date (DD-MM-YYYY): ";
			cin >> date;
			cout << "Enter coach type (Economy/Business): ";
			cin >> coachType;

			bookingSystem.searchTickets(fromStation, toStation, date, coachType); // Search for tickets
		}
		else if (choice == "2") // My tickets
		{
			// Assuming passport number is used to identify customers
			cout << "Enter your passport number: ";
			string passport;
			cin >> passport;

			bookingSystem.searchCustomerTickets(passport); // Display customer's tickets
		}
		else if (choice == "3") // Return ticket
		{
			// Assuming ticket ID is used to identify tickets
			cout << "Enter your ticket ID: ";
			string ticketId;
			cin >> ticketId;

			bookingSystem.returnTicket(ticketId); // Process ticket return
		}
		else if (choice == "4") // Cashier report
		{
			// Display sales report
			bookingSystem.cashierReport();
		}
		else if (choice == "0") // Exit
		{
			cout << "Exiting the system. Goodbye!" << endl;
		}
		else
		{
			cout << "Invalid choice. Please try again: " << endl;
		}
	} while (choice != "0");
	return 0;
}