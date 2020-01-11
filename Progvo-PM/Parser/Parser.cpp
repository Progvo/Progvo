// (c) 2019-2020 Pttn (https://Progvo.dev)

#include "../Interpreter/BuiltIn/BuiltIn.hpp"
#include "Parser.hpp"

Ecii::String Parser::_namespacePrefix() const {
	Ecii::String nsp;
	for (std::size_t i(0) ; i < _curNamespace.size() ; i++) {
		nsp += _curNamespace[i];
		nsp += "/";
	}
	return nsp;
}
Ecii::String Parser::_withNamespacePrefix(const Ecii::String &es) const {
	if (es.size() > 0) {
		if (isSlash(es.getChar(0)))
			return es.substr(1, es.size() - 1);
	}
	return _namespacePrefix() + es;
}
void Parser::_goAfterNextDelimiter(const std::byte &c) {
	for (; _curToken < _tokens.size() ; _curToken++) {
		if (_tokens[_curToken].isDelimiter(std::vector<std::byte>{c})) {
			_curToken++;
			break;
		}
	}
}

bool Parser::_isValid(const std::vector<Instruction> &instructionSequence) const {
	for (const auto &i : instructionSequence) {
		if (i.getType() == Instruction::Type::Invalid)
			return false;
	}
	return true;
}

bool Parser::_isValid(const List<ObjectProperties> &declarationList) const {
	const std::vector<ObjectProperties> declarations(declarationList.linearized());
	std::vector<Ecii::String> names;
	for (const auto &d : declarations) {
		if (d.getClass() == nullptr) return false;
		for (const auto &n : names) {
			if (n == d.getName())
				return false;
		}
		names.push_back(d.getName());
	}
	return true;
}

