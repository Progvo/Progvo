// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Parser_hpp
#define HEADER_Parser_hpp

#include "Tokenizer.hpp"

class Parser {
	std::shared_ptr<ParsingData> _parsingData;
	std::vector<Token> _tokens;
	std::vector<Ecii::String> _curNamespace;
	std::shared_ptr<Environment> _environment;
	std::size_t _curToken;
	
	void _addMessage(const Message::Type &t, const Ecii::String &s, const Ecii::String &f, const TextPos &p) {_parsingData->messages.push_back(Message(t, s, f, p));}
	void _addMessage(const Message::Type &t, const Ecii::String &s) {_parsingData->messages.push_back(Message(t, s, _tokens[_curToken].file(), _tokens[_curToken].pos()));}
	
	Ecii::String _namespacePrefix() const;
	Ecii::String _withNamespacePrefix(const Ecii::String&) const;
	void _goAfterNextDelimiter(const std::byte&);
	bool _isValid(const std::vector<Instruction>&) const;
	bool _isValid(const List<ObjectProperties>&) const;
	
	std::vector<Instruction> _parseInstructionSequence(const std::shared_ptr<Environment>&, const bool &main = false, const bool &loop = false);
	Expression _parseExpression(const std::shared_ptr<Environment>&);
	void _parseExpressionAlt(const std::shared_ptr<Environment>&, std::vector<Instruction>&);
	ObjectProperties _parseDeclaration();
	List<ObjectProperties> _parseObjectPropertiesList();
	void _parseClass();
	void _parseClassDefinition(const std::shared_ptr<Class>&);
	void _parseFunction(const std::shared_ptr<Class>&);
	void _parseObjectDeclaration(const std::shared_ptr<Environment>&, std::vector<Instruction>&);
public:
	Parser() : _parsingData(std::make_shared<ParsingData>()) {reset();}
	
	void reset();
	std::shared_ptr<Function> parse(const std::filesystem::path &path);
	std::shared_ptr<ParsingData> getParsingData() {return _parsingData;}
};

#endif