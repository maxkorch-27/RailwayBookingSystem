#pragma once
#include <vector>
#include "Train.h"
using namespace std;

// Function to create and return a list of trains with their routes and coaches
vector<Train> createTrains();

// Function to get a list of all unique stations from the list of trains
vector<string> getAllStations(const vector<Train>& trains);