#include <iomanip>
#include <ostream>

#include <currency.h>

namespace msim{

	// stream
	std::ostream& operator<<(std::ostream& os, const Currency& c){
		os << std::fixed;
		std::streamsize prev = os.precision();
		os << std::setprecision(Currency::PRECISION() + 1);
		os << Currency::ROUND(c.value) / Currency::SHIFT();
		os.unsetf(std::ios_base::floatfield);
		os << std::setprecision(prev);
		return os;
	}

}