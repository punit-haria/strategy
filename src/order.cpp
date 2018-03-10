#include <order.h>
#include <ostream> // this include required for g++ (prior to C++11)

namespace msim{

#define PROCESS_TYPE(t) case(t): str = #t; break

	std::ostream& operator<<(std::ostream& os, const Order& o){
		const char* str = 0;
		switch (o._type){
			PROCESS_TYPE(BUY);
			PROCESS_TYPE(SELL);
		}
		os << "Order Type: " << str
			<< ", Price: " << o._price
			<< ", Quantity: " << o._quantity;
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const StockOrder& s){
		const Order& o(s); // reference to a sliced version of StockOrder (i.e. Order) 
		const char* str = 0;
		switch (s._stype){
			PROCESS_TYPE(MARKET);
			PROCESS_TYPE(LIMIT);
		}
		os << o
			<< ", Symbol: " << s._symbol
			<< ", Stock Order Type: " << str;
		return os;
	}

}
#undef PROCESS_TYPE
