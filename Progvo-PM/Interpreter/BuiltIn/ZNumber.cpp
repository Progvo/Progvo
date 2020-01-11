// (c) 2015-2020 Pttn (https://Progvo.dev)

#include "ZNumber.hpp"

ZNumber::ZNumber(Ecii::String es, uint32_t base) {
	if (!isValid(es, base)) setUndef();
	else {
		_sign = false;
		if (Ecii::eciiToChar(es.getChar(0)) == '-') {
			_sign = true;
			es.erase(1);
		}
		_n = NNumber(es, base);
	}
	if (_n.isZero() || _n.isUndef()) _sign = false;
}

bool ZNumber::isValid(Ecii::String es, uint32_t base) {
	if (es.size() == 0) return false;
	if (Ecii::eciiToChar(es.getChar(0)) == '-') es.erase(1);
	return NNumber::isValid(es, base);
}

Ecii::String ZNumber::es(uint32_t base) const {
	Ecii::String es;
	if (_sign) es += "-";
	es += _n.es(base);
	return es;
}

ZNumber& ZNumber::operator+=(const ZNumber& y) {
	if (_sign == y._sign) _n += y._n;
	else {
		if (_n.isInf() && y._n.isInf()) setUndef();
		else {
			if (_n < y._n) _sign = y._sign;
			_n = delta(_n, y._n);
		}
	}
	if (_n.isZero() || _n.isUndef()) _sign = false;
	return *this;
}

ZNumber& ZNumber::operator-=(const ZNumber& y) {
	*this += -y;
	return *this;
}

ZNumber& ZNumber::operator*=(const ZNumber& y) {
	_n *= y._n;
	if (_sign == y._sign) _sign = false;
	else _sign = true;
	if (_n.isZero() || _n.isUndef()) _sign = false;
	return *this;
}

ZNumber& ZNumber::operator/=(const ZNumber& y) {
	if (y._n.isZero()) setUndef();
	else {
		_n /= y._n;
		if (_sign == y._sign) _sign = false; 
		else _sign = true;
		if (_n.isZero() || _n.isUndef()) _sign = false;
	}
	return *this;
}

ZNumber& ZNumber::operator++() {
	if (!_sign) _n++;
	else {
		_n--;
		if (_n.isZero())
			_sign = false;
	}
	return *this;
}

ZNumber& ZNumber::operator--() {
	if (_sign) _n++;
	else if (_n.isZero()) {
		_sign = true;
		_n = NNumber::one();
	}
	else _n--;
	return *this;
}

bool operator==(const ZNumber& x, const ZNumber& y) {
	return (x.n() == y.n() && x.sign() == y.sign());
}
bool operator!=(const ZNumber& x, const ZNumber& y) {
	if (x.isUndef() || y.isUndef()) return false;
	return (x.n() != y.n() || x.sign() != y.sign());
}
bool operator<(const ZNumber& x, const ZNumber& y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (!x.sign() && y.sign()) return false;
	else if (x.sign() && !y.sign()) return true;
	else {
		if (!x.sign()) return x.n() < y.n();
		else return y.n() < x.n();
	}
}
bool operator<=(const ZNumber& x, const ZNumber& y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (!x.sign() && y.sign()) return false;
	else if (x.sign() && !y.sign()) return true;
	else {
		if (!x.sign()) return x.n() <= y.n();
		else return y.n() <= x.n();
	}
}
bool operator>(const ZNumber& x, const ZNumber& y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (x.sign() && !y.sign()) return false;
	else if (!x.sign() && y.sign()) return true;
	else {
		if (!x.sign()) return x.n() > y.n();
		else return y.n() > x.n();
	}
}
bool operator>=(const ZNumber& x, const ZNumber& y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (x.sign() && !y.sign()) return false;
	else if (!x.sign() && y.sign()) return true;
	else {
		if (!x.sign()) return x.n() >= y.n();
		else return y.n() >= x.n();
	}
}