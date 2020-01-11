// (c) 2015-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_ZNumber_hpp
#define HEADER_ZNumber_hpp

#include "NNumber.hpp"

class ZNumber {
	NNumber _n;
	bool _sign; // -0 and -undef are always avoided
	
public:
	ZNumber() {_n = NNumber::zero(); _sign = false;}
	ZNumber(Ecii::String, uint32_t = 10);
	ZNumber(const uint64_t& a) {_n = NNumber(a); _sign = false;}
	ZNumber(const int64_t& a) {if (a < 0) {_n = NNumber(-a); _sign = true;} else {_n = NNumber(a); _sign = false;}}
	ZNumber(const NNumber& n) {_n = n; _sign = false;}
	static bool isValid(Ecii::String, uint32_t = 10);
	Ecii::String es(uint32_t = 10) const;
	
	void reset() {_n = NNumber::zero(); _sign = false;}
	unsigned int size(uint32_t base = 10) const {return _n.size(base);}
	NNumber n() const {return _n;}
	void n(const NNumber& n) {_n = n; if (n.isUndef() || n.isZero()) _sign = false;}
	
	bool isInf() const {return _n.isInf();}
	void setInf() {_n.setInf();}
	bool isUndef() const {return _n.isUndef();}
	void setUndef() {_sign = false; _n.setUndef();}
	bool sign() const {return _sign;}
	void sign(bool newSign) {_sign = newSign; if (_n.isZero() || _n.isUndef()) _sign = false;}
	
	void shr() {_n.shr();}
	void shl() {_n.shl(); if (_n.isZero()) _sign = false;}
	
	static ZNumber zero() {ZNumber n; n.n(NNumber::zero()); return n;}
	static ZNumber one() {ZNumber n; n.n(NNumber::one()); return n;}
	static ZNumber two() {ZNumber n; n.n(NNumber::two()); return n;}
	
	bool isZero() const {return _n.isZero();}
	
	ZNumber operator-() const {
		ZNumber z(*this);
		if (!_n.isUndef() && !_n.isZero()) z._sign = !z._sign;
		return z;
	}
	
	ZNumber& operator+=(const ZNumber&);
	ZNumber& operator-=(const ZNumber&);
	ZNumber& operator*=(const ZNumber&);
	ZNumber& operator/=(const ZNumber&);
	ZNumber& operator%=(const ZNumber& y) {
		_n %= y._n;
		if (isUndef() || isZero()) sign(false);
		return *this;
	}
	ZNumber& operator++();
	ZNumber& operator--();
	ZNumber operator++(int) {ZNumber tmp(*this); ++(*this); return tmp;}
	ZNumber operator--(int) {ZNumber tmp(*this); --(*this); return tmp;}
	
	friend bool operator==(const ZNumber&, const ZNumber&);
	friend bool operator!=(const ZNumber&, const ZNumber&);
	friend bool operator<(const ZNumber&, const ZNumber&);
	friend bool operator<=(const ZNumber&, const ZNumber&);
	friend bool operator>(const ZNumber&, const ZNumber&);
	friend bool operator>=(const ZNumber&, const ZNumber&);
};

inline ZNumber operator+(ZNumber x, const ZNumber& y) {x += y; return x;}
inline ZNumber operator-(ZNumber x, const ZNumber& y) {x -= y; return x;}
inline ZNumber operator*(ZNumber x, const ZNumber& y) {x *= y; return x;}
inline ZNumber operator/(ZNumber x, const ZNumber& y) {x /= y; return x;}
inline ZNumber operator%(ZNumber x, const ZNumber& y) {x %= y; return x;}

inline std::ostream& operator<<(std::ostream& stream, const ZNumber& z) {
	stream << z.es(10);
	return stream;
}

#endif