// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Ecii_hpp
#define HEADER_Ecii_hpp

#include <array>
#include <vector>
#include <string>

#define ES(c)	Ecii::String(std::byte{c})

struct TextPos {
	std::size_t line, column;
	TextPos() {line = 0; column = 0;}
	TextPos(std::size_t x0, std::size_t y0) {line = x0; column = y0;}
	std::string str() const {return "(" + std::to_string(line) + ", " + std::to_string(column) + ")";}
};

inline bool operator==(const TextPos tp1, const TextPos tp2) {return tp1.line == tp2.line && tp1.column == tp2.column;}
inline bool operator!=(const TextPos tp1, const TextPos tp2) {return !(tp1 == tp2);}
inline bool operator<(const TextPos tp1, const TextPos tp2) {
	if (tp1.line < tp2.line) return true;
	else if (tp1.line == tp2.line && tp1.column < tp2.column) return true;
	else return false;
}
inline bool operator<=(const TextPos tp1, const TextPos tp2) {return tp1 < tp2 || tp1 == tp2;}
inline bool operator>(const TextPos tp1, const TextPos tp2) {return !(tp1 <= tp2);}
inline bool operator>=(const TextPos tp1, const TextPos tp2) {return !(tp1 < tp2);}
#include <iostream>
namespace Ecii {
// See latest Ecii definition here https://Progvo.dev/Ekio.html
constexpr std::array<uint8_t, 128> asciiToEcii =
	{255, 252, 252, 252, 252, 252, 252, 252, 252, 253, 254, 252, 252, 254, 252, 252,
	 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
	  15, 120, 119, 123, 125, 116,  82, 118,  98,  99, 106,  10,  92,  11,  94,  90,
	   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  95,  93, 102,  14, 103, 121,
	 124,  48,  49,  50,  52,  53,  54,  55,  57,  59,  60,  62,  63,  64,  65,  66,
	  67,  76,  68,  69,  71,  72,  74,  77,  78,  79,  75,  96,  91,  97, 110, 127,
	 252,  16,  17,  18,  20,  21,  22,  23,  25,  27,  28,  30,  31,  32,  33,  34,
	  35,  44,  36,  37,  39,  40,  42,  45,  46,  47,  43, 100,  89, 101, 111, 252};
constexpr std::array<uint8_t, 128> asciiToEciiAlt =
	{252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
	 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
	 252,  80, 109, 252, 122, 117,  81, 108, 252, 252,  12, 252, 252, 126, 252,  13,
	 104, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,  86,  83,  87, 252,
	 252, 252, 252,  51, 113, 252, 252,  56,  58, 252,  61, 252, 252, 252, 252, 252,
	 252, 252, 252,  70, 252,  73, 252, 252, 252, 252, 252, 252, 252, 252,  85, 252,
	 252, 252, 252,  19, 252, 252, 252,  24,  26, 252,  29, 252, 252, 252, 252, 252,
	 252, 252, 252,  38, 252,  41,  84, 252, 252, 252, 252, 252, 252, 252, 252, 252};
constexpr char eciiToAscii[256] =
	{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '=', ' ',
	 'a', 'b', 'c', 'c', 'd', 'e', 'f', 'g', 'g', 'h', 'h', 'i', 'j', 'j', 'k', 'l',
	 'm', 'n', 'o', 'p', 'r', 's', 's', 't', 'u', 'u', 'v', 'z', 'q', 'w', 'x', 'y',
	 'A', 'B', 'C', 'C', 'D', 'E', 'F', 'G', 'G', 'H', 'H', 'I', 'J', 'J', 'K', 'L',
	 'M', 'N', 'O', 'P', 'R', 'S', 'S', 'T', 'U', 'U', 'V', 'Z', 'Q', 'W', 'X', 'Y',
	 '?', '&', '?', '=', '?', '?', '?', '?', '?', '|', '/','\\', ',', ';', '.', ':',
	 '[', ']', '(', ')', '{', '}', '<', '>', '?', '?', '*', '?', '\'','"', '^', '?',
	 '?', '?', '?', '?', '%', '?','\'', '"', '!', '?', '?', '#', '@', '$', '-', '_',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',
	 '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?','\t','\n','\0'};

std::byte charToEcii(char, bool = false);
std::vector<std::byte> strToEciiVb(std::string);
char eciiToChar(std::byte);
std::string eciiVbToStr(std::vector<std::byte>);

inline bool isBinary(std::byte b) {return (uint8_t) b < 2;}
inline bool isDigit(std::byte b) {return (uint8_t) b < 10;}
inline bool isHexDigit(std::byte b) {
	uint8_t u(static_cast<uint8_t>(b));
	return u < 10 || u == 16 || u == 17 || u == 18 || u == 20 || u == 21 || u == 22;
}
inline uint8_t toHexDigit(std::byte b) {
	uint8_t u(static_cast<uint8_t>(b));
	if (u < 10) return u;
	else if (u == 16) return 10;
	else if (u == 17) return 11;
	else if (u == 18) return 12;
	else if (u == 20) return 13;
	else if (u == 21) return 14;
	else if (u == 22) return 15;
	else return 0;
}
inline bool isSpace(std::byte b) {
	uint8_t u(static_cast<uint8_t>(b));
	return u == 15 || u == 253 || u == 254; // " ", "\t", "\n"
}
inline bool isLetter(std::byte b) {return (uint8_t) b >= 16 && (uint8_t) b < 80;}
inline bool isSpecial(std::byte b) {return (uint8_t) b > 127;}
inline bool isEol(std::byte b) {return b == std::byte{254};}
inline bool isEos(std::byte b) {return b == std::byte{255};}
inline bool isEolOrEos(std::byte b) {return isEol(b) || isEos(b);}

constexpr std::byte INV = std::byte{252};
constexpr std::byte EOL = std::byte{254};
constexpr std::byte EOS = std::byte{255};

class String {
	std::vector<std::vector<std::byte>> _lines;
public:
	String() : _lines(std::vector<std::vector<std::byte>>()) {}
	explicit String(uint8_t u8) : _lines(std::vector<std::vector<std::byte>>{std::vector<std::byte>{std::byte{u8}}}) {}
	String(std::byte b) : _lines(std::vector<std::vector<std::byte>>{std::vector<std::byte>{b}}) {}
	String(const std::vector<bool>&);
	String(const std::vector<std::byte>&);
	String(const std::string &str) : String(strToEciiVb(str)) {}
	String(const char *str) : String(std::string(str)) {}
	
