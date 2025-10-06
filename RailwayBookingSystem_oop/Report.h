#pragma once
#include <string>
#include <vector>
#include "Ticket.h"
#include "IDisplay.h"
using namespace std;

class Report : public IDisplay
{
	private:
		int totalSold;
		int totalPrice;
		int totalCancelled;
		int totalRefund;
		int totalRevenue;

	public:
		Report(int sold, int price, int cancelled, int refund, int revenue);
		static void generateReport(const vector<unique_ptr<Ticket>>& tickets);
		void display() const override;
};