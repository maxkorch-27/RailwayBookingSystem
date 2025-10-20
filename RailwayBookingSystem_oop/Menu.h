#pragma once
#include <vector>
#include <string>
#include "IDisplay.h"
using namespace std;

class Menu : public IDisplay
{
	private:
		vector<string> options;
	public:
		Menu(const vector<string>& opts);
		void display() const override;
};

