// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Tokenizer_hpp
#define HEADER_Tokenizer_hpp

#include <filesystem>
#include "../Ecii.hpp"
#include "../Interpreter/BitSequence.hpp"
#include "../Interpreter/BuiltIn/NNumber.hpp"
#include "../Interpreter/Instruction.hpp"

inline const std::vector<Ecii::String> ProgvoKeywords = {"ERARO", "INKLUZIVI", "FINO", "NOMSPACO", "EKSTERA", "KLASO", "KONSTANTO", "KONSTRUILO", "SE", "ALIE", "RIPETI", "DUM", "HALTI", "LIVERI", "ELIRI", "VERA", "MALVERA"};
inline bool isProgvoKeyword(const Ecii::String &w) {return std::find(ProgvoKeywords.begin(), ProgvoKeywords.end(), w) != ProgvoKeywords.end();}

inline bool isSpecialSymbol(std::byte b) {
	uint8_t u(static_cast<uint8_t>(b));
	return u == 86 || u == 87 || (u >= 91 && u <= 101) || u == 118 || u == 119 || u == 122 || u == 123; // "<-", "->", "\", ",", ";", ".", ":", "[", "]", "(", ")", "{", "}", "'", """, "§", "#"
}
inline bool isNumberCharacter(std::byte b) {
	uint8_t u(static_cast<uint8_t>(b));
	return u == 11 || u == 90 || u == 94 || Ecii::isDigit(b); // "-", "/", "."
}

inline bool isIdentifierFirstCharacter(std::byte b) {return !Ecii::isSpace(b) && !Ecii::isDigit(b) && !Ecii::isSpecial(b) && !isSpecialSymbol(b);}
inline bool isIdentifierCharacter(std::byte b) {return !Ecii::isSpace(b) && !Ecii::isSpecial(b) && !isSpecialSymbol(b);}
inline bool isSlash(std::byte b) {return b == std::byte{90};} // "/"
inline bool isCommentStart(std::byte b) {return b == std::byte{122};} // "§"
inline bool isBitSequenceStart(std::byte b) {
	uint8_t u(static_cast<uint8_t>(b));
	return u == 96 || u == 123; // "[" , "#"
}
inline bool isDelimiter(std::byte b) {
	uint8_t u(static_cast<uint8_t>(b));
	return u == 86 || u == 87 || (u >= 92 && u <= 95) || (u >= 98 && u <= 101) || u == 118; // "<-", "->", ",", ";", ".", ":", "(", ")", "{", "}", "'"
}

struct Message {
	enum class Type {Log, Warning, Error};
	
	Type type;
	Ecii::String text, file;
	TextPos tp;
	
	Message() : type(Type::Log), text(""), file(""), tp(TextPos()) {}
	Message(const Type &t, const Ecii::String &s, const Ecii::String &f, const TextPos &tp0) : type(t), text(s), file(f), tp(tp0) {}
	
	Ecii::String es() const {
		Ecii::String str;
		if (type == Type::Error) str += "E";
		else if (type == Type::Warning) str += "A";
		else str += "I";
		str += " : En " + file + ":" + std::to_string(tp.line) + ":" + std::to_string(tp.column) + "\n";
		str += "    " + text + "\n";
		return str;
	}
};

struct ParsingData {
	std::vector<std::pair<std::filesystem::path, Ecii::String>> files;
	std::vector<Message> messages;
	
	ParsingData() {clear();}
	
	bool alreadyIncluded(const std::filesystem::path &path) const {
		for (const auto &f : files) {if (std::filesystem::equivalent(f.first, path)) return true;}
		return false;
	}
	
	void clear() {
		files = std::vector<std::pair<std::filesystem::path, Ecii::String>>();
		messages = std::vector<Message>();
	}
};

class Token {
public:
	enum class Type {Invalid, Identifier, Delimiter, Constant};
private:
	Type _type;
	Ecii::String _label;
	std::shared_ptr<Class> _class;
	std::shared_ptr<Data> _data;
	Ecii::String _file;
	TextPos _pos;
public:
	Token() : _type(Type::Invalid), _label("ERARO") {}
	Token(const Type &type, const Ecii::String &label, const Ecii::String &path, const TextPos &pos) :
		_type(type),
		_label(label),
		_class(nullptr),
		_data(nullptr),
		_file(path),
		_pos(pos) {}
	
	Token(const std::shared_ptr<Class>& c, const std::shared_ptr<Data>& data, const Ecii::String &path, const TextPos &pos) :
		_type(Type::Constant),
		_label("<Konstanto de klaso " + c->getName() + ">"),
		_class(c),
		_data(data),
		_file(path),
		_pos(pos) {}
	
	Ecii::String label() const {return _label;}
	std::shared_ptr<Class> getClass() const {return _class;}
	std::shared_ptr<Data> data() const {return _data;}
	Ecii::String file() const {return _file;}
	TextPos pos() const {return _pos;}
	
	bool isIdentifier() const {return _type == Type::Identifier;}
	bool isIdentifier(const Ecii::String &es) const {return _type == Type::Identifier && _label == es;}
	bool isDelimiter() const {return _type == Type::Delimiter;}
	bool isDelimiter(const Ecii::String &es) const {return _type == Type::Delimiter && _label == es;}
	bool isConstant() const {return _type == Type::Constant;}
};

class Tokenizer {
	std::shared_ptr<ParsingData> _parsingData;
	std::vector<Token> _tokens;
	std::filesystem::path _curDir;
	std::size_t _curPos;
	
	void _addFile(const std::filesystem::path &file, const Ecii::String &code) {
		_parsingData->files.push_back(std::make_pair(file, code));
	}
	
	void _addMessage(const Message::Type &t, const Ecii::String &s) {
		if (_parsingData->files.size() != 0)
			_parsingData->messages.push_back(Message(t, s, _parsingData->files.back().first.string(), _parsingData->files.back().second.toLineCol(_curPos)));
	}
	void _addMessage(const Message::Type &t, const Ecii::String &s, const Ecii::String &f, const TextPos &p) {
		_parsingData->messages.push_back(Message(t, s, f, p));
	}
	
	void _addToken(const Token::Type &t, const Ecii::String& l) {
		if (_parsingData->files.size() != 0)
			_tokens.push_back(Token(t, l, _parsingData->files.back().first.string(), _parsingData->files.back().second.toLineCol(_curPos)));
	}
	Token _makeToken(const std::shared_ptr<Class> &c, const std::shared_ptr<Data> &d) const {
		if (_parsingData->files.size() != 0)
			return Token(c, d, _parsingData->files.back().first.string(), _parsingData->files.back().second.toLineCol(_curPos));
		return Token(c, d, "Nedifinita", TextPos(0, 0));
	}
	Token _makeToken(const Token::Type &t, const Ecii::String& l) const {
		if (_parsingData->files.size() != 0)
			return Token(t, l, _parsingData->files.back().first.string(), _parsingData->files.back().second.toLineCol(_curPos));
		return Token(t, l, "Nedifinita", TextPos(0, 0));
	}
	
	void _parseComment();
	Token _parseBitSequence();
	Token _parseNumber();
	Token _parseEciiString();
	Token _parseIdentifier();
public:
	Tokenizer(const std::shared_ptr<ParsingData> &parsingData);
	
	void setCurDir(const std::filesystem::path &path) {_curDir = path;}
	std::vector<Token> tokenize(const std::filesystem::path&);
};

#endif