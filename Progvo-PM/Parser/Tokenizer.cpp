// (c) 2019-2020 Pttn (https://Progvo.dev)

#include "../GUI/IDE.hpp"
#include "Tokenizer.hpp"

Tokenizer::Tokenizer(const std::shared_ptr<ParsingData> &parsingData) :
	_parsingData(parsingData),
	_tokens(std::vector<Token>()),
	_curDir(IDE::get()->getCurDir()),
	_curPos(0) {
	if (_parsingData == nullptr) BUG("Datumo de la analizo ne estas pravalorizita");
}

void Tokenizer::_parseComment() {
	const std::vector<std::byte> &code(_parsingData->files.back().second.vb());
	while (_curPos < code.size()) {
		if (Ecii::isEol(code[_curPos])) {
			_curPos++;
			break;
		}
		_curPos++;
	}
}

Token Tokenizer::_parseBitSequence() {
	const std::vector<std::byte> &code(_parsingData->files.back().second.vb());
	uint8_t type(0);
	
	if (_curPos >= code.size()) {
		BUG("Jam en la fino antau analizo de Bitsekvenco");
		return Token();
	}
	else if (code[_curPos] == std::byte{123}) { // #
		_curPos++;
		if (_curPos + 1 >= code.size()) {
			_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de Bitsekvenco");
			return Token();
		}
		else if (code[_curPos] == std::byte{1} && static_cast<uint8_t>(code[_curPos + 1]) <= 6) { // #1X[...]
			type = 10 + static_cast<uint8_t>(code[_curPos + 1]);
			_curPos += 2;
		}
		else if (static_cast<uint8_t>(code[_curPos]) >= 2 && static_cast<uint8_t>(code[_curPos]) <= 9) { // #X[...]
			type = static_cast<uint8_t>(code[_curPos]);
			_curPos++;
		}
		else if (code[_curPos] == std::byte{96}) type = 10; // #[...]
		else {
			_addMessage(Message::Type::Error, "Atendita <2-16>[ poste # por Bitsekvenco");
			return Token();
		}
	}
	else if (code[_curPos] == std::byte{96}) type = 127; // [
	else {
		BUG("Malvalida signo " << Ecii::eciiToChar(code[_curPos]) << " por la starto de Bitsekvenco");
		return Token();
	}
	
	if (_curPos >= code.size()) {
		_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de Bitsekvenco");
		return Token();
	}
	else if (code[_curPos] != std::byte{96}) {
		_addMessage(Message::Type::Error, "Atendita [ por Bitsekvenco, '" + Ecii::String(code[_curPos]) + "' trovita");
		return Token();
	}
	else {
		_curPos++;
		std::vector<std::byte> content;
		bool foundEnd(false);
		while (_curPos < code.size()) {
			if (code[_curPos] == std::byte{97}) { // "]"
				foundEnd = true;
				_curPos++;
				break;
			}
			else content.push_back(code[_curPos]);
			_curPos++;
		}
		
		if (!foundEnd) {
			_addMessage(Message::Type::Error, "Ne eblis trovi la finilon ] de la Bitsekvenco");
			return Token();
		}
		else {
			if (type == 127) {
				BitSequence bs;
				for (uint64_t i(0) ; i < content.size() ; i++) {
					if (content[i] == std::byte{1}) bs.add(true);
					else if (content[i] == std::byte{0}) bs.add(false);
					else {
						_addMessage(Message::Type::Error, "Malvalida duuma datumoj en la Bitsekvenco, '" + Ecii::String(content[i]) + "' trovita");
						return Token();
					}
				}
				return _makeToken(builtInBitSequence, std::make_shared<BuiltInData<BitSequence>>(bs));
			}
			else if (type >= 2 && type <= 16) {
				if (!NNumber::isValid(Ecii::String(content), type)) {
					_addMessage(Message::Type::Error, "Malvalida entjero en la Bitsekvenco");
					return Token();
				}
				return _makeToken(builtInBitSequence, std::make_shared<BuiltInData<BitSequence>>(NNumber(Ecii::String(content), type).bs()));
			}
			else {
				BUG("Malvalida Bitsekvenco tipo");
				return Token();
			}
		}
	}
}

