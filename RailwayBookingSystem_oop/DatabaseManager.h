#pragma once
#include <vector>
#include "Train.h"
#include "Customer.h"
using namespace std;

class DatabaseManager
{
public:
	static vector<Train> loadTrainsFromCSV(const string& trainsFile, const string& stationsFile, const string& coachesFile);
	static vector<string> getAllStations(const vector<Train>& trains);

    static vector<Customer> loadCustomers(const string& filename);
    static void saveCustomers(const string& filename, const vector<Customer>& customers);
    static vector<unique_ptr<Ticket>> loadTickets(
        const string& filename, vector<Customer>& customers, vector<Train>& trains);
    static void saveTickets(
        const string& filename, const vector<unique_ptr<Ticket>>& tickets);
};