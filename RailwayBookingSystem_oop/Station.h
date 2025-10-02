#pragma once
#include <string>
using namespace std;

class Station
{
private:
	string name;
	string arrivalTime;
	string departureTime;
public:
	Station() = default;
	Station(string name, string arrivalTime, string departureTime);
	string getName() const;
	string getArrivalTime() const;
	string getDepartureTime() const;
};