std::vector<Instruction> Parser::_parseInstructionSequence(const std::shared_ptr<Environment> &localEnvironment, const bool &main, const bool &loop) {
	std::vector<Instruction> instructions;
	if (_curToken >= _tokens.size())
		BUG("Jam en la fino anta" + ES(41) + " la analizo de instrukcisekvenco");
	else if (!main && !_tokens[_curToken].isDelimiter("{"))
		_addMessage(Message::Type::Error, "Atendita { anta" + ES(41) + " instrukcisekvenco");
	else {
		if (!main) _curToken++;
		while (_curToken < _tokens.size()) {
			if (_tokens[_curToken].isIdentifier()) {
				if (isProgvoKeyword(_tokens[_curToken].label())) {
					if (_tokens[_curToken].isIdentifier("NOMSPACO")) {
						_curToken++;
						if (!main)
							_addMessage(Message::Type::Error, "NOMSPACO povas nur esti uzata en la " + ES(19) + "efa instrukcisekvenco");
						else if (_curToken + 1 >= _tokens.size())
							_addMessage(Message::Type::Error, "Neatendita fino poste NOMSPACO");
						else if (!_tokens[_curToken].isIdentifier())
							_addMessage(Message::Type::Error, "Atendita identigilo poste NOMSPACO");
						else if (!_tokens[_curToken + 1].isDelimiter(";"))
							_addMessage(Message::Type::Error, "Atendita ; poste la identigilo de NOMSPACO");
						else
							_curNamespace.push_back(_tokens[_curToken].label());
					}
					else if (_tokens[_curToken].isIdentifier("EKSTERA")) {
						if (_curToken + 1 >= _tokens.size())
							_addMessage(Message::Type::Error, "Neatendita fino poste EKSTERA");
						else if (!_tokens[_curToken + 1].isIdentifier())
							_addMessage(Message::Type::Error, "Atendita identigilo poste EKSTERA");
						else {
							const Ecii::String identifier(_tokens[_curToken + 1].label());
							if (isProgvoKeyword(identifier)) {
								if (identifier == "KLASO")
									_parseClass();
								else
									_addMessage(Message::Type::Error, "Neatendita " + ES(38) + "losilvorto " + identifier + " poste EKSTERA");
							}
							else
								_parseFunction(nullptr);
						}
					}
					else if (_tokens[_curToken].isIdentifier("KLASO"))
						_parseClass();
					else if (_tokens[_curToken].isIdentifier("FINO")) {
						if (_curToken + 2 >= _tokens.size())
							_addMessage(Message::Type::Error, "Neatendita fino poste FINO");
						else {
							if (!_tokens[_curToken + 1].isIdentifier())
								_addMessage(Message::Type::Error, "Atendita nomo poste FINO");
							else if (!_tokens[_curToken + 2].isDelimiter(";"))
								_addMessage(Message::Type::Error, "Atendita ; poste FINO nomo");
							else {
								if (_tokens[_curToken + 1].isIdentifier("NOMSPACO")) {
									if (!main) {
										_addMessage(Message::Type::Error, "NOMSPACO povas nur esti uzata en la " + ES(19) + "efa instrukcisekvenco");
										return {Instruction()};
									}
									else if (_curNamespace.size() == 0)
										_addMessage(Message::Type::Error, "Jam en " + ES(19) + "efa nomspaco");
									else _curNamespace.pop_back();
								}
								else
									_addMessage(Message::Type::Error, "Neatendita nomo " + _tokens[_curToken + 1].label() + " poste FINO");
							}
						}
					}
					else if (_tokens[_curToken].isIdentifier("HALTI")) {
						_curToken++;
						if (_curToken >= _tokens.size())
							_addMessage(Message::Type::Error, "Neatendita fino poste HALTI");
						else if (!_tokens[_curToken].isDelimiter(";"))
							_addMessage(Message::Type::Error, "Atendita ; poste HALTI");
						else {
							Instruction instruction;
							instruction.setStop();
							instructions.push_back(instruction);
						}
					}
					else if (_tokens[_curToken].isIdentifier("LIVERI")) {
						_curToken++;
						if (_curToken >= _tokens.size())
							_addMessage(Message::Type::Error, "Neatendita fino poste LIVERI");
						else if (!_tokens[_curToken].isDelimiter(";"))
							_addMessage(Message::Type::Error, "Atendita ; poste LIVERI");
						else {
							Instruction instruction;
							instruction.setReturn();
							instructions.push_back(instruction);
						}
					}
					else if (_tokens[_curToken].isIdentifier("ELIRI")) {
						if (!loop) {
							_addMessage(Message::Type::Error, "ELIRI povas nur esti en instrukcisekvenco de iteracio");
							return {Instruction()};
						}
						else {
							_curToken++;
							if (_curToken >= _tokens.size())
								_addMessage(Message::Type::Error, "Neatendita fino poste ELIRI");
							else if (!_tokens[_curToken].isDelimiter(";"))
								_addMessage(Message::Type::Error, "Atendita ; poste ELIRI");
							else {
								Instruction instruction;
								instruction.setBreak();
								instructions.push_back(instruction);
							}
						}
					}
					else if (_tokens[_curToken].isIdentifier("SE")) {
						_curToken++;
						const Expression condition(_parseExpression(localEnvironment));
						if (localEnvironment->typeOf(condition) != builtInBoolean) {
							_addMessage(Message::Type::Error, "SE kondicho ne taksas al Buleo");
							return {Instruction()};
						}
						else {
							const std::vector<Instruction> is(_parseInstructionSequence(localEnvironment, false, loop));
							if (!_isValid(instructions))
								return {Instruction()};
							else {
								Instruction instruction;
								instructions.push_back(instruction);
								instructions.back().addCondition(condition, is);
								continue;
							}
						}
					}
					else if (_tokens[_curToken].isIdentifier("ALIE")) {
						if (instructions.size() == 0) {
							_addMessage(Message::Type::Error, "Ne povas havi ALIE en la starto de instrukcisekvenco");
							return {Instruction()};
						}
						else if (instructions.back().getType() != Instruction::Type::Condition) {
							_addMessage(Message::Type::Error, "ALIE sen antaŭa SE");
							return {Instruction()};
						}
						else if (instructions.back().getConditionStructure().back().first.isEmpty()) {
							_addMessage(Message::Type::Error, "ALIE jam cheestanta");
							return {Instruction()};
						}
						else {
							Expression condition;
							if (_tokens[_curToken + 1].isIdentifier("SE")) {
								_curToken += 2;
								condition = _parseExpression(localEnvironment);
								if (localEnvironment->typeOf(condition) != builtInBoolean) {
									_addMessage(Message::Type::Error, "ALIE SE kondicho ne taksas al Buleo");
									return {Instruction()};
								}
							}
							else {
								_curToken++;
								condition.setEmpty();
							}
							const std::vector<Instruction> is(_parseInstructionSequence(localEnvironment, false, loop));
							if (!_isValid(instructions))
								return {Instruction()};
							else {
								instructions.back().addCondition(condition, is);
								continue;
							}
						}
					}
					else if (_tokens[_curToken].isIdentifier("RIPETI")) {
						Expression condition;
						condition.setEmpty();
						if (_tokens[_curToken + 1].isIdentifier("DUM")) {
							_curToken += 2;
							condition = _parseExpression(localEnvironment);
							if (localEnvironment->typeOf(condition) != builtInBoolean) {
								_addMessage(Message::Type::Error, "RIPETI DUM kondicho ne taksas al Buleo");
								return {Instruction()};
							}
						}
						else
							_curToken++;
						const std::vector<Instruction> is(_parseInstructionSequence(localEnvironment, false, true));
						if (!_isValid(instructions))
							return {Instruction()};
						else {
							Instruction instruction;
							instruction.setLoop(condition, is);
							instructions.push_back(instruction);
							continue;
						}
					}
					else if (_tokens[_curToken].isIdentifier("KONSTANTO")) {
						_parseObjectDeclaration(localEnvironment, instructions);
					}
					else {
						_addMessage(Message::Type::Error, "Neatendita " + ES(37) + "hlosilvorto " + _tokens[_curToken].label());
						return {Instruction()};
					}
				}
				else {
					if (_curToken + 1 >= _tokens.size())
						_addMessage(Message::Type::Error, "Neatendita fino poste " + _tokens[_curToken].label());
					else if (_tokens[_curToken + 1].isDelimiter(":")) {
						_parseFunction(nullptr);
						continue;
					}
					else if (_tokens[_curToken + 1].isIdentifier() || _tokens[_curToken + 1].isDelimiter("'")) {
						_parseObjectDeclaration(localEnvironment, instructions);
					}
					else
						_parseExpressionAlt(localEnvironment, instructions);
				}
			}
			else if (_tokens[_curToken].isDelimiter()) {
				if (_tokens[_curToken].isDelimiter(";"));
				else if (_tokens[_curToken].isDelimiter("}")) {
					if (main) {
						_addMessage(Message::Type::Error, "Neatendita } por la " + ES(19) + "efa instrukcisekvenco");
						return {Instruction()};
					}
					else {
						_curToken++;
						break;
					}
				}
				else
					_parseExpressionAlt(localEnvironment, instructions);
			}
			else if (_tokens[_curToken].isConstant())
				_parseExpressionAlt(localEnvironment, instructions);
			else {
				_addMessage(Message::Type::Error, "Neatendita " + ES(29) + "etono " + _tokens[_curToken].label());
				return {Instruction()};
			}
			_goAfterNextDelimiter(std::byte{93}); // ";"
		}
	}
	
	const std::vector<std::shared_ptr<Class>> incompleteClasses(localEnvironment->getIncompleteClasses());
	if (incompleteClasses.size() != 0) {
		Ecii::String errorMessage("Klaso(j) ");
		for (std::vector<std::shared_ptr<Class>>::size_type i(0) ; i < incompleteClasses.size() ; i++) {
			errorMessage += incompleteClasses[i]->getName();
			if (i + 1 < incompleteClasses.size()) errorMessage += ", ";
		}
		errorMessage += " estas malkompleta(j)";
		_addMessage(Message::Type::Error, errorMessage, _tokens[_curToken - 1].file(), _tokens[_curToken - 1].pos());
		return {Instruction()};
	}
	const std::vector<std::shared_ptr<Function>> incompleteFunctions(localEnvironment->getIncompleteFunctions());
	if (incompleteFunctions.size() != 0) {
		Ecii::String errorMessage("Funkcio(j) ");
		for (std::vector<std::shared_ptr<Function>>::size_type i(0) ; i < incompleteFunctions.size() ; i++) {
			errorMessage += incompleteFunctions[i]->getName();
			if (i + 1 < incompleteFunctions.size()) errorMessage += ", ";
		}
		errorMessage += " estas malkompleta(j)";
		_addMessage(Message::Type::Error, errorMessage, _tokens[_curToken - 1].file(), _tokens[_curToken - 1].pos());
		return {Instruction()};
	}
	return instructions;
}

