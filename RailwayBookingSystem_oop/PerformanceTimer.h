#pragma once
#include <chrono>
#include <string>
#include <iostream>
using namespace std;
using namespace std::chrono;

class PerformanceTimer
{
private:
	string operation;
	high_resolution_clock::time_point startTime;
public:
	PerformanceTimer(const string& op)
	{
		operation = op;
		startTime = high_resolution_clock::now();
	}
	~PerformanceTimer()
	{
		auto endTime = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(endTime - startTime);
		cout << endl << "---OPERATION " << operation << " took " << duration.count() << " ms." << endl;
	}
};