Token Tokenizer::_parseEciiString() {
	const std::vector<std::byte> &code(_parsingData->files.back().second.vb());
	
	if (_curPos >= code.size()) {
		BUG("Jam en la fino anta" + Ecii::String(std::byte{41}) + " la analizo de Ekia " + Ecii::String(std::byte{19}) + "eno");
		return Token();
	}
	else if (code[_curPos] != std::byte{119}) { // "
		BUG("Malvalida signo " << Ecii::eciiToChar(code[_curPos]) << " por la starto de Ekia " + Ecii::String(std::byte{19}) + "eno");
		return Token();
	}
	else {
		_curPos++;
		std::vector<std::byte> content;
		bool foundEnd(false);
		while (_curPos < code.size()) {
			if (code[_curPos] == std::byte{91}) { // "\"
				_curPos++;
				if (_curPos + 1 >= code.size()) {
					_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de Ekia " + Ecii::String(std::byte{19}) + "eno");
					return Token();
				}
				else if (!Ecii::isHexDigit(code[_curPos]) || !Ecii::isHexDigit(code[_curPos + 1])) {
					_addMessage(Message::Type::Error, "Atendita du deksesumaj ciferoj (0-f) poste la eskapilo");
					return Token();
				}
				else {
					content.push_back(std::byte{static_cast<uint8_t>(static_cast<uint8_t>(16)*Ecii::toHexDigit(code[_curPos]) + Ecii::toHexDigit(code[_curPos + 1]))});
					_curPos++;
				}
			}
			else if (code[_curPos] == std::byte{119}) { // """
				foundEnd = true;
				_curPos++;
				break;
			}
			else content.push_back(code[_curPos]);
			_curPos++;
		}
		
		if (!foundEnd) {
			_addMessage(Message::Type::Error, "Ne eblis trovi la finilon \" de la Ekia " + Ecii::String(std::byte{19}) + "eno");
			return Token();
		}
		else
			return _makeToken(builtInEcii, std::make_shared<BuiltInData<Ecii::String>>(content));
	}
}

