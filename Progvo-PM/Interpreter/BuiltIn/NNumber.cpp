// (c) 2015-2020 Pttn (https://Progvo.dev)

#include "NNumber.hpp"

void NNumber::_delLeadingZeros() {
	uintmax_t max(_n.size() - 1);
	for (uint32_t i(0) ; i < max ; i++) {
		if (_n[max - i] == 0) _n.pop_back();
		else break;
	}
}

std::vector<Word> NNumber::_fromDigits(const std::vector<uint64_t>& v0, uint32_t base) const {
	NNumber n(zero()), powBase(one());
	for (uintmax_t i(0) ; i < v0.size() ; i++) {
		NNumber temp;
		temp._n = powBase._n;
		temp *= v0[i]; 
		n += temp;
		powBase *= base;
	}
	return n._n;
}

NNumber& NNumber::_divide(const NNumber &y, NNumber& r) {
	NNumber q;
	r = zero();
	q._n.resize(_n.size(), 0);
	uintmax_t offset(0);
	for (uintmax_t i(WORDSIZE - 1) ; i + 1 > 0 ; i--) {
		if ((_n.back() & (1ULL << i)) == 0) offset++;
		else break;
	}
	for (uintmax_t i(_n.size()*WORDSIZE - offset - 1) ; i + 1 > 0 ; i--) {
		r.shl();
		r._n[0] |= (_n[i/WORDSIZE] & (1ULL << (i % WORDSIZE))) >> (i % WORDSIZE);
		if (r >= y) {
			r -= y;
			q._n[i/WORDSIZE] |= (1ULL << (i % WORDSIZE));
		}
	}
	q._delLeadingZeros();
	*this = q;
	return *this;
}

NNumber::NNumber(const Ecii::String &es, uint32_t base) : _n(std::vector<Word>()), _inf(false), _undef(false) {
	if (base == 1) _n.push_back(es.size());
	else {
		if (!isValid(es, base)) setUndef();
		else if (es == ES(88)) setInf();
		else {
			for (uintmax_t i(0) ; i < es.size() ; i++) {
				bool exists(false);
				for (uintmax_t j(0) ; j < digitsLut.size() ; j++) {
					if (digitsLut[j] == es.getChar(es.size() - i - 1)) {
						exists = true;
						_n.push_back(j);
						break;
					}
				}
				if (!exists) {setUndef(); break;}
			}
			_delLeadingZeros();
			_n = _fromDigits(_n, base);
		}
	}
}

NNumber::NNumber(const BitSequence &bs0) : _inf(false), _undef(false) {
	if (bs0.size() == 0) setUndef();
	else _n = bs0.vw();
}

bool NNumber::isValid(const Ecii::String &es, uint32_t base) {
	if ((es.size() == 0 && base != 1) || base == 0 || base > 16) return false;
	else if (es == Ecii::String(std::byte{88}) || base == 1) return true;
	else {
		for (uintmax_t i(0) ; i < es.size() ; i++) {
			bool validChar(false);
			for (uintmax_t j(0) ; j < base ; j++) {
				if (es.getChar(es.size() - i - 1) == digitsLut[j]) {
					validChar = true;
					break;
				}
			}
			if (!validChar) return false;
		}
	}
	
	return true;
}

std::vector<uint64_t> NNumber::digits(uint32_t base) const {
	if (isZero()) return _n;
	else {
		std::vector<Word> v;
		NNumber n(*this);
		while (!n.isZero()) {
			NNumber r;
			n._divide(base, r);
			v.push_back(r._n[0]);
		}
		return v;
	}
}

Ecii::String NNumber::es(uint32_t base) const {
	Ecii::String es;
	if (_undef || base == 0 || base > 16) es = std::byte{251};
	else if (_inf) es = std::byte{88};
	else if (base == 1) {
		if (isZero()) es = std::byte{0};
		else {
			NNumber temp(*this);
			while (temp != 0) {
				es.add(std::byte{89}); // |
				temp--;
			}
		}
	}
	else {
		std::vector<uint64_t> v(digits(base));
		for (uintmax_t i(0) ; i < v.size() ; i++) es.add(digitsLut[v[v.size() - i - 1]]);
	}
	return es;
}

BitSequence NNumber::bs() const {
	if (_inf || _undef) return BitSequence();
	uintmax_t digits(_n.size()*WORDSIZE);
	for (uint8_t i(0) ; i < WORDSIZE ; i++) {
		if ((_n.back() & (1ULL << (WORDSIZE - i - 1))) == 0) digits--;
		else break;
	}
	if (digits == 0) digits = 1;
	return BitSequence(_n, digits);
}

