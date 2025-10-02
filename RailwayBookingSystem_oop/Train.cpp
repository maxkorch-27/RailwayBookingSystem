#include "Train.h"
#include <string>

Train::Train(string num, string date1, vector<Station> route1, vector<Coach> coaches1)
{
	number = num;
	date = date1;
	route = route1;
	coaches = coaches1;
}

// Getters
string Train::getNumber()
{
	return number;
}
string Train::getDate()
{
	return date;
}
const vector<Station>& Train::getRoute() const
{
	return route;
}

vector<Coach>& Train::getCoaches()
{
	return coaches;
}