Token Tokenizer::_parseNumber() {
	const std::vector<std::byte> &code(_parsingData->files.back().second.vb());
	Ecii::String number;
	while (_curPos < code.size()) {
		if (isNumberCharacter(code[_curPos])) {
			number.add(code[_curPos]);
			_curPos++;
			if (_curPos == code.size()) goto parseAddress;
		}
		else {
			if (code[_curPos] == Ecii::charToEcii('N', false) || code[_curPos] == Ecii::charToEcii('n', false)) {
				_curPos++;
				if (!NNumber::isValid(number, 10)) {
					_addMessage(Message::Type::Error, "Malvalida natura nombro");
					return Token();
				}
				else {
					NNumber n(number, 10);
					if (code[_curPos - 1] == Ecii::charToEcii('N', false))
						return _makeToken(builtInNNumber, std::make_shared<BuiltInData<NNumber>>(n));
					else {
						if (!n.fitsSingleWord()) {
							_addMessage(Message::Type::Error, "Tro granda konstanto");
							 return Token();
						}
						else
							return _makeToken(builtInNatUInt, std::make_shared<BuiltInData<Word>>(n.firstWord()));
					}
				}
			}
			else if (code[_curPos] == Ecii::charToEcii('Z', false) || code[_curPos] == Ecii::charToEcii('z', false)) {
				_curPos++;
				if (!ZNumber::isValid(number, 10)) {
					_addMessage(Message::Type::Error, "Malvalida entjera nombro");
					return Token();
				}
				else {
					ZNumber z(number, 10);
					if (code[_curPos - 1] == Ecii::charToEcii('Z', false))
						return _makeToken(builtInZNumber, std::make_shared<BuiltInData<ZNumber>>(z));
					else {
						if (!z.n().fitsSingleWord()) {
							_addMessage(Message::Type::Error, "Tro granda konstanto");
							 return Token();
						}
						else {
							Word w(z.n().firstWord());
							if (z.sign()) {
								if (w > 9223372036854775808ULL)	{
									_addMessage(Message::Type::Error, "Tro granda konstanto");
							 		return Token();
								}
								else return _makeToken(builtInNatInt, std::make_shared<BuiltInData<Word>>(static_cast<Word>(-static_cast<int64_t>(w)))); // ...
							}
							else {
								if (w > 9223372036854775807ULL)	{
									_addMessage(Message::Type::Error, "Tro granda konstanto");
							 		return Token();
								}
								else return _makeToken(builtInNatInt, std::make_shared<BuiltInData<Word>>(w)); // ...
							}
						}
					}
				}
			}
			else if (code[_curPos] == Ecii::charToEcii('Q', false)) {
				_curPos++;
				if (!QNumber::isValid(number, 10)) {
					_addMessage(Message::Type::Error, "Malvalida racionala nombro");
					return Token();
				}
				else {
					QNumber q(number, 10);
					return _makeToken(builtInQNumber, std::make_shared<BuiltInData<QNumber>>(q));
				}
			}
			else if (code[_curPos] == Ecii::charToEcii('r', false)) {
				_curPos++;
				bool valid(true), hasPoint(false);
				for (std::size_t i(0) ; i < number.size() ; i++) {
					if (number.getChar(i) == std::byte{11} && i != 0) // "-"
						valid = false;
					if (number.getChar(i) == std::byte{94}) { // "."
						if (hasPoint || i == 0) valid = false;
						else if (number.getChar(i - 1) == std::byte{11}) valid = false;
						else hasPoint = true;
					}
					if (!valid) break;
				}
				if (!valid) {
					_addMessage(Message::Type::Error, "Malvalida reelo");
					return Token();
				}
				else
					return _makeToken(builtInNatReal, std::make_shared<BuiltInData<Real>>(std::stold(number.str().c_str())));
			}
			else {
parseAddress:
				if (!NNumber::isValid(number, 10)) {
					_addMessage(Message::Type::Error, "Malvalida natura nombro por adresmontrilo!");
					return Token();
				}
				else {
					NNumber n(number, 10);
					if (!n.fitsSingleWord()) {
						_addMessage(Message::Type::Error, "Tro granda adresmontrilo");
						return Token();
					}
					else
						return _makeToken(builtInPointer, std::make_shared<BuiltInData<Word>>(n.firstWord()));
				}
			}
		}
	}
	return Token();
}

Token Tokenizer::_parseIdentifier() {
	const std::vector<std::byte> &code(_parsingData->files.back().second.vb());
	Ecii::String identifier;
	for (; _curPos < code.size() ; _curPos++) {
		if (Ecii::isSpace(code[_curPos]) || isSpecialSymbol(code[_curPos])) break;
		else if (isIdentifierCharacter(code[_curPos])) identifier.add(code[_curPos]);
		else {
			_addMessage(Message::Type::Error, "Malvalida signo " + Ecii::String(code[_curPos]) + " por identigilo");
			return Token();
		}
	}
	if (identifier == "MALVERA" || identifier == "VERA")
		return _makeToken(builtInBoolean, std::make_shared<BuiltInData<bool>>(identifier == "VERA"));
	else return _makeToken(Token::Type::Identifier, identifier);
}

