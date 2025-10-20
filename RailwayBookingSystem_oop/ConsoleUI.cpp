#include "ConsoleUI.h"
#include "InputValidator.h"
#include <iostream>
#include "DatabaseManager.h"
#include "Menu.h"
using namespace std;

ConsoleUI::ConsoleUI(BookingSystem& bs) : system(bs) {}

void ConsoleUI::run()
{
	string choice;
	cout << "Hello, customer. This is Railway booking system" << endl;
	do
	{
		Menu menu({
			"Search and Buy tickets",
			"My tickets",
			"Return ticket",
			"Cashier report"
			});
		menu.display();
		cout << "Enter your choice: ";
		cin >> choice;
		if (choice == "1") // Search and Buy tickets
		{
			handleSearchAndBuy();
		}
		else if (choice == "2") // My tickets
		{
			handleMyTickets();
		}
		else if (choice == "3") // Return ticket
		{
			handleReturnTicket();
		}
		else if (choice == "4") // Cashier report
		{
			handleCashierReport();
		}
		else if (choice != "0")
		{
			cout << "Invalid choice. Please try again." << endl;
		}
	} while (choice != "0");
	system.saveAll();
	cout << "Thank you for using the Railway booking system. Goodbye!" << endl;
}

void ConsoleUI::handleSearchAndBuy()
{
	vector<string> stations = DatabaseManager::getAllStations(system.getTrains());
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
	system.searchTickets(fromStation, toStation, date, coachType); // Search for tickets
}

void ConsoleUI::handleMyTickets()
{
	// Assuming passport number is used to identify customers
	string passport = InputValidator::getValidatedInput("Enter your passport (6 digits): ",
		InputValidator::isValidPassport,
		"Invalid passport format. Please enter a 6-digit number.");

	system.searchCustomerTickets(passport); // Display customer's tickets
}

void ConsoleUI::handleReturnTicket()
{
	string ticketId = InputValidator::getValidatedInput("Enter your ticket ID: ",
		InputValidator::isValidTicketId,
		"Invalid ticket ID format. Please use the format TXXXXXX where X are digits.");

	string today = InputValidator::getValidatedInput("Enter today's date (DD-MM-YYYY): ",
		InputValidator::isValidDate,
		"Invalid date format. Please use DD-MM-YYYY.");

	system.returnTicket(ticketId, today); // Process ticket return
}

void ConsoleUI::handleCashierReport()
{
	system.cashierReport();
}