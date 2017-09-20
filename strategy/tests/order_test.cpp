#include <tests/catch.hpp>

#include <order.h>

//#define TEST_ORDER_PRINT

TEST_CASE("overall order test"){
	order::Order o1(25.0, 500, order::BUY);
	order::Order o2(12.55, 200, order::SELL);
	order::StockOrder s1(123.75, 15000000, order::BUY, "GOOG", order::LIMIT);
	order::StockOrder s2(12.55, 200, order::SELL, "AAPL", order::MARKET);
#ifdef TEST_ORDER_PRINT
	std::cout << o1 << std::endl;
	std::cout << o2 << std::endl;
	std::cout << s1 << std::endl;
	std::cout << s2 << std::endl;
#endif
	REQUIRE(s2.price() == o2.price());
	REQUIRE(s2.quantity() == o2.quantity());
	REQUIRE(s2.type() == o2.type());
}

#undef TEST_ORDER_PRINT