std::vector<Token> Tokenizer::tokenize(const std::filesystem::path &path) {
	std::vector<Token> nonExpandedTokens;
	const std::filesystem::path fullPath(_curDir/path);
	_curDir = fullPath.parent_path().string();
	const std::ifstream file(fullPath);
	if (!file)
		_addMessage(Message::Type::Error, "Ne eblis malfermi la dosieron \"" + fullPath.string() + "\"", _curDir.string(), TextPos(0, 0));
	else {
		const std::vector<std::byte> code(load(fullPath.string()));
		_addFile(fullPath.string(), code);
		_curPos = 0;
		while (_curPos < code.size()) {
			if (Ecii::isSpace(code[_curPos])) {_curPos++; continue;}
			else if (isCommentStart(code[_curPos]))
				_parseComment();
			else if (isBitSequenceStart(code[_curPos]))
				nonExpandedTokens.push_back(_parseBitSequence());
			else if (code[_curPos] == std::byte{119}) // """
				nonExpandedTokens.push_back(_parseEciiString());
			else if (isDelimiter(code[_curPos])) {
				nonExpandedTokens.push_back(_makeToken(Token::Type::Delimiter, Ecii::String(code[_curPos])));
				_curPos++;
			}
			else if (code[_curPos] == std::byte{11}) { // "−"
				if (_curPos + 1 < code.size() ? Ecii::isDigit(code[_curPos + 1]) : false)
					nonExpandedTokens.push_back(_parseNumber());
				else nonExpandedTokens.push_back(_parseIdentifier());
			}
			else if (Ecii::isDigit(code[_curPos]))
				nonExpandedTokens.push_back(_parseNumber());
			else if (isIdentifierFirstCharacter(code[_curPos]))
				nonExpandedTokens.push_back(_parseIdentifier());
			else {
				_addMessage(Message::Type::Error, "Neatendita signo " + Ecii::String(code[_curPos]));
				break;
			}
		}
		
		for (std::vector<Token>::size_type i(0) ; i < nonExpandedTokens.size() ; i++) {
			if (nonExpandedTokens[i].isIdentifier("INKLUZIVI")) {
				if (i + 2 >= nonExpandedTokens.size()) {
					_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de INKLUZIVI ordono", nonExpandedTokens[i].file(), nonExpandedTokens[i].pos());
					break;
				}
				else if (nonExpandedTokens[i + 1].getClass() != builtInEcii) {
					_addMessage(Message::Type::Error, "Atendita Ekia " + Ecii::String(std::byte{19}) + "eno poste INKLUZIVI", nonExpandedTokens[i].file(), nonExpandedTokens[i].pos());
					break;
				}
				else if (!nonExpandedTokens[i + 2].isDelimiter(";")) {
					_addMessage(Message::Type::Error, "Atendita ; poste INKLUZIVI ordono", nonExpandedTokens[i].file(), nonExpandedTokens[i].pos());
					break;
				}
				else {
					Ecii::String relativePathToInclude(*std::static_pointer_cast<Ecii::String>(nonExpandedTokens[i + 1].data()->data));
					std::filesystem::path pathToInclude;
					if (isSlash(relativePathToInclude.getChar(0))) {
						relativePathToInclude = relativePathToInclude.substr(1, relativePathToInclude.size() - 1);
						pathToInclude = IDE::get()->getIncDir()/relativePathToInclude.str();
					}
					else pathToInclude = _curDir/relativePathToInclude.str();
					pathToInclude.make_preferred();
					const std::ifstream includeFile(pathToInclude);
					if (!includeFile) {
						_addMessage(Message::Type::Error, "Ne eblis malfermi la inkluzivan dosieron " + pathToInclude.string(), nonExpandedTokens[i + 1].file(), nonExpandedTokens[i + 1].pos());
						break;
					}
					else if (!_parsingData->alreadyIncluded(pathToInclude)) {
						Tokenizer subTokenizer(_parsingData);
						subTokenizer.setCurDir(pathToInclude.parent_path());
						std::vector<Token> tokensToInclude(subTokenizer.tokenize(pathToInclude.filename()));
						for (const auto &t : tokensToInclude) _tokens.push_back(t);
					}
					i += 2;
				}
			}
			else _tokens.push_back(nonExpandedTokens[i]);
		}
	}
	
	return _tokens;
}