Expression Parser::_parseExpression(const std::shared_ptr<Environment> &environment) {
	Expression expression;
	std::shared_ptr<Class> currentClass(nullptr);
	while (_curToken < _tokens.size()) {
		if (currentClass == nullptr) {
			if (_tokens[_curToken].isConstant()) {
				expression.setConstant(_tokens[_curToken].getClass(), _tokens[_curToken].data());
				_curToken++;
				if (_curToken >= _tokens.size() ? true : !_tokens[_curToken].isDelimiter("."))
					break;
				currentClass = _tokens[_curToken - 1].getClass();
				_curToken++;
			}
			else if (_tokens[_curToken].isIdentifier()) {
				const Ecii::String identifier(_tokens[_curToken].label());
				if (environment->isUsedFunctionName(identifier) || environment->getClass(identifier) != nullptr) {
					_curToken++;
					if (_curToken >= _tokens.size()) {
						_addMessage(Message::Type::Error, "Neatendita fino poste funkcio a" + ES(41) + " konstruilo " + identifier);
						return Expression();
					}
					else if (!_tokens[_curToken].isDelimiter("(")) {
						_addMessage(Message::Type::Error, "Atendita ( poste funkcio a" + ES(41) + " konstruilo " + identifier);
						return Expression();
					}
					else {
						Expression input(_parseExpression(environment));
						if (!input.isValid())
							return Expression();
						else {
							const Type inputType(environment->typeOf(input));
							std::shared_ptr<Function> function(nullptr);
							const std::shared_ptr<Class> c(environment->getClass(identifier));
							if (c != nullptr) {
								function = c->getMethod(identifier, inputType);
								if (function == nullptr) {
									_addMessage(Message::Type::Error, "Konstruilo " + identifier + inputType.es() + " ne estas deklarita");
									return Expression();
								}
							}
							else {
								function = environment->getFunction(identifier, inputType);
								if (function == nullptr) {
									_addMessage(Message::Type::Error, "Funkcio " + identifier + inputType.es() + " ne estas deklarita");
									return Expression();
								}
							}
							expression.setCall(function, input);
							if (_curToken >= _tokens.size() ? true : !_tokens[_curToken].isDelimiter("."))
								break;
							else {
								const Type outputType(function->getOutputType());
								if (!outputType.isSimple()) {
									_addMessage(Message::Type::Error, "Ne povas uzi . kun malsimpla tipo " + outputType.es());
									return Expression();
								}
								currentClass = outputType.getSimpleClass();
								_curToken++;
							}
						}
					}
				}
				else if (environment->getObject(identifier) != nullptr) {
					const std::shared_ptr<ObjectProperties> obj(environment->getObject(identifier));
					expression.setObjectPos(environment->getObjectPos(identifier));
					_curToken++;
					if (!_tokens[_curToken].isDelimiter("."))
						break;
					currentClass = obj->getClass();
					_curToken++;
				}
				else {
					_addMessage(Message::Type::Error, _tokens[_curToken].label() + " ne estas deklarita");
					return Expression();
				}
			}
			else if (_tokens[_curToken].isDelimiter("(")) {
				expression.setEmpty();
				_curToken++;
				if (_curToken >= _tokens.size()) {
					_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de listo de esprimo");
					return Expression();
				}
				else if (_tokens[_curToken].isDelimiter(")")) { // Empty List ()
					_curToken++;
					return expression;
				}
				else {
					while (_curToken < _tokens.size()) {
						const Expression subExpression(_parseExpression(environment));
						if (!subExpression.isValid())
							return Expression();
						else {
							if (_curToken >= _tokens.size()) {
								_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de arbo");
								return Expression();
							}
							else if (!_tokens[_curToken].isDelimiter(",") && !_tokens[_curToken].isDelimiter(")")) {
								_addMessage(Message::Type::Error, "Atendita , a" + ES(41) + " ) poste subesprimo");
								return Expression();
							}
							else {
								expression.addSubExpression(std::move(subExpression));
								_curToken++;
								if (_tokens[_curToken - 1].isDelimiter(")"))
									return expression;
							}
						}
					}
				}
			}
			else {
				_addMessage(Message::Type::Error, "Atendita konstanto, objekto a" + ES(41) + " ( por esprimo");
				return Expression();
			}
		}
		else {
			if (!_tokens[_curToken].isIdentifier()) {
				_addMessage(Message::Type::Error, "Atendita identigilo poste .");
				return Expression();
			}
			else {
				const Ecii::String memberName(_tokens[_curToken].label());
				if (currentClass->isAttribute(memberName)) {
					uintmax_t attributePos(currentClass->getAttributePos(memberName));
					currentClass = currentClass->getAttribute(memberName).getClass();
					Expression e;
					e.setAttributeAccess(attributePos);
					expression.addAccess(e);
					_curToken++;
					if (!_tokens[_curToken].isDelimiter("."))
						return expression;
					_curToken++;
				}
				else if (currentClass->isMethod(memberName)) {
					_curToken++;
					if (!_tokens[_curToken].isDelimiter("(")) {
						_addMessage(Message::Type::Error, "Atendita ( poste metodo");
						return Expression();
					}
					else {
						const Expression input(_parseExpression(environment));
						const std::shared_ptr<Function> function(currentClass->getMethod(memberName, environment->typeOf(input)));
						if (function == nullptr) {
							_addMessage(Message::Type::Error, "Metodo " + memberName.str() + " prenanta enigo de tipo " + environment->typeOf(input).es() + " ne estas membro de " + currentClass->getName().str());
							return Expression();
						}
						else {
							Expression methodCall;
							methodCall.setCall(currentClass->getMethod(memberName, environment->typeOf(input)), input);
							expression.addAccess(methodCall);
							if (!_tokens[_curToken].isDelimiter("."))
								break;
							else {
								Type outputType(function->getOutputType());
								if (!outputType.isSimple()) {
									_addMessage(Message::Type::Error, "Ne povas uzi . kun malsimpla tipo " + outputType.es());
									return Expression();
								}
								currentClass = outputType.getSimpleClass();
								_curToken++;
							}
						}
					}
				}
				else {
					_addMessage(Message::Type::Error, memberName.str() + " ne estas membro de " + currentClass->getName().str());
					return Expression();
				}
			}
		}
	}
	return expression;
}

