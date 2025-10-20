#pragma once
#include <string>
#include <vector>
#include "Seat.h"
using namespace std;

class Coach
{
private:
	string number;
	string type;
	vector<Seat> seats;
public:
	Coach() = default;
	Coach(string number, string coachType, vector<Seat> seats);
	string getNumber();
	string getType();
	vector<Seat>& getSeats();
};