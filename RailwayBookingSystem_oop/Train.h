#pragma once
#include <string>
#include <vector>
#include "Station.h"
#include "Coach.h"

using namespace std;
class Train
{
private:
	string number;
	string date;
	vector<Station> route;
	vector<Coach> coaches;
public:
	Train() = default;
	Train(string number, string date, vector<Station> route, vector<Coach> coaches);
	string getNumber();
	string getDate();
	const vector<Station>& getRoute() const;
	vector <Coach>& getCoaches();
};