void Parser::_parseExpressionAlt(const std::shared_ptr<Environment> &localEnvironment, std::vector<Instruction> &instructions) {
	Expression expression(_parseExpression(localEnvironment));
	if (!expression.isValid())
		instructions = {Instruction()};
	else if (_curToken >= _tokens.size())
		_addMessage(Message::Type::Error, "Neatendita fino poste esprimo");
	else if (_tokens[_curToken].isDelimiter(";")) {
		Instruction instruction;
		instruction.setEvaluation(std::move(expression));
		instructions.push_back(instruction);
	}
	else if (_tokens[_curToken].isDelimiter(std::byte{86})) { // ←
		if (!localEnvironment->isAssignable(expression))
			_addMessage(Message::Type::Error, "La esprimo ne estas asignebla");
		else {
			_curToken++;
			Expression input(_parseExpression(localEnvironment));
			Type outputType(localEnvironment->typeOf(expression)), inputType(localEnvironment->typeOf(input));
			if (outputType != inputType)
				_addMessage(Message::Type::Error, "La esprimo (de tipo " + inputType.es() + ") malkongruas la objekto(j)n (de tipo " + outputType.es() + ") por la asigno");
			else {
				Instruction instruction;
				instruction.setAssignment(expression.toObjectLocations(), input);
				instructions.push_back(instruction);
			}
		}
	}
	else
		_addMessage(Message::Type::Error, "Atendita ; a" + ES(41) + " " + ES(86) + " poste esprimo, '" + _tokens[_curToken].label() + "' trovita");
}

