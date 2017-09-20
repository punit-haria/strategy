#include <currency\money.h>

#include <iostream>
#include <iomanip>
#include <ios>

Money flatten(const Money& m, Money::Double cents, Money::boolean rounding){
	// Returns a Money data item where the cents are 
	// rounded modulo "cents". In this way cents can be stripped
	// of Money items when the currency does not have all
	// the coins required to pay every possible quantity.
	Money temp;
	Money::Double c = floor(fabs(cents*Money::SCALE())); // cents
	Money::Double r = fmod(m.m_money, c); // remainder
	temp.m_money = (!rounding || (2.0*r <= c)
		? m.m_money - r
		: m.m_money - r + c);
	return temp;
}

std::ostream& operator<<(std::ostream& os, const Money& m){
	os << std::fixed;
	std::streamsize prev = os.precision();
	os << std::setprecision(MONEY_DECIMALS);
	os << "$" << m.m_money / Money::SCALE();
	os.unsetf(std::ios_base::floatfield);
	os << std::setprecision(prev);
	return os;
}