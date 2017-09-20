#include <tests/catch.hpp>
#include <ctime>
#include <cstdlib>

#define private public // allow testing of private members
#include <currency.h>
#undef private

//#define _LOOP_TESTS // enable loop tests

TEST_CASE("currency overall test"){
	int shift = int(Currency::SHIFT());
	Currency c1, c2;
	Currency c3(0.0 / shift);
	c1 = Currency::Double(537.0 / shift);
	c2 = 218941.0 / shift;
	REQUIRE((c2 += c1) == Currency((218941.0 + 537.0) / shift));
	REQUIRE((c2 * 0.0) == Currency(0.0 / shift));
	Currency::Double d = (218941.0 / shift) / 537.0;
	c3 = d;
	REQUIRE(c3 == Currency(218941.0 / shift) / 537.0);
}

TEST_CASE("currency multiplication and division test"){
	int shift = int(Currency::SHIFT());
	Currency::Double d1 = 21515.0 / shift;
	Currency::Double d2 = 1.0 / shift;
	REQUIRE(Currency(d1*d2) == d1*Currency(d2));
	REQUIRE(Currency(d1*d2) == Currency(d1)*d2);
	REQUIRE(Currency(d2/d1) == Currency(d2)/d1);
}

TEST_CASE("currency addition & subtraction test"){
	int shift = int(Currency::SHIFT());
	Currency::Double d1 = 24563.0 / shift;
	double d2 = 33059.0 / shift;
	Currency c1, c2;
	c1 = d1;
	c2 = d1;
	REQUIRE(c1 == c2);
	c1 = d2;
	c2 = d2;
	REQUIRE(c1 == c2);
	c1 = d1;
	Currency c3 = c1 + c2;
	Currency c4 = d1 + d2;
	REQUIRE(c3 == c1 + c2);
	REQUIRE(c3 == c4);
	c3 = c1 - c2;
	c4 = d1 - d2;
	REQUIRE(c3 == c1 - c2);
	REQUIRE(c3 == c4);
	c1 = d1;
	c2 = d2;
	c3 = c1;
	c3 += c2;
	REQUIRE(c3 == c1 + c2);
	c3 = c1;
	c3 += d2;
	REQUIRE(c3 == c1 + c2);
	c1 = d1;
	c2 = d2;
	c3 = c1;
	c3 -= c2;
	REQUIRE(c3 == c1 - c2);
	c3 = c1;
	c3 -= d2;
	REQUIRE(c3 == c1 - c2);
}

TEST_CASE("currency conversion operators test"){
	int shift = int(Currency::SHIFT());
	Currency::Double bef = 4328974.0 / shift;
	Currency c(bef);
	Currency::Double aft = c;
	REQUIRE(bef == aft);
	bef = -4328974.0 / shift;
	Currency c1(bef);
	aft = c1;
	REQUIRE(bef == aft);
}

TEST_CASE("currency assignment operators test"){
	int shift = int(Currency::SHIFT());
	Currency c1;
	Currency c2(3254.0 / shift);
	REQUIRE(c1 != c2);
	c1 = c2;
	REQUIRE(c1 == c2);	
	Currency c3;
	c3 = 3254.0 / shift;
	REQUIRE(c3 == c2);
	REQUIRE(c3 == c1);
}

TEST_CASE("currency constructors and comparators test"){	
	int shift = int(Currency::SHIFT());
	
	//Currency c1, c2; 
	//REQUIRE(c1 == c2); // undefined behaviour!
	REQUIRE(Currency(5275.0 / shift) == Currency(5275.0 / shift));
	REQUIRE(Currency(270.0 / shift) != Currency(271.0 / shift));
	Currency c3(270.4 / shift);
	Currency c4(270.1 / shift);
	REQUIRE(c3 == c4);
	REQUIRE(c3.value != c4.value);

	REQUIRE(Currency(-174.0 / shift) == Currency(-174.0 / shift));
	REQUIRE(Currency(-174.1 / shift) == Currency(-174.4 / shift));
	REQUIRE(Currency(-174.1 / shift) != Currency(-174.6 / shift));
	REQUIRE(Currency(-174.1 / shift) == Currency(-173.6 / shift));

	REQUIRE(Currency(1122.0 / shift) > Currency(1121.0 / shift));
	REQUIRE(Currency(1226.0 / shift) < Currency(1227.0 / shift));
	REQUIRE(Currency(1122.0 / shift) >= Currency(1121.0 / shift));
	REQUIRE(Currency(1226.0 / shift) <= Currency(1227.0 / shift));
	REQUIRE(Currency(1353.0 / shift) >= Currency(1353.0 / shift));
	REQUIRE(Currency(1353.0 / shift) <= Currency(1353.0 / shift));

	REQUIRE(Currency(1333.4 / shift) >= Currency(1333.1 / shift));
	REQUIRE(Currency(1333.1 / shift) >= Currency(1333.4 / shift));
	REQUIRE(Currency(1333.4 / shift) <= Currency(1333.1 / shift));
	REQUIRE(Currency(1333.1 / shift) <= Currency(1333.4 / shift));

	REQUIRE(Currency(2386.6 / shift) > Currency(2386.4 / shift));
	REQUIRE(Currency(2386.4 / shift) < Currency(2386.6 / shift));
#ifdef _LOOP_TESTS
	Currency::Double limit = Currency::MAX_VALUE();
	Currency::Double increment = std::pow(10, 10 - Currency::PRECISION());
	for (Currency::Double dd = 0.0; dd <= limit; 
		dd += increment * Currency::Double(std::rand())/ RAND_MAX){
		REQUIRE(Currency(dd) == Currency(dd));
		REQUIRE(Currency(-dd) == Currency(-dd));
		REQUIRE(Currency(dd) < Currency(dd + (001.1 / shift)));
		REQUIRE(Currency(-dd) > Currency(-dd - (001.1 / shift)));
		REQUIRE(Currency(dd) > Currency(dd - (001.1 / shift)));
		REQUIRE(Currency(-dd) < Currency(-dd + (001.1 / shift)));
		REQUIRE(Currency(dd - (001.0 / shift)) != Currency(-dd + (001.0 / shift)));
	}
#endif
	Currency c(89217.0 / shift); 
	Currency c_less(89216.4 / shift);
	REQUIRE(c_less < c);
	REQUIRE(Currency(c_less) < Currency(c));
	Currency c_more(89217.55 / shift);
	REQUIRE(c_more > c);
	REQUIRE(Currency(c_more) > Currency(c));
	REQUIRE(Currency(c_more) != Currency(c));
	REQUIRE(Currency(c_less) != Currency(c));
}

#undef _LOOP_TESTS