ObjectProperties Parser::_parseDeclaration() {
	if (_curToken + 1 >= _tokens.size()) {
		_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de deklaro");
		return ObjectProperties();
	}
	else {
		bool constant(false), reference(false);
		if (isProgvoKeyword(_tokens[_curToken].label())) {
			if (_tokens[_curToken].label() != "KONSTANTO") {
				_addMessage(Message::Type::Error, "Neatendita " + ES(38) + "losilvorto " + _tokens[_curToken].label());
				return ObjectProperties();
			}
			else {
				constant = true;
				_curToken++;
				if (_curToken + 1 >= _tokens.size()) {
					_addMessage(Message::Type::Error, "Neatendita fino poste KONSTANTO");
					return ObjectProperties();
				}
			}
		}
		const Ecii::String className(_withNamespacePrefix(_tokens[_curToken].label()));
		_curToken++;
		if (isProgvoKeyword(className)) {
			_addMessage(Message::Type::Error, "Ne povas uzi " + ES(37) + "hlosilvorto " + className + " al klasnomo");
			return ObjectProperties();
		}
		else if (_environment->getClass(className) == nullptr) {
			_addMessage(Message::Type::Error, "Klaso " + className + " ne estas deklarita");
			return ObjectProperties();
		}
		Ecii::String objectName;
		if (_tokens[_curToken].isDelimiter("'")) {
			reference = true;
			_curToken++;
			if (_curToken + 1 >= _tokens.size()) {
				_addMessage(Message::Type::Error, "Neatendita fino poste '");
				return ObjectProperties();
			}
		}
		
		if (_tokens[_curToken].isIdentifier())
			objectName = _tokens[_curToken].label();
		else {
			_addMessage(Message::Type::Error, "Atendita ' a" + ES(41) + " objektnomo poste klasnomo");
			return ObjectProperties();
		}
		_curToken++;
		if (isProgvoKeyword(objectName)) {
			_addMessage(Message::Type::Error, "Ne povas uzi " + ES(38) + "losilvorto " + objectName + " al objektnomo");
			return ObjectProperties();
		}
		else return ObjectProperties(_environment->getClass(className), objectName, constant, reference);
	}
}

List<ObjectProperties> Parser::_parseObjectPropertiesList() {
	if (_curToken >= _tokens.size()) {
		_addMessage(Message::Type::Error, "Neatendita fino al la starto de listo de deklaroj");
		return ObjectProperties();
	}
	else {
		if (!_tokens[_curToken].isDelimiter("(")) {
			_addMessage(Message::Type::Error, "Atendita ( por listo de deklaroj");
			return ObjectProperties();
		}
		else {
			_curToken++;
			List<ObjectProperties> objectPropertiesList;
			if (_tokens[_curToken].isDelimiter(")")) // Empty List ()
				_curToken++;
			else {
				while (_curToken < _tokens.size()) {
					if (_curToken + 1 >= _tokens.size()) {
						_addMessage(Message::Type::Error, "Neatendita fino dum la analizo de listo de deklaroj");
						return ObjectProperties();
					}
					else if (!_tokens[_curToken].isDelimiter("(") && (!_tokens[_curToken].isIdentifier() || (!_tokens[_curToken + 1].isIdentifier() && !_tokens[_curToken + 1].isDelimiter("'")))) {
						_addMessage(Message::Type::Error, "Atendita klaso kaj objekto a" + ES(41) + " ( nomoj poste ( a" + ES(41) + " ,");
						return ObjectProperties();
					}
					else {
						if (_tokens[_curToken].isDelimiter("(")) {
							const List<ObjectProperties> subObjectPropertiesList(_parseObjectPropertiesList());
							if (!_isValid(subObjectPropertiesList)) {
								_addMessage(Message::Type::Error, "Malvalida subarbo");
								return ObjectProperties();
							}
							else objectPropertiesList.add(subObjectPropertiesList);
							if (_tokens[_curToken].isDelimiter(","))
								_curToken++;
							else if (_tokens[_curToken].isDelimiter(")")) {
								_curToken++;
								break;
							}
							continue;
						}
						else {
							const ObjectProperties declaration(_parseDeclaration());
							if (declaration.getClass() == nullptr) return declaration;
							else {
								objectPropertiesList.add(declaration);
								if (_tokens[_curToken].isDelimiter(","))
									_curToken++;
								else if (_tokens[_curToken].isDelimiter(")")) {
									_curToken++;
									break;
								}
							}
						}
					}
				}
			}
			return objectPropertiesList;
		}
	}
}

