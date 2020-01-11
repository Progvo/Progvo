// (c) 2015-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_QNumber_hpp
#define HEADER_QNumber_hpp

#include "ZNumber.hpp"

class QNumber {
	ZNumber _num, _den;
	
	void _setNumSign() {
		_num.sign(_num.sign() != _den.sign());
		_den.sign(false);
	}
	
public:
	QNumber() {_num = ZNumber::zero(); _den = ZNumber::one();}
	QNumber(Ecii::String, uint32_t = 10);
	QNumber(const ZNumber& num0, const ZNumber& den0 = ZNumber::one()) {
		_num = num0;
		_den = den0;
		_setNumSign();
	}
	
	static bool isValid(Ecii::String, uint32_t = 10);
	Ecii::String es(uint32_t = 10) const;
	
	void reset() {_num = ZNumber::zero(); _den = ZNumber::one();}
	ZNumber num() const {return _num;}
	ZNumber den() const {return _den;}
	void num(const ZNumber& n0) {_num = n0;}
	void den(const ZNumber& n0) {_den = n0;}
	
	bool isInf() const {return (_num.isInf() && !_den.isInf());}
	void setInf() {_num.setInf(); _den = ZNumber::one();}
	bool isUndef() const {return (_num.isUndef() || _den.isUndef() || (_num.isInf() && _den.isInf()) || _den.isZero());}
	void setUndef() {_num.setUndef(); _den.setUndef();}
	bool sign() const {return _num.sign() != _den.sign();}
	void sign(bool newSign) {_num.sign(newSign); _den.sign(false);}
	bool isInt() const {return (!isUndef() && !isInf() && _den == ZNumber::one());}
	ZNumber intPart() const {
		if (isInt()) return _num;
		else return _num/_den;
	}
	
	QNumber& reverse() {
		if (_num.isZero()) setUndef();
		else if (!isInf() && !isUndef()) {std::swap(_num, _den); _setNumSign();}
		else if (isInf()) *this = zero();
		return *this;
	}
	QNumber& simp();
	
	static QNumber zero() {return QNumber();}
	static QNumber one() {return QNumber(ZNumber::one(), ZNumber::one());}
	static QNumber two()  {return QNumber(ZNumber::two(), ZNumber::one());}
	static QNumber half()  {return QNumber(ZNumber::one(), ZNumber::two());}
	bool isZero() const {return _num.isZero() && !isUndef();}
	
	QNumber operator-() const;
	QNumber& operator+=(const QNumber&);
	QNumber& operator-=(const QNumber&);
	QNumber& operator*=(const QNumber&);
	QNumber& operator/=(const QNumber&);
	
	QNumber& operator++() {*this += one(); return *this;}
	QNumber& operator--() {*this -= one(); return *this;}
	QNumber operator++(int) {QNumber tmp(*this); ++(*this); return tmp;}
	QNumber operator--(int) {QNumber tmp(*this); --(*this); return tmp;}
};


inline QNumber operator+(QNumber x, const QNumber& y) {x += y; return x;}
inline QNumber operator-(QNumber x, const QNumber& y) {x -= y; return x;}
inline QNumber operator*(QNumber x, const QNumber& y) {x *= y; return x;}
inline QNumber operator/(QNumber x, const QNumber& y) {x /= y; return x;}

inline bool operator==(const QNumber &x, const QNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	return (x.num()*y.den() == x.den()*y.num());
}
inline bool operator!=(const QNumber &x, const QNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	return (x.num()*y.den() != x.den()*y.num());
}
inline bool operator<(const QNumber &x, const QNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	return (x.num()*y.den() < x.den()*y.num());
}
inline bool operator<=(const QNumber &x, const QNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	return (x.num()*y.den() <= x.den()*y.num());
}
inline bool operator>(const QNumber &x, const QNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	return (x.num()*y.den() > x.den()*y.num());
}
inline bool operator>=(const QNumber &x, const QNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	return (x.num()*y.den() >= x.den()*y.num());
}

inline std::ostream& operator<<(std::ostream& stream, const QNumber& n) {
	stream << n.es(10);
	return stream;
}

#endif