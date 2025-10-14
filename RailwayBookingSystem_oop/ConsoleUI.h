#pragma once
#include "BookingSystem.h"
class ConsoleUI
{
	private:
		BookingSystem& system;
	public:
		ConsoleUI(BookingSystem& bs);
		void run();
	private:
		void handleSearchAndBuy();
		void handleMyTickets();
		void handleReturnTicket();
		void handleCashierReport();
};