	TextPos toLineCol(std::size_t) const;
	
	void put(std::byte, TextPos);
	void put(std::byte c, std::size_t pos) {put(c, toLineCol(pos));}
	void add(std::byte c) {
		if (_lines.size() == 0) put(c, TextPos(0, 0));
		else put(c, TextPos(_lines.size() - 1, _lines.back().size()));
	}
	void erase(TextPos);
	void erase(std::size_t pos) {erase(toLineCol(pos));}
	void eraseLine(std::size_t line) {if (line < _lines.size()) _lines.erase(_lines.begin() + line);}
	
	std::size_t size() const;
	std::size_t lineCount() const {return _lines.size();}
	
	std::vector<std::byte> getLine(std::size_t l) const {if (l < _lines.size()) return _lines[l]; else return {};}
	std::vector<std::vector<std::byte>> getLines() const {return _lines;}
	std::byte getChar(TextPos tp) const {
		if (tp.line < _lines.size()) {
			if (tp.column < _lines[tp.line].size()) return _lines[tp.line][tp.column];
			else return EOL;
		}
		else return EOS;
	}
	std::byte getChar(uint64_t pos) const {return getChar(toLineCol(pos));}
	void setChar(std::byte c, uint64_t pos) {
		const TextPos tp(toLineCol(pos));
		if (tp.line < _lines.size()) {
			if (tp.column < _lines[tp.line].size())
				_lines[tp.line][tp.column] = c;
		}
		else put(c, tp);
	}
	uint64_t find(std::byte c) const {
		for (uint64_t i(0) ; i < size() ; i++) {
			if (c == getChar(i)) return i;
		}
		return -1ULL;
	}
	
	std::vector<std::byte> vb() const;
	String substr(std::size_t, std::size_t) const;
	std::string str() const {return eciiVbToStr(vb());}
	
	bool equals(String es) const {return _lines == es._lines;}
	String& operator+=(const String&);
	bool comp(String es) const {
		std::vector<std::byte> vb1(vb()), vb2(es.vb());
		std::size_t minSize(std::min(vb1.size(), vb2.size()));
		for (std::size_t i(0) ; i < minSize ; i++) {
			if (static_cast<uint8_t>(vb1[i]) < static_cast<uint8_t>(vb2[i]))
				return true;
			else if (static_cast<uint8_t>(vb1[i]) > static_cast<uint8_t>(vb2[i]))
				return false;
		}
		return vb1.size() < vb2.size();
	}
};

inline bool operator==(const String es1, const String es2) {return es1.equals(es2);}
inline bool operator!=(const String es1, const String es2) {return !(es1 == es2);}
inline bool operator<(const String es1, const String es2) {return es1.comp(es2);}
inline std::ostream& operator<<(std::ostream& os, const String& es) {os << es.str(); return os;}
}

inline Ecii::String operator+(Ecii::String es1, const Ecii::String es2) {es1 += es2; return es1;}

#endif