void NNumber::shl() {
	uintmax_t words(_n.size());
	if ((_n.back() & (1ULL << (WORDSIZE - 1))) != 0) _n.push_back(1ULL);
	for (uintmax_t i(words - 1) ; i > 0 ; i--) {
		_n[i] <<= 1;
		_n[i] |= ((1ULL << (WORDSIZE - 1)) & _n[i - 1]) >> (WORDSIZE - 1);
	}
	_n[0] <<= 1;
}

void NNumber::shr() {
	for (uintmax_t i(0) ; i < _n.size() ; i++) _n[i] >>= 1;
	if (_n.back() == 0 && _n.size() > 1) _n.pop_back();
}

NNumber gcd(NNumber x, NNumber y) {
	if (x < y) x._n.swap(y._n);
	NNumber r(NNumber::one());
	if (x.isUndef() || y.isUndef() || x.isInf() || y.isInf())
		y.setUndef();
	else if (y.isZero()) return x;
	else {
		while (!r.isZero()) {
			x._divide(y, r);
			if (r.isZero()) return y;
			else {
				x._n.swap(y._n);
				y._n.swap(r._n);
			}
		}
	}
	return y;
}

NNumber lcm(const NNumber& x, const NNumber& y) {
	NNumber r(x), dummy;
	if (x.isZero() && y.isZero()) r = NNumber::zero();
	else if (x.isUndef() || y.isUndef()
		    || x.isInf() || y.isInf()
		    || x.isZero() || y.isZero()) r.setUndef();
	else {
		r /= gcd(r, y);
		r *= y;
	}
	return r;
}

NNumber& NNumber::operator+=(NNumber y) {
	if (_undef || y._undef) setUndef();
	else if (_inf || y._inf) setInf();
	else {
		uintmax_t posMax(std::max(_n.size(), y._n.size()));
		if (_n.size() > y._n.size()) y._n.resize(posMax, 0ULL);
		else _n.resize(posMax, 0ULL);
		Word sum(0ULL);
		bool carry(false);
		for (uintmax_t i(0) ; i < posMax ; i++) {
			sum = _n[i] + y._n[i];
			if (carry) {
				sum++;
				carry = false;
			}
			_n[i] = sum;
			if (sum < y._n[i]) carry = true;
		}
		if (carry) _n.push_back(1ULL);
	}
	return *this;
}

NNumber& NNumber::operator-=(NNumber y) {
	if (_undef || y._undef || y._inf || *this < y) setUndef();
	else if (_inf) setInf();
	else {
		uintmax_t posMax(_n.size());
		if (_n.size() > y._n.size()) y._n.resize(posMax, 0);
		Word diff(0);
		bool borrow(false);
		for (uintmax_t i(0) ; i < posMax ; i++) {
			diff = _n[i] - y._n[i];
			if (borrow) {
				diff--;
				borrow = false;
			}
			if (diff > _n[i]) borrow = true;
			_n[i] = diff;
		}
		_delLeadingZeros();
	}
	return *this;
}

NNumber& NNumber::operator*=(const NNumber& y0) {
	if (_undef || y0._undef || (_inf && y0.isZero()) || (isZero() && y0._inf)) setUndef();
	else if (_inf || y0._inf) setInf(); 
	else {
		NNumber y(y0);
		if (_n.size() < y._n.size()) _n.swap(y._n);
		uintmax_t size2(y._n.size());
		NNumber x(*this);
		reset();
		for (uintmax_t i(0) ; i < size2 ; i++) {
			NNumber temp(x);
			uintmax_t size1(temp._n.size());
			Word prod(0), carry(0);
			Word b1(y._n[i] >> WORDSIZE/2ULL), b0(y._n[i] & ((1ULL << (WORDSIZE/2ULL)) - 1ULL));
			for (uintmax_t j(0) ; j < size1 ; j++) {
				Word a1(temp._n[j] >> (WORDSIZE/2ULL)), a0(temp._n[j] & ((1ULL << (WORDSIZE/2ULL)) - 1ULL));
				Word t(a0*b0);
				Word c0(t & ((1ULL << (WORDSIZE/2ULL)) - 1ULL));
				Word k(t >> WORDSIZE/2ULL);
				
				t = a1*b0 + k;
				k = t & ((1ULL << WORDSIZE/2) - 1ULL);
				Word d1 = t >> WORDSIZE/2;
				t = a0*b1 + k;
				k = (t >> WORDSIZE/2ULL);
				
				prod = (t << (WORDSIZE/2ULL)) + c0;
				temp._n[j] = prod + carry;
				carry = a1*b1 + d1 + k;
				if (temp._n[j] < prod) carry++;
			}
			if (carry != 0ULL) temp._n.push_back(carry);
			*this += temp;
			x._n.insert(x._n.begin(), 0ULL);
		}
	}
	return *this;
}

