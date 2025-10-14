#pragma once
#include <vector>
#include <string>
#include "IDisplay.h"
class Menu : public IDisplay
{
	private:
		std::vector<std::string> options;
	public:
		Menu(const std::vector<std::string>& opts);
		void display() const override;
};

