#pragma once
#include <string>
#include <vector>
using namespace std;

class Seat
{
private:
	string number;
	vector<pair<int, int>> reservations; // Each pair represents a reservation with (fromIndex, toIndex)
public:
	Seat() = default;
	Seat(string seatNumber);
	string getNumber();
	vector<pair<int, int>> getReservations();
	bool isAvailable(int fromIndex, int toIndex);
	void addReservation(int fromIndex, int toIndex);
	void removeReservation(int fromIndex, int toIndex);
};

