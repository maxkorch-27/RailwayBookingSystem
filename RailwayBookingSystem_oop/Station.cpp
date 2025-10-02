#include "Station.h"
#include <string>
using namespace std;
Station::Station(string n, string arrTime, string depTime)
{
	name = n;
	arrivalTime = arrTime;
	departureTime = depTime;
}

// Getters
string Station::getName() const
{
	return name;
}
string Station::getArrivalTime() const
{
	return arrivalTime;
}
string Station::getDepartureTime() const
{
	return departureTime;
}