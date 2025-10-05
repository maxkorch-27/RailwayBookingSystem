#include <iostream>
#include <string>
#include "BookingSystem.h"
#include "TrainsData.h"
#include "InputValidator.h"
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

			fromStation = InputValidator::getValidatedInput("Enter departure station: ",
				[&stations](const string& input) { return InputValidator::isValidStation(input, stations); },
				"No such station in system. Please try again.");
			toStation = InputValidator::getValidatedInput("Enter arrival station: ",
				[&stations](const string& input) { return InputValidator::isValidStation(input, stations); },
				"No such station in system. Please try again.");
			date = InputValidator::getValidatedInput("Enter travel date (DD-MM-YYYY): ",
				InputValidator::isValidDate,
				"Invalid date format. Please use DD-MM-YYYY.");
			coachType = InputValidator::getValidatedInput("Enter coach type (Economy/Business): ",
				InputValidator::isValidCoachType,
				"Invalid coach type. Please enter 'Economy' or 'Business'.");

			bookingSystem.searchTickets(fromStation, toStation, date, coachType); // Search for tickets
		}
		else if (choice == "2") // My tickets
		{
			// Assuming passport number is used to identify customers
			string passport=InputValidator::getValidatedInput("Enter your passport (6 digits): ",
				InputValidator::isValidPassport,
				"Invalid passport format. Please enter a 6-digit number.");

			bookingSystem.searchCustomerTickets(passport); // Display customer's tickets
		}
		else if (choice == "3") // Return ticket
		{
			// Assuming ticket ID is used to identify tickets
			string ticketId = InputValidator::getValidatedInput("Enter your ticket ID: ",
				InputValidator::isValidTicketId,
				"Invalid ticket ID format. Please use correct ticket ID.");

			string todayDate = InputValidator::getValidatedInput("Enter today's date (DD-MM-YYYY): ",
				InputValidator::isValidDate,
				"Invalid date format. Please use DD-MM-YYYY.");

			bookingSystem.returnTicket(ticketId, todayDate); // Process ticket return
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