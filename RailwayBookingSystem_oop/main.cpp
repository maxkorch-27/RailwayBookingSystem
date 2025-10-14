#include <iostream>
#include <string>
#include "BookingSystem.h"
#include "TrainsData.h"
#include "InputValidator.h"
#include "ConsoleUI.h"
using namespace std;


int main()
{
	// Initialize trains and booking system
	vector<Train> trains = loadTrainsFromCSV("trains.csv", "stations.csv", "coaches.csv");
	BookingSystem bookingSystem(trains, {});
	ConsoleUI ui(bookingSystem);
	ui.run();
}