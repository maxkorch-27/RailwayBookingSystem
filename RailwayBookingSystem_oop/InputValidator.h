#pragma once
#include <string>
#include <regex>
#include <vector>
#include <iostream>
#include <functional>
using namespace std;

class InputValidator {
	public:
		static bool isValidPassport(const string& passport) 
		{
			return regex_match(passport, regex("^[0-9]{6}$"));
		}

		static bool isValidTicketId(const string& id) 
		{
			return regex_match(id, regex("^T[0-9]+$"));
		}

		static bool isValidDate(const string& date) 
		{
			return regex_match(date, regex("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-[0-9]{4}$"));
		}

		static bool isValidCoachType(const string& type) 
		{
			return type == "Economy" || type == "Business";
		}

		static bool isValidStation(const string& name, const vector<string>& stations) 
		{
			for (const auto& s : stations)
				if (s == name) return true;
			return false;
		}

		static bool isValidTrainNumber(const string& trainNumber, const vector<string>& trainNumbers)
		{
			for (const auto& t : trainNumbers)
				if (t == trainNumber) return true;
			return false;
		}

		static bool isValidYesNo(const string& input) 
		{
			return input == "yes" || input == "no";
		}

		static bool isValidName(const string& name) 
		{
			return regex_match(name, regex("^[A-Za-z]+$"));
		}

		static string getValidatedInput(const string& prompt, function<bool(const string&)> validator, const string& errorMsg)
		{
			string input;
			while (true) 
			{
				cout << prompt;
				cin >> input;
				if (validator(input)) break;
				cout << errorMsg << endl;
			}
			return input;
		}
};