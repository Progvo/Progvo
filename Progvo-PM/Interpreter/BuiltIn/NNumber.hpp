// (c) 2015-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_NNumber_hpp
#define HEADER_NNumber_hpp

#include "../../Ecii.hpp"
#include "../BitSequence.hpp"

constexpr std::array<std::byte, 16> digitsLut = { // 0123456789abcdef
	std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3},
	std::byte{4}, std::byte{5}, std::byte{6}, std::byte{7},
	std::byte{8}, std::byte{9}, std::byte{16}, std::byte{17},
	std::byte{18}, std::byte{20}, std::byte{21}, std::byte{22}};

class NNumber {
	std::vector<Word> _n;
	bool _inf, _undef;
	
	void _delLeadingZeros();
	std::vector<Word> _fromDigits(const std::vector<uint64_t>&, uint32_t) const;
	NNumber& _divide(const NNumber&, NNumber&);
	
public:
	NNumber() : _n({0}), _inf(false), _undef(false) {}
	NNumber(const uint64_t& a) {_n = {a}; _inf = false; _undef = false;}
	NNumber(const Ecii::String&, uint32_t = 10);
	NNumber(const BitSequence&);
	
	static bool isValid(const Ecii::String&, uint32_t = 10);
	std::vector<uint64_t> digits(uint32_t) const;
	Ecii::String es(uint32_t = 10) const;
	BitSequence bs() const;
	bool fitsSingleWord() const {return _n.size() == 1;}
	Word firstWord() const {return _n[0];}
	
	void reset() {_n = {0}; _inf = false; _undef = false;}
	uint32_t size(uint32_t base = 10) const {
		if (_inf || _undef) return 0;
		return digits(base).size();
	}
	
	bool isInf() const {return _inf;}
	void setInf() {reset(); _inf = true;}
	bool isUndef() const {return _undef;}
	void setUndef() {reset(); _undef = true;}
	
	void shl();
	void shr();
	
	static NNumber zero() {NNumber n; return n;}
	static NNumber one() {NNumber n; n._n[0] = 1; return n;}
	static NNumber two() {NNumber n; n._n[0] = 2; return n;}
	
	bool isZero() const {
		if (_inf || _undef) return false;
		else return (_n.size() == 1 && _n[0] == 0);
	}
	
	friend NNumber gcd(NNumber x, NNumber y);
	friend NNumber lcm(const NNumber& x, const NNumber& y);
	
	NNumber& operator+=(NNumber y);
	NNumber& operator-=(NNumber y);
	NNumber& operator*=(const NNumber& y);
	NNumber& operator/=(const NNumber& y);
	NNumber& operator%=(const NNumber& y);
	NNumber& operator++();
	NNumber& operator--();
	NNumber operator++(int) {NNumber tmp(*this); ++(*this); return tmp;}
	NNumber operator--(int) {NNumber tmp(*this); --(*this); return tmp;}
	
	friend bool operator==(const NNumber&, const NNumber&);
	friend bool operator!=(const NNumber&, const NNumber&);
	friend bool operator<(const NNumber&, const NNumber&);
	friend bool operator<=(const NNumber&, const NNumber&);
	friend bool operator>(const NNumber&, const NNumber&);
	friend bool operator>=(const NNumber&, const NNumber&);
};

inline NNumber operator+(NNumber x, const NNumber& y) {x += y; return x;}
inline NNumber operator-(NNumber x, const NNumber& y) {x -= y; return x;}
inline NNumber operator*(NNumber x, const NNumber& y) {x *= y; return x;}
inline NNumber operator/(NNumber x, const NNumber& y) {x /= y; return x;}
inline NNumber operator%(NNumber x, const NNumber& y) {x %= y; return x;}

inline NNumber delta(const NNumber &x, const NNumber &y) {
	NNumber n;
	if (x.isUndef() || y.isUndef() || (x.isInf() && y.isInf())) n.setUndef();
	else if (x.isInf() || y.isInf()) n.setInf();
	else n = (x <= y) ? y - x : x - y;
	return n;
}

inline std::ostream& operator<<(std::ostream& stream, const NNumber& n) {
	stream << n.es(10);
	return stream;
}

#endif