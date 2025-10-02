#include "TrainsData.h"
#include "Station.h"
#include "Coach.h"
#include "Seat.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

vector<Train> createTrains() {
    vector<Train> trains;

    // FIRST TRAIN Tallinn - Tartu
    vector<Station> route1 = {
        Station("Tallinn", "06:15", "06:15"),
        Station("Kehra", "06:40", "06:45"),
        Station("Tapa", "07:25", "07:27"),
        Station("Vageva", "08:15", "08:16"),
        Station("Tabivere", "08:53", "08:55"),
        Station("Tartu", "09:15", "09:15")
    };

    vector<Seat> seats1;
    for (int i = 1; i <= 10; i++)
        seats1.push_back(Seat(to_string(i)));

    vector<Coach> coaches1 = {
        Coach("1", "Economy", seats1),
        Coach("2", "Economy", seats1),
        Coach("3", "Business", seats1),
        Coach("4", "Business", seats1),
    };

    Train train1("101", "01-10-2025", route1, coaches1);
    Train train2("101", "02-10-2025", route1, coaches1);
    Train train3("101", "03-10-2025", route1, coaches1);
    trains.push_back(train1);
    trains.push_back(train2);
    trains.push_back(train3);

    // SECOND TRAIN Tallinn - Narva
    vector<Station> route2 = {
        Station("Tallinn", "11:00", "11:00"),
        Station("Tapa", "12:16", "12:18"),
        Station("Rakvere", "12:380", "12:40"),
        Station("Johvi", "13:20", "13:21"),
        Station("Narva", "13:48", "13:48")
    };
    Train train4("202", "01-10-2025", route2, coaches1);
    Train train5("202", "02-10-2025", route2, coaches1);
    Train train6("202", "03-10-2025", route2, coaches1);
    trains.push_back(train4);
    trains.push_back(train5);
    trains.push_back(train6);

    // THIRD TRAIN Tallinn - Paldiski
    vector<Station> route3 = {
        Station("Tallinn", "17:45", "17:45"),
        Station("Laagri", "18:05", "18:06"),
        Station("Keila", "18:18", "18:20"),
        Station("Paldiski", "18:40", "18:40")
    };
    Train train7("303", "01-10-2025", route3, coaches1);
    Train train8("303", "02-10-2025", route3, coaches1);
    Train train9("303", "03-10-2025", route3, coaches1);
    trains.push_back(train7);
    trains.push_back(train8);
    trains.push_back(train9);

    return trains;
}

vector<string> getAllStations(const vector<Train>& trains)
{
    // Use a set to avoid duplicate station names
    set<string> uniqueStations;
    // Iterate through all trains and their routes to collect station names
    for (auto& train : trains) {
        for (auto& station : train.getRoute()) {
            uniqueStations.insert(station.getName());
        }
    }
    return vector<string>(uniqueStations.begin(), uniqueStations.end());
}