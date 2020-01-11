// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_BitSequence_hpp
#define HEADER_BitSequence_hpp

#include <array>
#include <vector>
#include <climits>
#include <cstddef>
#include "../main.hpp"

/*inline std::vector<std::byte> v8ToVb(std::vector<uint8_t> v8) {
	std::vector<std::byte> vb;
	for (const auto & u8 : v8) vb.push_back(std::byte{u8});
	return vb;
}

inline std::vector<uint8_t> vbToV8(std::vector<std::byte> vb) {
	std::vector<uint8_t> v8;
	for (const auto & b : vb) v8.push_back(std::to_integer<uint8_t>(b));
	return v8;
}*/

inline std::vector<std::byte> v8ToVb(std::vector<uint8_t> v8) {
	std::vector<std::byte> vb;
	for (const auto & u8 : v8) vb.push_back(std::byte{u8});
	return vb;
}

inline std::vector<uint8_t> vbToV8(std::vector<std::byte> vb) {
	std::vector<uint8_t> v8;
	for (const auto & b : vb) v8.push_back(std::to_integer<uint8_t>(b));
	return v8;
}

class BitSequence {
	std::vector<Word> _bits;
	uintmax_t _size;

public:
	BitSequence() :
		_bits(std::vector<Word>()),
		_size(0) {}
	BitSequence(bool b) :
		_bits({b ? 1ULL : 0ULL}),
		_size(1) {}
	BitSequence(bool b, uintmax_t count) :
		_bits(std::vector<Word>()),
		_size(0) {
		for (std::size_t i(0) ; i < count ; i++) add(b);
	}
	BitSequence(std::byte b) :
		_bits(std::vector<Word>()),
		_size(0) {add(b);}
	BitSequence(const std::vector<std::byte>& vb) : _size(0) {
		for (uintmax_t i(0) ; i < vb.size() ; i++) add(vb[i]);
	}
	BitSequence(const std::vector<Word>&, uintmax_t);
	BitSequence(const std::vector<Word>& vw) : BitSequence(vw, WORDSIZE*vw.size()) {}
	
	BitSequence(const Word &w) :
		_bits({w}),
		_size(WORDSIZE) {}
	BitSequence(const Real &r) :
		_size(sizeof(Real)*CHAR_BIT) {
		while (_bits.size()*WORDSIZE < _size) _bits.push_back(0);
		for (uint8_t i(0) ; i < sizeof(Real) ; i++)
			reinterpret_cast<char*>(_bits.data())[i] = const_cast<char*>(reinterpret_cast<const char*>(&r))[i];
	}
	
	void add(bool b) {
		_size++;
		if (_size > _bits.size()*WORDSIZE) _bits.push_back(0);
		if (b) _bits.back() |= 1ULL << ((_size - 1ULL) % WORDSIZE);
	}
	void add(const std::byte &b) {
		for (uint8_t i(0) ; i < CHAR_BIT ; i++) add(((1ULL << i) & static_cast<uint8_t>(b)) != 0);
	}
	void add(const Word &w) {
		for (uint8_t i(0) ; i < WORDSIZE ; i++) add(((1ULL << i) & w) != 0);
	}
	
	bool get(uintmax_t pos) const {
		if (pos >= _size) return false;
		else return (_bits[pos/WORDSIZE] & (1ULL << (pos % WORDSIZE))) == (1ULL << (pos % WORDSIZE));
	}
	
	void set(bool b, uintmax_t pos) {
		if (pos < _size) {
			if (b) _bits[pos/WORDSIZE] |= 1ULL << (pos % WORDSIZE);
			else _bits[pos/WORDSIZE] &= WORDMAX - (1ULL << (pos % WORDSIZE));
		}
	}
	
	std::vector<Word> vw() const {return _bits;}
	std::vector<std::byte> vbyte() const;
	std::vector<bool> vbool() const;
	uintmax_t size() const {return _size;}
	template <class T> T to() const {
		return ((T*) _bits.data())[0];
	}
	
	BitSequence& operator+=(BitSequence y);
	BitSequence operator~() const {
		BitSequence inv;
		for (uintmax_t i(0) ; i < _size ; i++) {inv.add(get(i) != true);}
		return inv;
	}
	
	friend bool operator==(const BitSequence&, const BitSequence&);
};

inline BitSequence operator+(BitSequence x, const BitSequence &y) {x += y; return x;}
inline bool operator!=(const BitSequence &x, const BitSequence &y) {return !(x == y);}

#endif