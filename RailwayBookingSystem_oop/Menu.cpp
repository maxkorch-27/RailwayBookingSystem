#include "Menu.h"
#include <iostream>
using namespace std;

Menu::Menu(const vector<string>& opts) : options(opts) {}

void Menu::display() const
{
	cout << "===== MENU =====" << endl;
	for (size_t i = 0; i < options.size(); i++) 
	{
		cout << (i + 1) << ". " << options[i] << endl;
	}
	cout << "0. Exit" << endl;
}
