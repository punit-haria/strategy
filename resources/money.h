/*			Copymiddle 1991 Adolfo Di Mare				 */
/*   Use freely but acknowledge author and publication.  */
/*    DO NOT SELL IT. The author reserves all rights!    */
#ifndef GUARD_MONEY
#define GUARD_MONEY

extern "C"{	// avoid type clashes with the C library
#include <math.h>  /* floor() */
#include <float.h> /* DBL_DIG */
}

#include <iosfwd>

#ifndef MONEY_DECIMALS		/* number of decimals in */
#define MONEY_DECIMALS 2	/* any money quantity    */
#endif

//#define __VAL(n) n					/* 1 level indirection */
//#define TENPOW(n) __VAL(1.0e##n)		/* trick to yield 10^n */

#define MONEY_DIG DBL_DIG

class Money{
public:
	typedef double Double;
	typedef int boolean;

	static int decimals(){ return MONEY_DECIMALS; }
	static int digits(){ return MONEY_DIG; }
	static Double TENPOW(int n){ return pow(10.0, n); }
	static Double SCALE(){ return TENPOW(MONEY_DECIMALS); }	

	Money(){ /*do nothing constructor for efficiency*/ };
	Money(Double);
	Money(const Money&); 

	Money& operator=(const Money&);
	Money& operator=(Double);

	~Money(){ /*nothing to destroy*/ };

	// convert to Double:
	operator Double() const;	// use case: Money m; Double d = m;
	
	boolean OK() const;		// check Money's invariant
	void FIX();			// get rid of unwanted decimals

	friend Money operator+(const Money&, const Money&);
	friend Money operator+(Double, const Money&);
	friend Money operator+(const Money&, Double);
	friend Money operator-(const Money&, const Money&);
	friend Money operator-(Double, const Money&);
	friend Money operator-(const Money&, Double);

	friend Money operator*(const Money&, Double);
	friend Money operator*(Double, const Money&);
	// Money * Money is invalid.
	// Double / Money is invalid.
	friend Money operator/(const Money&, Double);
	friend Double operator/(const Money&, const Money&);	
	friend Money operator%(const Money&, const Money&);

	friend boolean operator==(const Money&, const Money&);
	friend boolean operator!=(const Money&, const Money&);
	friend boolean operator<(const Money&, const Money&);
	friend boolean operator>(const Money&, const Money&);
	friend boolean operator<=(const Money&, const Money&);
	friend boolean operator>=(const Money&, const Money&);
	
	friend boolean operator==(const Money&, Double);
	friend boolean operator!=(const Money&, Double);
	friend boolean operator<(const Money&, Double);
	friend boolean operator>(const Money&, Double);
	friend boolean operator<=(const Money&, Double);
	friend boolean operator>=(const Money&, Double);

	friend boolean operator==(Double, const Money&);
	friend boolean operator!=(Double, const Money&);
	friend boolean operator<(Double, const Money&);
	friend boolean operator>(Double, const Money&);
	friend boolean operator<=(Double, const Money&);
	friend boolean operator>=(Double, const Money&);

	Money& operator+=(const Money&);
	Money& operator+=(const Double);
	Money& operator-=(const Money&);
	Money& operator-=(const Double);

	Money& operator*=(Double);
	Money& operator/=(Double);

	friend Money operator+(const Money&);
	friend Money operator-(const Money&);
	Money& operator++(); // prefix
	Money& operator--();
	Money& operator++(int); //postfix (int argument is just to distinguish from prefix)
	Money& operator--(int);
	friend boolean operator!(const Money&); 

	friend Money abs(const Money&);
	friend Money flatten(const Money& m, Double cents = 0.25, boolean rounding = 1);

	// write to stream
	friend std::ostream& operator<<(std::ostream&, const Money&);

protected:	// allow users to change the class behaviour
	Double m_money;
};

// Constructors and assignment:

inline Money::Money(Double d){
	m_money = d * Money::SCALE();
}

inline Money::Money(const Money& m){
	m_money = m.m_money;
}

inline Money& Money::operator=(const Money& m){
	m_money = m.m_money;
	return *this;
}

inline Money& Money::operator=(Double d){
	// assign from Double
	m_money = d * Money::SCALE();
	FIX();
	return *this;
}

inline Money::operator Money::Double() const{
	return m_money / Money::SCALE();
}

inline Money::boolean Money::OK() const{
	// returns true when the quantity stored 
	// in *this really corresponds to a money quantity.
	Money temp;
	temp.m_money = m_money;
	temp.FIX();
	return((temp.m_money == m_money) &&
		(fabs(m_money) < (TENPOW(MONEY_DIG) / Money::SCALE())));
}

inline void Money::FIX(){
	// deletes all decimal digits beyond the MONEY_DECIMALS
	// decimal place. If the value is out of range, FIX will 
	// NOT fix it.
	m_money = (m_money > 0.0
		? floor(m_money
#ifdef MONEY_ROUNDING
		+ 0.5 // 0.49 is also an option
#endif
		) : ceil(
		m_money
#ifdef MONEY_ROUNDING
		- 0.5
#endif
		));
}

