// (c) 2015-2020 Pttn (https://Progvo.dev)

#include "QNumber.hpp"

QNumber::QNumber(Ecii::String es, uint32_t base) {
	*this = QNumber();
	if (!isValid(es, base)) setUndef();
	else if (ZNumber::isValid(es, base)) _num = ZNumber(es, base);
	else if (isValid(es, base)) {
		const uint64_t fracBarPos(es.find(Ecii::charToEcii('/')));
		Ecii::String numEs(es.substr(0, fracBarPos - 1)), denEs(es.substr(fracBarPos + 1, es.size() - 1));
		_num = ZNumber(numEs, base);
		_den = NNumber(denEs, base);
		simp();
	}
}

bool QNumber::isValid(Ecii::String es, uint32_t base) {
	if (ZNumber::isValid(es, base)) return true;
	else {
		const uint64_t fracBarPos(es.find(Ecii::charToEcii('/')));
		if (fracBarPos != 0 && fracBarPos != -1ULL) {
			Ecii::String numEs(es.substr(0, fracBarPos - 1)), denEs(es.substr(fracBarPos + 1, es.size() - 1));
			return ZNumber::isValid(numEs, base) && NNumber::isValid(denEs, base);
		}
	}
	return false;
}

Ecii::String QNumber::es(uint32_t base) const {
	Ecii::String es;
	if (isUndef()) es = ES(251);
	else if (isInf()) {
		if (!sign()) es = ES(88);
		else es = "-" + ES(88);
	}
	else {
		es = _num.es(base);
		if (_den != ZNumber::one()) {
			if (_den.sign()) es += "/(" + _den.es(base) + ")";
			else  es += "/" + _den.es(base);
		}
	}
	return es;
}

QNumber& QNumber::simp() {
	if (_num.isZero() || _num.isInf()) {
		_den = ZNumber::one();
		if (_num.isZero()) sign(false);
	}
	else {
		ZNumber GCD(gcd(_num.n(), _den.n()));
		_num /= GCD;
		_den /= GCD;
		_setNumSign();
	}
	return *this;
}

QNumber QNumber::operator-() const {
	QNumber q(*this);
	q.sign(!q.sign());
	return q;
}

QNumber& QNumber::operator+=(const QNumber& y) {
	if (isUndef() || y.isUndef()) {
		setUndef();
		return *this;
	}
	else { // a/b + c/d = (ad + bc)/bd
		_num = (_num * y._den) + (_den * y._num);
		_den *= y._den;
		if (y.sign() && *this < y)
			sign(true);
		simp();
		return *this;
	}
}

QNumber& QNumber::operator-=(const QNumber& y) { // a - b = a + (-b)
	*this += -y;
	return *this;
}

QNumber& QNumber::operator*=(const QNumber& y) { // (a/b)(c/d) = (ac/bd)
	_num *= y._num;
	_den *= y._den;
	simp();
	return *this;
}

QNumber& QNumber::operator/=(const QNumber& y) { // (a/b)/(c/d) = (a/b)(d/c)
	QNumber q(y);
	q.reverse();
	*this *= q;
	return *this;
}