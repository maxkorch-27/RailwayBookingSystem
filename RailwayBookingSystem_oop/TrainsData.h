#pragma once
#include <vector>
#include "Train.h"
using namespace std;

vector<Train> loadTrainsFromCSV(const string& trainsFile, const string& stationsFile, const string& coachesFile);
vector<string> getAllStations(const vector<Train>& trains);
