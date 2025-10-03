#include "Coach.h"
#include <string>
#include <vector>
using namespace std;

Coach::Coach(string num, string coachType, vector<Seat> s)
{
	number = num;
	type = coachType;
	seats = s;
}

// Getters
string Coach::getNumber()
{
	return number;
}
string Coach::getType()
{
	return type;
}
vector<Seat>& Coach::getSeats()
{
	return seats;
}

void Coach::addSeat(Seat seat)
{
	seats.push_back(seat);
}