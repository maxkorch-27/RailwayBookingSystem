#include <iostream>
#include <string>
#include "BookingSystem.h"
#include "DatabaseManager.h"
#include "InputValidator.h"
#include "ConsoleUI.h"
using namespace std;


int main()
{
	// Initialize trains and booking system
	vector<Train> trains = DatabaseManager::loadTrainsFromCSV("trains.csv", "stations.csv", "coaches.csv");
	BookingSystem bookingSystem(trains, "customers.csv", "tickets.csv");
	ConsoleUI ui(bookingSystem);
	ui.run();
}