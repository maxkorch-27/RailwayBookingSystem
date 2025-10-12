#include "TrainsData.h"
#include "Station.h"
#include "Coach.h"
#include "Seat.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iostream>
#include <unordered_map>
using namespace std;

vector<Train> loadTrainsFromCSV(const string& trainsFile, const string& stationsFile, const string& coachesFile)
{
    vector<Train> trains;
    unordered_map<string, vector<Station>> trainStations;
    unordered_map<string, vector<Coach>> trainCoaches;

    // Load stations
    ifstream stFile(stationsFile);
    string line;
    getline(stFile, line);
    while (getline(stFile, line))
    {
        stringstream ss(line);
        string trainNum, name, arr, dep;
        getline(ss, trainNum, ',');
        getline(ss, name, ',');
        getline(ss, arr, ',');
        getline(ss, dep, ',');
        trainStations[trainNum].emplace_back(name, arr, dep);
    }

    // Load coaches
    ifstream cFile(coachesFile);
    getline(cFile, line);
    while (getline(cFile, line))
    {
        stringstream ss(line);
        string trainNum, coachNum, type;
        getline(ss, trainNum, ',');
        getline(ss, coachNum, ',');
        getline(ss, type, ',');

        vector<Seat> seats;
        for (int i = 1; i <= 10; ++i)
            seats.emplace_back(to_string(i));
        trainCoaches[trainNum].emplace_back(coachNum, type, seats);
    }

    // Load trains
    ifstream tFile(trainsFile);
    getline(tFile, line);
    while (getline(tFile, line))
    {
        stringstream ss(line);
        string number, date, route;
        getline(ss, number, ',');
        getline(ss, date, ',');
        getline(ss, route, ',');
        trains.emplace_back(number, date, trainStations[number], trainCoaches[number]);
    }

    return trains;
}


vector<string> getAllStations(const vector<Train>& trains)
{
    set<string> uniqueStations;
    for (auto& train : trains)
    {
        for (auto& station : train.getRoute())
        {
            uniqueStations.insert(station.getName());
        }
    }
    return vector<string>(uniqueStations.begin(), uniqueStations.end());
}
