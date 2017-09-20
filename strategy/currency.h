#ifndef GUARD_CURRENCY
#define GUARD_CURRENCY

#include <iosfwd>
#include <cmath>
#include <limits>

namespace msim{

	// number of significant decimal places (precision)
	// note: increasing this will decrease maximum value capacity
#define _PRECISION 2 
#define _DECIMAL_PLACES 1.0e+2
	// Note: make sure the above values are consistent!
	// range: currency < 1.0e12

	// tolerance value for comparators (value after PRECISION decimal places)
#define _EPSILON 0.1

	class Currency{
	public:
		// allow for flexibility in underlying data type
		typedef double Double;
		// returns maximum capacity of type Double in currency units 
		static Double MAX_VALUE(){
			return std::pow(10, std::numeric_limits<Double>::digits10 - _PRECISION) - 1;
		}

		Currency(){}

		Currency(Double d) : value(d * SHIFT()) {}
		Currency(const Currency& c) : value(c.value) {}

		Currency& operator=(const Currency& c){
			value = c.value;
			return *this;
		}
		Currency& operator=(Double d){
			value = d * SHIFT();
			return *this;
		}

		~Currency(){ }

	private:
		Double value;

		static Double SHIFT(){ return _DECIMAL_PLACES; }
		static int PRECISION(){ return _PRECISION; }
		static Double ROUND(Double val){ return std::floor(val + 0.5); }

	public:
		// conversion operator
		operator Double const();

		// negative
		friend Currency operator-(const Currency&);

		// addition & subtraction
		friend Currency operator+(const Currency&, const Currency&);
		friend Currency operator-(const Currency&, const Currency&);
		Currency& operator+=(const Currency&); // implicit conversions allowed
		Currency& operator-=(const Currency&); 

		// multiplication & division
		friend Currency operator*(const Currency&, Double);
		friend Currency operator*(Double, const Currency&);
		friend Currency operator/(const Currency&, Double);

		// comparators
		friend bool operator==(const Currency&, const Currency&);
		friend bool operator!=(const Currency&, const Currency&);
		friend bool operator<=(const Currency&, const Currency&);
		friend bool operator>=(const Currency&, const Currency&);
		friend bool operator<(const Currency&, const Currency&);
		friend bool operator>(const Currency&, const Currency&);

		// stream
		friend std::ostream& operator<<(std::ostream&, const Currency&);
	};

	//conversion operator
	inline Currency::operator Currency::Double const(){
		return value / Currency::SHIFT();
	}

	// negative
	inline Currency operator-(const Currency& c){
		Currency ret;
		ret.value = -c.value;
		return ret;
	}

	// addition & subtraction
	inline Currency operator+(const Currency& c1, const Currency& c2){
		Currency c;
		c.value = c1.value + c2.value;
		return c;
	}
	inline Currency operator-(const Currency& c1, const Currency& c2){
		Currency c;
		c.value = c1.value - c2.value;
		return c;
	}
	inline Currency& Currency::operator+=(const Currency& c){
		value += c.value;
		return *this;
	}
	inline Currency& Currency::operator-=(const Currency& c){
		value -= c.value;
		return *this;
	}

	// multiplication & division
	inline Currency operator*(const Currency& c, Currency::Double d){
		Currency ret;
		ret.value = c.value * d;
		return ret;
	}
	inline Currency operator*(Currency::Double d, const Currency& c){
		return c*d;
	}
	inline Currency operator/(const Currency& c, Currency::Double d){
		Currency ret;
		ret.value = c.value / d;
		return ret;
	}

	// comparators
	inline bool operator==(const Currency& c1, const Currency& c2){
		return std::abs(Currency::ROUND(c1.value) - Currency::ROUND(c2.value)) < _EPSILON;
	}
	inline bool operator!=(const Currency& c1, const Currency& c2){
		return !(c1 == c2);
	}
	inline bool operator>(const Currency& c1, const Currency& c2){
		return Currency::ROUND(c1.value) - Currency::ROUND(c2.value) > 5 * _EPSILON;
	}
	inline bool operator<(const Currency& c1, const Currency& c2){
		return Currency::ROUND(c1.value) - Currency::ROUND(c2.value) < -5 * _EPSILON;
	}
	inline bool operator<=(const Currency& c1, const Currency& c2){
		return (c1 == c2) || (c1 < c2);
	}
	inline bool operator>=(const Currency& c1, const Currency& c2){
		return (c1 == c2) || (c1 > c2);
	}

#undef _DECIMAL_PLACES
#undef _EPSILON
#undef _PRECISION

}

#endif // !GUARD_CURRENCY