NNumber& NNumber::operator/=(const NNumber& y) {
	if (_undef || y._undef || (_inf && y._inf) || (isZero() && y.isZero()))
		setUndef();
	else if (_inf || y.isZero()) setInf();
	else if (y._inf || *this < y) *this = zero();
	else {
		NNumber r;
		_divide(y, r);
	}
	return *this;
}

NNumber& NNumber::operator%=(const NNumber& y) {
	NNumber r;
	if (_undef || y._undef || _inf || y.isZero()) r.setUndef();
	else if (*this < y) return *this;
	else _divide(y, r);
	*this = r;
	return *this;
}

NNumber& NNumber::operator++() {
	for (uintmax_t i(0) ; i < _n.size() ; i++) {
		if (_n[i] != WORDMAX) {
			_n[i]++;
			return *this;
		}
		else _n[i] = 0;
	}
	_n.push_back(1);
	return *this;
}

NNumber& NNumber::operator--() {
	if (_n.size() == 1) {
		if (_n[0] == 0) _undef = true;
		else _n[0]--;
	}
	else {
		for (uintmax_t i(0) ; i < _n.size() ; i++) {
			if (_n[i] != 0) {
				_n[i]--;
				if (_n[i] == 0 && i == _n.size() - 1) _n.pop_back();
				return *this;
			}
			else _n[i] = WORDMAX;
		}
	}
	return *this;
}

bool operator==(const NNumber &x, const NNumber &y) {
	if (x.isInf() || y.isInf() || x.isUndef() || y.isUndef()) return false;
	else return x._n == y._n;
}

bool operator!=(const NNumber &x, const NNumber &y) {
	if ((x.isInf() && y.isInf()) || x.isUndef() || y.isUndef()) return false;
	else if (x.isInf() || y.isInf()) return true;
	else return x._n != y._n;
}

bool operator<(const NNumber &x, const NNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (x.isInf()) return false;
	else if (y.isInf()) return true;
	else if (x._n.size() > y._n.size()) return false;
	else if (x._n.size() < y._n.size()) return true;
	else {
		for (uintmax_t i(0) ; i < x._n.size() ; i++) {
			uintmax_t j(x._n.size() - i - 1);
			if (x._n[j] < y._n[j]) return true;
			else if (x._n[j] > y._n[j]) return false;
		}
		return false;
	}
}

bool operator<=(const NNumber &x, const NNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (x.isInf()) return false;
	else if (y.isInf()) return true;
	else if (x._n.size() > y._n.size()) return false;
	else if (x._n.size() < y._n.size()) return true;
	else {
		for (uintmax_t i(0) ; i < x._n.size() ; i++) {
			uintmax_t j(x._n.size() - i - 1);
			if (x._n[j] < y._n[j]) return true;
			else if (x._n[j] > y._n[j]) return false;
		}
		return true;
	}
}

bool operator>(const NNumber &x, const NNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (y.isInf()) return false;
	else if (x.isInf()) return true;
	else if (x._n.size() < y._n.size()) return false;
	else if (x._n.size() > y._n.size()) return true;
	else {
		for (uintmax_t i(0) ; i < x._n.size() ; i++) {
			uintmax_t j(x._n.size() - i - 1);
			if (x._n[j] > y._n[j]) return true;
			else if (x._n[j] < y._n[j]) return false;
		}
		return false;
	}
}

bool operator>=(const NNumber &x, const NNumber &y) {
	if (x.isUndef() || y.isUndef()) return false;
	else if (y.isInf()) return false;
	else if (x.isInf()) return true;
	else if (x._n.size() < y._n.size()) return false;
	else if (x._n.size() > y._n.size()) return true;
	else {
		for (uintmax_t i(0) ; i < x._n.size() ; i++) {
			uintmax_t j(x._n.size() - i - 1);
			if (x._n[j] > y._n[j]) return true;
			else if (x._n[j] < y._n[j]) return false;
		}
		return true;
	}
}