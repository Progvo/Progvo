// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Tools_hpp
#define HEADER_Tools_hpp

#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include "main.hpp"
#include "Ecii.hpp"

std::vector<std::byte> load(const Ecii::String&);
void save(const std::vector<std::byte>&, const Ecii::String&);

double timeSince(const std::chrono::time_point<std::chrono::system_clock>&);

inline std::default_random_engine eng((std::random_device())());

inline Word rand(Word min, Word max) {
	if (min > max) std::swap(min, max);
	std::uniform_int_distribution<Word> urd(min, max);
	return urd(eng);
}
inline Real rand(Real min, Real max) {
	if (min > max) std::swap(min, max);
	std::uniform_real_distribution<Real> urd(min, max);
	return urd(eng);
}

template <class T> struct Vect2 {
	T x, y;
	Vect2() {x = T(); y = T();}
	Vect2(const T &x0, const T &y0) {x = x0; y = y0;}
};

template <class T> inline bool operator==(const Vect2<T> v1, const Vect2<T> v2) {return v1.x == v2.x && v1.y == v2.y;}

template <class T> class List {
protected:
	bool _isLeaf;
	T _data;
	std::vector<List<T>> _children;
	
public:
	List() : _isLeaf(false) {}
	List(const T &data) : _isLeaf(true), _data(data) {}
	List(const std::vector<T> &vdata) : _isLeaf(false) {
		for (const auto &data : vdata) _children.push_back(List(data));
	}
	
	void add(const T &t) {
		if (_isLeaf) {
			_children.push_back(*this);
			_isLeaf = false;
			_data = T();
			_children.push_back(List(t));
		}
		else if (_children.size() == 0)
			*this = List(t);
		else
			_children.push_back(List(t));
	}
	void add(const List &child) {
		if (child._isLeaf)
			add(child.data());
		else if (_isLeaf) {
			_children.push_back(*this);
			_isLeaf = false;
			_data = T();
			_children.push_back(child);
		}
		else
			_children.push_back(child);
	}
	
	bool isLeaf() const {return _isLeaf;}
	T data() const {
		if (isLeaf()) return _data;
		else return T();
	}
	
	bool has(T t) const {
		if (_isLeaf) return {t == _data};
		else {
			for (const auto &c : _children) {
				if (c.has(t)) return true;
			}
			return false;
		}
	}
	
	bool equals(const List<T> &tt) const {
		if (_isLeaf && tt._isLeaf) return _data == tt._data;
		else if (_isLeaf != tt._isLeaf) return false;
		else {
			if (_children.size() != tt._children.size()) return false;
			else {
				for (std::size_t i(0) ; i < _children.size() ; i++) {
					if (!_children[i].equals(tt._children[i]))
						return false;
				}
				return true;
			}
		}
	}
	
	std::vector<T> linearized() const {
		if (_isLeaf) return {_data};
		else {
			std::vector<T> vt;
			for (const auto &c : _children) {
				std::vector<T> subVt(c.linearized());
				for (const auto &t : subVt) vt.push_back(t);
			}
			return vt;
		}
	}
	
	const std::vector<List<T>>& children() const {return _children;}
	std::size_t size() const {
		if (_isLeaf) return 1;
		else {
			std::size_t s(0);
			for (const auto &child : _children) s += child.size();
			return s;
		}
	}
};

template <class T> bool operator==(const List<T> &t1, const List<T> &t2) {return t1.equals(t2);}
template <class T> bool operator!=(const List<T> &t1, const List<T> &t2) {return !(t1 == t2);}
template <class T> bool operator==(const List<T> &t1, const T &t) {return t1 == List(t);}
template <class T> bool operator!=(const List<T> &t1, const T &t) {return t1 != List(t);}

#endif