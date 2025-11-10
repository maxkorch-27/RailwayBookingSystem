#include "Report.h"
#include "PerformanceTimer.h"
using namespace std;

Report::Report(int sold, int price, int cancelled, int refund, int revenue)
	: totalSold(sold), totalPrice(price), totalCancelled(cancelled), totalRefund(refund), totalRevenue(revenue)
{
}

void Report::display() const
{
	cout << endl << "===== Cashier Report =====" << endl;
	cout << "Total tickets sold: " << totalSold << endl;
	cout << "Total price of sold tickets: " << totalPrice << " euro" << endl;
	cout << "Total tickets cancelled: " << totalCancelled << endl;
	cout << "Total refunds issued: " << totalRefund << " euro" << endl;
	cout << "Total revenue: " << totalRevenue << " euro" << endl;
	cout << "==========================" << endl << endl;
}

void Report::generateReport(const vector<unique_ptr<Ticket>>& tickets)
{
	PerformanceTimer timer("Generate Cashier Report");

	Report report(0, 0, 0, 0, 0);
	for (auto& ticket : tickets)
	{
		if (ticket->getStatus() == "booked")
		{
			report.totalSold++;
			report.totalPrice += ticket->getPrice();
		}
		else if (ticket->getStatus() == "cancelled")
		{
			report.totalSold++;
			report.totalCancelled++;
			report.totalPrice += ticket->getPrice();
			report.totalRefund += ticket->getMoneyReturned();
		}
	}
	report.totalRevenue = report.totalPrice - report.totalRefund;
	report.display();
}