void Parser::_parseClass() {
	if (_curToken >= _tokens.size())
		BUG("Jam en la fino anta" + ES(41) + " la analizo de klaso");
	else {
		bool external(false);
		if (_tokens[_curToken].isIdentifier("EKSTERA")) {
			_curToken++;
			external = true;
		}
		if (_curToken >= _tokens.size())
			BUG("Neatendita fino poste EKSTERA");
		else if (!_tokens[_curToken].isIdentifier("KLASO"))
			BUG("Atendita KLASO");
		else {
			_curToken++;
			if (_curToken >= _tokens.size())
				_addMessage(Message::Type::Error, "Neatendita fino poste KLASO");
			else if (!_tokens[_curToken].isIdentifier())
				_addMessage(Message::Type::Error, "Atendita identigilo poste KLASO");
			else {
				const Ecii::String name(_withNamespacePrefix(_tokens[_curToken].label()));
				_curToken++;
				if (_curToken >= _tokens.size())
					_addMessage(Message::Type::Error, "Neatendita fino poste la klasnomo");
				else if (isProgvoKeyword(name))
					_addMessage(Message::Type::Error, "Ne povas uzi " + ES(38) + "losilvorton " + name + " kiel klasnomo");
				else if (!external) {
					const std::shared_ptr<Class> c(_environment->getClass(name));
					if (c == nullptr) {
						if (_tokens[_curToken].isDelimiter(";")) // Declare incomplete class
							_environment->add(std::make_shared<Class>(false, name));
						else
							_parseClassDefinition(std::make_shared<Class>(false, name));
					}
					else {
						if (c->isComplete())
							_addMessage(Message::Type::Error, "Klaso " + name + " jam difinita");
						else
							_parseClassDefinition(c);
					}
				}
				else {
					if (!_tokens[_curToken].isDelimiter(";"))
						_addMessage(Message::Type::Error, "Atendita ; poste la klasnomo");
					else {
						std::shared_ptr<Class> builtInClass(getBuiltInClass(name));
						if (builtInClass == nullptr)
							_addMessage(Message::Type::Error, "La interpretilo a" + ES(41) + " tradukilo ne proponas klason " + name);
						else
							_environment->add(builtInClass);
					}
				}
			}
		}
	}
}

void Parser::_parseClassDefinition(const std::shared_ptr<Class> &classToDefine) {
	if (_curToken >= _tokens.size())
		BUG("Jam en la fino anta" + ES(41) + " la analizo de klasdifino");
	else if (!_tokens[_curToken].isDelimiter(":"))
		_addMessage(Message::Type::Error, "Atendita : por klasdifino");
	else if (classToDefine == nullptr)
		BUG("La klaso estas malvalida");
	else if (isBuiltIn(classToDefine))
		BUG("Ne povas difini eksteran klason");
	else if (classToDefine->isComplete())
		BUG("La klaso estas jam kompleta");
	else {
		_curToken++;
		_environment->add(classToDefine);
		while (_curToken < _tokens.size()) {
			if (_tokens[_curToken].isIdentifier()) {
				if (isProgvoKeyword(_tokens[_curToken].label())) {
					if (_tokens[_curToken].isIdentifier("KONSTRUILO")) {
						_curToken++;
						if (_curToken >= _tokens.size()) {
							_addMessage(Message::Type::Error, "Neatendita fino poste KONSTRUILO");
							return;
						}
						else {
							const auto input(_parseObjectPropertiesList());
							if (!_isValid(input)) {
								_addMessage(Message::Type::Error, "Malvalida listo de enigo");
								return;
							}
							else {
								if (_curToken >= _tokens.size()) {
									_addMessage(Message::Type::Error, "Neatendita fino poste la listo de enigo");
									return;
								}
								else {
									const std::shared_ptr<Environment> localEnvironment(std::make_shared<Environment>(_environment));
									localEnvironment->add(input);
									localEnvironment->add(ObjectProperties(classToDefine));
									if (classToDefine != nullptr) {
										for (const auto &a : classToDefine->getAttributes())
											localEnvironment->add(ObjectProperties(a.getClass(), a.getName(), false, false));
									}
									const std::vector<Instruction> instructions(_parseInstructionSequence(localEnvironment));
									if (!_isValid(instructions))
										break;
									else {
										const Type inputType(Type::from(input));
										if (classToDefine->getMethod(classToDefine->getName(), inputType) != nullptr) {
											_addMessage(Message::Type::Error, "Konstruilo " + classToDefine->getName() + inputType.es() + " jam ekzistas");
											return;
										}
										else {
											classToDefine->addMethod(std::make_shared<UserFunction>(classToDefine, input, instructions));
											continue;
										}
									}
								}
							}
						}
					}
					else if (_tokens[_curToken].isIdentifier("KLASO")) {
						_curToken++;
						if (_curToken >= _tokens.size())
							_addMessage(Message::Type::Error, "Neatendita fino poste KLASO");
						else if (!_tokens[_curToken].isIdentifier())
							_addMessage(Message::Type::Error, "Atendita identigilo poste KLASO");
						else {
							const Ecii::String name(_withNamespacePrefix(_tokens[_curToken].label()));
							_curToken++;
							if (isProgvoKeyword(name))
								_addMessage(Message::Type::Error, "Ne povas uzi " + ES(37) + "hlosilvorto " + name + " al klasnomo");
							else if (_environment->getClass(name) != nullptr)
								_addMessage(Message::Type::Error, "Klaso " + name + " jam difinita");
							else {
								_curNamespace.push_back(classToDefine->getName());
								_parseClassDefinition(std::make_shared<Class>(false, _withNamespacePrefix(name)));
								_curNamespace.pop_back();
							}
						}
					}
					else if (_tokens[_curToken].isIdentifier("FINO")) {
						_curToken++;
						if (_curToken >= _tokens.size()) {
							_addMessage(Message::Type::Error, "Neatendita fino poste FINO");
							return;
						}
						else if (_tokens[_curToken].isIdentifier("KLASO")) {
							_curToken++;
							if (_curToken >= _tokens.size())
								_addMessage(Message::Type::Error, "Neatendita fino poste FINO");
							else if (!_tokens[_curToken].isDelimiter(";"))
								_addMessage(Message::Type::Error, "Atendita ; poste FINO KLASO");
							else
								classToDefine->setComplete();
							return;
						}
						else _addMessage(Message::Type::Error, "Atendita KLASO poste FINO");
					}
					else {
						_addMessage(Message::Type::Error, "Neatendita " + ES(37) + "hlosilvorto " + _tokens[_curToken].label().str());
						return;
					}
				}
				else {
					if (_curToken + 1 >= _tokens.size()) {
						_addMessage(Message::Type::Error, "Neatendita fino poste " + _tokens[_curToken].label());
						return;
					}
					else if (_tokens[_curToken + 1].isDelimiter(":")) {
						_parseFunction(classToDefine);
						continue;
					}
					else {
						const ObjectProperties declaration(_parseDeclaration());
						if (declaration.getName() != "") {
							if (classToDefine->isMember(declaration.getName())) {
								_addMessage(Message::Type::Error, declaration.getName() + " jam estas membro de la klaso");
								return;
							}
							else {
								if (_curToken >= _tokens.size()) {
									_addMessage(Message::Type::Error, "Neatendita fino poste " + _tokens[_curToken - 1].label());
									return;
								}
								else if (!_tokens[_curToken].isDelimiter(";")) {
									_addMessage(Message::Type::Error, "Atendita ; poste deklaro de atribuo");
									return;
								}
								else {
									classToDefine->addAttribute(declaration);
								}
							}
						}
					}
				}
			}
			else if (_tokens[_curToken].isDelimiter(";"));
			else _addMessage(Message::Type::Error, "Neatendita jhetono " + _tokens[_curToken].label());
			_goAfterNextDelimiter(std::byte{93}); // ";"
		}
	}
}