// add
inline Money operator+(const Money& m, const Money& mm){
	Money temp;
	temp.m_money = m.m_money + mm.m_money;
	return temp;
}
inline Money operator+(Money::Double d, const Money& m){
	return Money(d) + m;
}
inline Money operator+(const Money& m, Money::Double d){
	return m + Money(d);
}

// subtract
inline Money operator-(const Money& m, const Money& mm){
	Money temp;
	temp.m_money = m.m_money - mm.m_money;
	return temp;
}
inline Money operator-(Money::Double d, const Money& m){
	return Money(d) - m;
}
inline Money operator-(const Money& m, Money::Double d){
	return m - Money(d);
}

// multiply
inline Money operator*(const Money& m, Money::Double d){
	Money temp;
	temp.m_money = m.m_money * d;
	temp.FIX();
	return temp;
}
inline Money operator*(Money::Double d, const Money& m){
	return m*d;
}

// divide
inline Money::Double operator/(const Money& m, const Money& mm){
	return m.m_money / mm.m_money;
}
inline Money operator/(const Money& m, Money::Double d){
	Money temp;
	temp.m_money = m.m_money / d;
	temp.FIX();
	return temp;
}
inline Money operator%(const Money& m, const Money& mm){
	Money temp;
	temp.m_money = fmod(m.m_money, mm.m_money);
	temp.FIX();
	return temp;
}

// compare
inline Money::boolean operator==(const Money& m, const Money& mm){
	return m.m_money == mm.m_money;
}
inline Money::boolean operator!=(const Money& m, const Money& mm){
	return m.m_money != mm.m_money;
}
inline Money::boolean operator<(const Money& m, const Money& mm){
	return m.m_money < mm.m_money;
}
inline Money::boolean operator>(const Money& m, const Money& mm){
	return m.m_money > mm.m_money;
}
inline Money::boolean operator<=(const Money& m, const Money& mm){
	return m.m_money <= mm.m_money;
}
inline Money::boolean operator>=(const Money& m, const Money& mm){
	return m.m_money >= mm.m_money;
}

inline Money::boolean operator==(const Money& m, Money::Double d){
	return m.m_money == d;
	// return m == Money(d); // take a pick!
	// Should the Double quality be promoted to a Money type?
	// Direct comparison seems more transparent.
}
inline Money::boolean operator!=(const Money& m, Money::Double mm){
	return !(m == mm);
}
inline Money::boolean operator<(const Money& m, Money::Double mm) {
	return m.m_money <  mm;
}
inline Money::boolean operator>(const Money& m, Money::Double mm) {
	return m.m_money >  mm;
}
inline Money::boolean operator<=(const Money& m, Money::Double mm) {
	return m.m_money <= mm;
}
inline Money::boolean operator>=(const Money& m, Money::Double mm) {
	return m.m_money >= mm;
}

inline Money::boolean operator==(Money::Double m, const Money& mm) {
	return (mm == m);
}
inline Money::boolean operator!=(Money::Double m, const Money& mm) {
	return !(mm == m);
}
inline Money::boolean operator<(Money::Double m, const Money& mm) {
	return m <  mm.m_money;
}
inline Money::boolean operator>(Money::Double m, const Money& mm) {
	return m >  mm.m_money;
}
inline Money::boolean operator<=(Money::Double m, const Money& mm) {
	return m <= mm.m_money;
}
inline Money::boolean operator>=(Money::Double m, const Money& mm) {
	return m >= mm.m_money;
}

inline Money& Money::operator+=(const Money& m){
	m_money += m.m_money;
	return *this;
}
inline Money& Money::operator+=(const Double d){
	m_money += d * Money::SCALE();
	FIX();
	return *this;
}
inline Money& Money::operator-=(const Money& m){
	m_money -= m.m_money;
	return *this;
}
inline Money& Money::operator-=(const Double d){
	m_money -= d * Money::SCALE();
	FIX();
	return *this;
}

inline Money& Money::operator*=(Double d){
	m_money *= d;
	FIX();
	return *this;
}
inline Money& Money::operator/=(Double d){
	m_money /= d;
	FIX();
	return *this;
}

// unary operators
inline Money operator+(const Money& m){
	return m;
}
inline Money operator-(const Money& m){
	Money temp;
	temp.m_money = -m.m_money;
	return temp;
}
inline Money& Money::operator++(){
	m_money += Money::SCALE();
#if (MONEY_DECIMALS < 0)
	FIX(); // avoid problems from representation of 10^-n
#endif
	return *this;
}
inline Money& Money::operator--(){
	m_money -= Money::SCALE();
#if (MONEY_DECIMALS < 0)
	FIX();
#endif
	return *this;
}

inline Money& Money::operator++(int){
	return ++(*this);
}
inline Money& Money::operator--(int){
	return --(*this);
}
inline Money::boolean operator!(const Money& m){
	return m.m_money == 0;
}

inline Money abs(const Money& m){
	Money temp;
	temp.m_money = fabs(m.m_money);
	return temp;
}

// avoid namespace overcrowding
//#undef __VAL
//#undef TENPOW  

#endif // !GUARD_MONEY
