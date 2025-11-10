#include <iostream>
#include <string>
#include "BookingSystem.h"
#include "DatabaseManager.h"
#include "InputValidator.h"
#include "ConsoleUI.h"
using namespace std;

const string TRAINS_FILE = "trains.csv";
const string STATIONS_FILE = "stations.csv";
const string COACHES_FILE = "coaches.csv";
const string CUSTOMERS_FILE = "customers.csv";
const string TICKETS_FILE = "tickets.csv";


int main()
{
	// Initialize trains and booking system
	vector<Train> trains = DatabaseManager::loadTrains(TRAINS_FILE, STATIONS_FILE, COACHES_FILE);
	BookingSystem bookingSystem(trains, CUSTOMERS_FILE, TICKETS_FILE);
	ConsoleUI ui(bookingSystem);
	ui.run();
}