void Parser::_parseFunction(const std::shared_ptr<Class> &c) {
	if (_curToken >= _tokens.size())
		BUG("Jam en la fino anta" + ES(41) + " la analizo de deklaro a" + ES(41) + " difino de funkcio");
	else {
		bool external(false);
		if (_tokens[_curToken].isIdentifier("EKSTERA")) {
			_curToken++;
			external = true;
		}
		if (_curToken >= _tokens.size())
			BUG("Neatendita fino poste EKSTERA");
		else if (!_tokens[_curToken].isIdentifier())
			BUG("Atendita nomo por deklaro a" + ES(41) + " difino de funkcio");
		else {
			_curToken++;
			if (_curToken >= _tokens.size())
				BUG("Neatendita fino poste " << _tokens[_curToken - 1].label());
			else if (!_tokens[_curToken].isDelimiter(":"))
				BUG("Atendita : poste funkcinomo " + _tokens[_curToken - 1].label());
			else {
				_curToken++;
				Ecii::String name;
				if (c == nullptr)  name = _withNamespacePrefix(_tokens[_curToken - 2].label());
				else name = _tokens[_curToken - 2].label();
				List<ObjectProperties> input(_parseObjectPropertiesList()), output;
				if (!_isValid(input))
					_addMessage(Message::Type::Error, "Malvalida deklarlisto por enigo (malvalida(j) deklaro(j) a" + ES(41) + " duobla(j) nomo(j))");
				else if (_curToken >= _tokens.size())
					_addMessage(Message::Type::Error, "Neatendita fino poste la deklarlisto por enigo");
				else if (!_tokens[_curToken].isDelimiter(std::byte{87})) // ->
					_addMessage(Message::Type::Error, "Atendita -> poste la deklarlisto por enigo");
				else {
					_curToken++;
					List<ObjectProperties> output(_parseObjectPropertiesList());
					if (!_isValid(output))
						_addMessage(Message::Type::Error, "Malvalida deklarlisto por eligo (malvalida(j) deklaro(j) a" + ES(41) + " duobla(j) nomo(j))");
					else if (_curToken >= _tokens.size())
						_addMessage(Message::Type::Error, "Neatendita fino poste la deklarlisto por enigo");
					else {
						std::shared_ptr<Function> f(nullptr);
						if (!external) {
							if (c == nullptr) f = _environment->getFunction(name, Type::from(input));
							else f = c->getMethod(name, Type::from(input));
							if (f == nullptr) {
								f = std::make_shared<UserFunction>(name, input, output);
								if (c == nullptr) _environment->add(f);
								else c->addMethod(f);
							}
							if (f->isComplete())
								_addMessage(Message::Type::Error, "Funkcio a" + ES(41) + " metodo " + name + " jam difinita");
							else if (_tokens[_curToken].isDelimiter(";")) {
								if (c == nullptr) {
									if (f == nullptr)
										_environment->add(std::make_shared<UserFunction>(name, input, output));
									else if (f->getOutputType() != Type::from(output))
										_addMessage(Message::Type::Error, "La eligo de la funkcio malkongruas la jam deklaritan funkcion");
								}
								else {
									if (f == nullptr)
										c->addMethod(std::make_shared<UserFunction>(name, input, output));
									else if (f->getOutputType() != Type::from(output))
										_addMessage(Message::Type::Error, "La eligo de la metodo malkongruas la jam deklaritan metodon");
								}
							}
							else {
								std::shared_ptr<Environment> localEnvironment(std::make_shared<Environment>(_environment));
								localEnvironment->add(input);
								localEnvironment->add(output);
								if (c != nullptr) {
									for (const auto &a : c->getAttributes())
										localEnvironment->add(ObjectProperties(a.getClass(), a.getName(), false, false));
								}
								std::vector<Instruction> instructions(_parseInstructionSequence(localEnvironment));
								if (_isValid(instructions)) {
									if (c == nullptr) {
										if (f == nullptr)
											_environment->add(std::make_shared<UserFunction>(name, input, output, instructions));
										else if (f->getOutputType() != Type::from(output))
											_addMessage(Message::Type::Error, "La eligo de la funkcio malkongruas la jam deklaritan funkcion");
										else
											*std::static_pointer_cast<UserFunction>(f) = UserFunction(name, input, output, instructions);
									}
									else {
										if (f == nullptr)
											c->addMethod(std::make_shared<UserFunction>(name, input, output, instructions));
										else if (f->getOutputType() != Type::from(output))
											_addMessage(Message::Type::Error, "La eligo de la metodo malkongruas la jam deklaritan metodon");
										else
											*std::static_pointer_cast<UserFunction>(f) = UserFunction(name, input, output, instructions);
									}
								}
							}
						}
						else {
							if (!_tokens[_curToken].isDelimiter(";"))
								_addMessage(Message::Type::Error, "Atendita ; poste eligo");
							else {
								if (c != nullptr)
									BUG("Metodoj ne povas esti ekstera");
								else {
									f = getBuiltInFunction(name, Type::from(input));
									if (f == nullptr)
										_addMessage(Message::Type::Error, "La interpretilo a" + ES(41) + " tradukilo ne proponas funkcion " + name + Type::from(input).es());
									else if (f->getOutputType() != Type::from(output))
										_addMessage(Message::Type::Error, "La eligo de la deklarita ekstera funkcio malkongruas la proponitan funkcion");
									else if (_environment->getFunction(name, Type::from(input)) != nullptr)
										_addMessage(Message::Type::Error, "Funkcio " + name + Type::from(input).es() + " jam deklarita");
									else
										_environment->add(f);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Parser::_parseObjectDeclaration(const std::shared_ptr<Environment> &localEnvironment, std::vector<Instruction> &instructions) {
	const ObjectProperties op(_parseDeclaration());
	if (op.getClass() != nullptr) {
		Instruction instruction;
		instruction.setDeclaration(op);
		if (_tokens[_curToken].isDelimiter(";")) {
			localEnvironment->add(op);
			instructions.push_back(instruction);
		}
		else if (_tokens[_curToken].isDelimiter(std::byte{86})) { // ←
			_curToken++;
			const Expression input(_parseExpression(localEnvironment));
			const Type outputType(op.getClass()), inputType(localEnvironment->typeOf(input));
			if (outputType != inputType)
				_addMessage(Message::Type::Error, "La esprimo (de klaso " + inputType.es() + ") estas malkongrua kun la objekto (de klaso " + outputType.es() + ") por la asigno");
			else {
				localEnvironment->add(op);
				Vect2<uintmax_t> pos(localEnvironment->getObjectPos(op.getName()));
				instructions.push_back(instruction);
				instruction.setAssignment({ObjectLocation(pos, std::vector<uintmax_t>{})}, input);
				instructions.push_back(instruction);
			}
		}
		else _addMessage(Message::Type::Error, "Atendita ; a" + ES(41) + " " + ES(86) + " poste/en la objektdeklaro");
	}
}

void Parser::reset() {
	if (_parsingData != nullptr) _parsingData->clear();
	_tokens = std::vector<Token>();
	_curNamespace = std::vector<Ecii::String>();
	_environment = std::make_shared<Environment>();
	_curToken = 0;
}

std::shared_ptr<Function> Parser::parse(const std::filesystem::path &path) {
	reset();
	Tokenizer tokenizer(_parsingData);
	std::filesystem::path fullPath(path), directory(fullPath.parent_path()), fileName(fullPath.filename());
	tokenizer.setCurDir(directory);
	_tokens = tokenizer.tokenize(fileName);
	_parseInstructionSequence(_environment, true);
	return _environment->getFunction("enirejo", List<std::shared_ptr<Class>>{});
}