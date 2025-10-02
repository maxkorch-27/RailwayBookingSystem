#include "Seat.h"
#include <string>
#include <vector>
using namespace std;

Seat::Seat(string seatNumber)
{
	number = seatNumber;

}

// Getters and Setters
string Seat::getNumber()
{
	return number;
}
vector<pair<int, int>> Seat::getReservations()
{
	return reservations;
}
bool Seat::isAvailable(int fromIndex, int toIndex)
{
	for (auto& res : reservations)
	{
		if (!(toIndex <= res.first || fromIndex >= res.second))
			return false;
	}
	return true;
}

// Methods to manage reservations
void Seat::addReservation(int fromIndex, int toIndex)
{
	reservations.push_back({ fromIndex, toIndex });
}

void Seat::removeReservation(int fromIndex, int toIndex)
{
	// Find the reservation and remove it
	for (auto it = reservations.begin(); it != reservations.end(); ++it)
	{
		if (it->first == fromIndex && it->second == toIndex)
		{
			reservations.erase(it);
			return;
		}
	}
}