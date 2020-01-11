// (c) 2019 Pttn (https://Progvo.dev)

#include "Ecii.hpp"

namespace Ecii {
std::byte charToEcii(char c, bool alt) {
	if ((uint8_t) c > 127) return INV;
	else return (alt) ? std::byte{asciiToEciiAlt[c]} : std::byte{asciiToEcii[c]};
}

std::vector<std::byte> strToEciiVb(std::string str) {
	std::vector<std::byte> chars;
	for (uint64_t i(0) ; i < str.size() ; i++)
		chars.push_back(charToEcii(str[i], false));
	return chars;
}

char eciiToChar(std::byte c) {return (uint8_t) eciiToAscii[(uint8_t) c];}
std::string eciiVbToStr(std::vector<std::byte> v8) {
	std::string str;
	for (uint64_t i(0) ; i < v8.size() ; i++)
		str += eciiToAscii[(uint8_t) v8[i]];
	return str;
}

String::String(const std::vector<bool> &bools) {
	std::vector<std::byte> line;
	for (const auto &b : bools) 
		line.push_back(b ? std::byte{1} : std::byte{0});
	_lines.push_back(line);
}

String::String(const std::vector<std::byte> &chars) {
	std::vector<std::byte> line;
	for (uint64_t i(0) ; i < chars.size() ; i++) {
		if (isEos(chars[i])) break;
		else if (isEol(chars[i])) {
			_lines.push_back(line);
			line = std::vector<std::byte>();
		}
		else line.push_back(chars[i]);
	}
	_lines.push_back(line);
}

TextPos String::toLineCol(std::size_t pos) const {
	TextPos tp(0, 0);
	std::size_t posTmp(pos);
	for (std::size_t i(0) ; i < _lines.size() ; i++) {
		if (posTmp < _lines[i].size()) {
			tp.column = posTmp;
			break;
		}
		else if (i + 1 == _lines.size()) {
			tp.column = _lines[i].size();
			break;
		}
		else {
			tp.line++;
			posTmp -= _lines[i].size();
			if (i > 0) posTmp--; // EOL
		}
	}
	return tp;
}

void String::put(std::byte c, TextPos tp) {
	if (tp.line >= _lines.size()) tp.line = _lines.size() - 1;
	if (!isEos(c)) {
		if (isEol(c)) {
			if (_lines.size() == 0) _lines = {std::vector<std::byte>(), std::vector<std::byte>()};
			else {
				_lines.insert(_lines.begin() + tp.line + 1, std::vector<std::byte>());
				_lines[tp.line + 1] = std::vector<std::byte>(&_lines[tp.line][tp.column], &_lines[tp.line][_lines[tp.line].size()]);
				_lines[tp.line] = std::vector<std::byte>(&_lines[tp.line][0], &_lines[tp.line][tp.column]);
			}
		}
		else {
			if (_lines.size() == 0) _lines.push_back(std::vector<std::byte>({c}));
			else if (tp.column >= _lines[tp.line].size()) _lines[tp.line].push_back(c);
			else _lines[tp.line].insert(_lines[tp.line].begin() + tp.column, c);
		}
	}
}

void String::erase(TextPos tp) {
	if (tp.line < _lines.size()) {
		if (tp.column != 0 && _lines[tp.line].size() != 0) {
			if (tp.column >= _lines[tp.line].size()) _lines[tp.line].pop_back();
			else _lines[tp.line].erase(_lines[tp.line].begin() + tp.column - 1);
		}
		else {
			if (tp.line != 0) {
				for (const auto &b : _lines[tp.line]) _lines[tp.line - 1].push_back(b);
				eraseLine(tp.line);
			}
		}
	}
}

std::size_t String::size() const {
	std::size_t s(0);
	if (_lines.size() > 1) s = _lines.size() - 1; // Count EOLs
	for (const auto &line : _lines) s += line.size();
	return s;
}

std::vector<std::byte> String::vb() const {
	std::vector<std::byte> vb;
	for (uint64_t i(0) ; i < _lines.size() ; i++) {
		vb.insert(std::end(vb), std::begin(_lines[i]), std::end(_lines[i]));
		if (i != _lines.size() - 1) vb.push_back(EOL);
	}
	return vb;
}

String String::substr(std::size_t start, std::size_t end) const {
	String es;
	if (size() != 0) {
		if (start < size()) {
			if (end >= size()) end = size() - 1;
			for (std::size_t i(start) ; i <= end ; i++) es.add(getChar(i));
		}
	}
	return es;
}

String& String::operator+=(const String& es) {
	if (lineCount() == 0) _lines.push_back(std::vector<std::byte>());
	if (es.lineCount() > 0) for (const auto &b : es._lines.front()) _lines.back().push_back(b);
	for (uint64_t i(1) ; i < es.lineCount() ; i++) _lines.push_back(es._lines[i]);
	return *this;
}

}