// (c) 2019 Pttn (https://Progvo.dev)

#include <iostream>
#include "BitSequence.hpp"

BitSequence::BitSequence(const std::vector<Word>& vb, uintmax_t size) {
	if (size > WORDSIZE*vb.size()) size = WORDSIZE*vb.size();
	for (uintmax_t i(0) ; i < size/WORDSIZE ; i++) _bits.push_back(vb[i]);
	_size = WORDSIZE*_bits.size();
	for (uint8_t i(0) ; i < size % WORDSIZE ; i++) {
		add((vb[size/WORDSIZE] & (1ULL << i)) != 0);
	}
}

std::vector<std::byte> BitSequence::vbyte() const {
	std::vector<std::byte> vbyte;
	for (uintmax_t i(0) ; i < _size/8 ; i++) vbyte.push_back(reinterpret_cast<const std::byte*>(_bits.data())[i]);
	return vbyte;
}

std::vector<bool> BitSequence::vbool() const {
	std::vector<bool> vbool;
	for (uintmax_t i(0) ; i < _size ; i++) vbool.push_back(get(i));
	return vbool;
}

BitSequence& BitSequence::operator+=(BitSequence y) {
	for (Word i(0) ; i < y.size() ; i++) add(y.get(i));
	return *this;
}

bool operator==(const BitSequence &x, const BitSequence &y) {
	if (x._size != y._size) return false;
	else {
		for (uintmax_t i(0) ; i < x._size/WORDSIZE ; i++)
			if (x._bits[i] != y._bits[i]) return false;
		for (uint8_t i(0) ; i < x._size % WORDSIZE ; i++)
			if ((x._bits[x._size/WORDSIZE] & (1ULL << i)) != (y._bits[x._size/WORDSIZE] & (1ULL << i))) return false;
		return true;
	}
}