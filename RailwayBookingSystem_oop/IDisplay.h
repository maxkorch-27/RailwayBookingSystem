#pragma once
#include <iostream>
using namespace std;


class IDisplay
{
public:
	virtual void display() const = 0;
	virtual ~IDisplay() = default;
};

