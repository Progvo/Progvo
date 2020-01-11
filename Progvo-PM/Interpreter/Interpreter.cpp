// (c) 2019-2020 Pttn (https://Progvo.dev)

#include "Interpreter.hpp"
#include "../GUI/IDE.hpp"

void Interpreter::_start() {
	if (_entryPoint == nullptr)
		*builtInStandardOutput += "Enirejo ne difinita!\n";
	else {
		std::chrono::time_point<std::chrono::system_clock> t0(std::chrono::system_clock::now());
		*builtInStandardOutput += "Programaro startis!\n";
		_running = true;
		_call(_entryPoint, {}, nullptr);
		_running = false;
		if (!_stop)
			*builtInStandardOutput += "\nProgramaro finis!";
		else
			*builtInStandardOutput += "\nProgramaro haltita!";
		*builtInStandardOutput += " Da" + ES(41) + "ro: " + std::to_string(timeSince(t0)) + " s\n";
		_stop = false;
	}
}

void Interpreter::_addData(const std::shared_ptr<Data>& data) {
	if (data != nullptr) {
		if (!data->isVector())
			_objects.back().push_back(data);
		else {
			for (const auto &o : data->get())
				_objects.back().push_back(o);
		}
	}
}

Interpreter::ExitType Interpreter::_execute(const Instruction& instruction) {
	if (_stop) return ExitType::Stop;
	else if (instruction.getType() == Instruction::Type::Declaration)
		_objects.back().push_back(instruction.construct());
	else if (instruction.getType() == Instruction::Type::Assignment)
		_assign(instruction.getAssignmentObjectsPos(), _eval(instruction.getAssignmentExpression()));
	else if (instruction.getType() == Instruction::Type::Condition) {
		const std::vector<std::pair<Expression, std::vector<Instruction>>> &conditionStructure(instruction.getConditionStructure());
		for (const auto &cs : conditionStructure) {
			if (!cs.first.isEmpty()) {
				const auto test(_eval(cs.first));
				if (_stop) return ExitType::Stop;
				if (*std::static_pointer_cast<bool>(test->data) != true)
					continue;
			}
			for (const auto& instruction : cs.second) {
				const ExitType instructionExitType(_execute(instruction));
				if (instructionExitType != ExitType::NONE || _stop)
					return instructionExitType;
			}
			break;
		}
	}
	else if (instruction.getType() == Instruction::Type::Loop) {
		const std::pair<Expression, std::vector<Instruction>> &ls(instruction.getLoopStructure());
		while (!_stop) {
			if (!ls.first.isEmpty()) {
				const auto test(_eval(ls.first));
				if (_stop) return ExitType::Stop;
				if (*std::static_pointer_cast<bool>(test->data) != true)
					break;
			}
			ExitType instructionExitType;
			for (const auto& instruction : ls.second) {
				instructionExitType = _execute(instruction);
				if (instructionExitType == ExitType::Return || instructionExitType == ExitType::Stop || _stop)
					return instructionExitType;
			}
			if (instructionExitType == ExitType::Break) break;
		}
	}
	else if (instruction.getType() == Instruction::Type::Evaluation) {
		_eval(instruction.getEvaluationExpression());
		if (_stop) return ExitType::Stop;
	}
	else if (instruction.getType() == Instruction::Type::Stop) {
		_stop = true;
		return ExitType::Stop;
	}
	else if (instruction.getType() == Instruction::Type::Return)
		return ExitType::Return;
	else if (instruction.getType() == Instruction::Type::Break)
		return ExitType::Break;
	else {
		BUG("Plenumo de malvalida instrukcio");
		return ExitType::Stop;
	}
	return ExitType::NONE;
}

std::shared_ptr<Data> Interpreter::_call(const std::shared_ptr<Function> &function, const std::shared_ptr<Data> &input0, const std::shared_ptr<Data> &thisObject) {
	if (_objects.size() >= CALLSTACKLIMIT) {
		*builtInStandardOutput += "\nLa alvokstako estas tro granda!\n";
		_stop = true;
	}
	if (_stop) return {};
	std::shared_ptr<Data> output;
	if (function == nullptr) BUG("Alvoko de malvalida funkcio");
	else {
		_objects.push_back(std::vector<std::shared_ptr<Data>>());
		if (function->isBuiltIn()) output = (*std::static_pointer_cast<BuiltInFunction>(function))(thisObject, input0);
		else {
			std::shared_ptr<Data> input(std::static_pointer_cast<UserFunction>(function)->createInput());
			_addData(input);
			_assign(input0, input);
			if (function->isConstructor()) {
				output = std::static_pointer_cast<UserFunction>(function)->createOutput();
				_objects.back().push_back(output);
				for (auto &a : std::static_pointer_cast<UserData>(output)->attributes)
					_objects.back().push_back(a);
			}
			else {
				output = std::static_pointer_cast<UserFunction>(function)->createOutput();
				if (output != nullptr) {
					if (!output->isVector())
						_objects.back().push_back(output);
					else {
						for (const auto &o : output->get())
							_objects.back().push_back(o);
					}
				}
			}
			if (thisObject != nullptr) {
				for (auto &a : std::static_pointer_cast<UserData>(thisObject)->attributes) {
					if (a->isReference() && a->data != nullptr)
						_objects.back().push_back(std::static_pointer_cast<Data>(a->data));
					else
						_objects.back().push_back(a);
				}
			}
			for (const auto& instruction : std::static_pointer_cast<UserFunction>(function)->instructions()) {
				const ExitType instructionExitType(_execute(instruction));
				if (instructionExitType != ExitType::NONE || _stop)
					break;
			}
		}
		_objects.pop_back();
	}
	return output;
}

std::shared_ptr<Data> Interpreter::_getObject(const Vect2<uintmax_t>& pos) {
	if (pos.x >= _objects.size()) {
		BUG("Malvalida profundeco por objekto (" << pos.x << ", " << pos.y << ")");
		return nullptr;
	}
	else if (pos.y >= _objects[_objects.size() - pos.x - 1].size()) {
		BUG("Malvalida pozicio por objekto (" << pos.x << ", " << pos.y << ")");
		return nullptr;
	}
	else {
		std::shared_ptr<Data> object(_objects[_objects.size() - pos.x - 1][pos.y]);
		if (object->isReference() && object->data != nullptr)
			return std::static_pointer_cast<Data>(object->data);
		else return object;
	}
}

std::shared_ptr<Data> Interpreter::_eval(const Expression &expression) {
	if (_stop) return {};
	std::shared_ptr<Data> objects;
	if (expression.isEmpty());
	else if (expression.isConstant())
		objects = expression.getConstant();
	else if (expression.isVariable()) {
		std::shared_ptr<Data> object(_getObject(expression.getObjectPos()));
		if (object->isReference() && object->data != nullptr)
			object = std::static_pointer_cast<Data>(object->data);
		objects = object;
	}
	else if (expression.isCall()) {
		std::shared_ptr<Expression> input(expression.getCallInput());
		if (input == nullptr) {
			BUG("Malvalida enigo de alvoko");
			return {nullptr};
		}
		std::shared_ptr<Data> output(_call(expression.getFunction(), _eval(*input), nullptr));
		if (output != nullptr) {
			if (!output->isVector()) objects = output;
			else return output;
		}
	}
	else if (expression.isList()) {
		objects = std::make_shared<VectorData>();
		const std::vector<Expression> &subExpressions(expression.children());
		for (const auto &se : subExpressions) {
			std::shared_ptr<Data> subObjects(_eval(se));
			if (subObjects != nullptr) {
				if (!subObjects->isVector())
					std::static_pointer_cast<VectorData>(objects)->objects.push_back(subObjects);
				else {
					for (const auto &o : subObjects->get())
						std::static_pointer_cast<VectorData>(objects)->objects.push_back(o);
				}
			}
		}
	}
	else if (expression.isAccessChain()) {
		const std::vector<Expression> &subAccessData(expression.children());
		std::shared_ptr<Data> currentObject(nullptr);
		if (subAccessData.size() < 2) {
			BUG("Cheno de acesoj devas havi pli ol 2 erojn");
			_stop = true;
			return {};
		}
		if (subAccessData[0].isConstant())
			currentObject = subAccessData[0].getConstant();
		else if (subAccessData[0].isVariable()) {
			currentObject = _getObject(subAccessData[0].getObjectPos());
		}
		else if (subAccessData[0].isCall()) {
			std::shared_ptr<Expression> input(subAccessData[0].getCallInput());
			if (input == nullptr) {
				BUG("Malvalida enigo por metodalvoko");
				return {nullptr};
			}
			else {
				objects = _call(subAccessData[0].getFunction(), _eval(*input), currentObject);
				if (objects == nullptr) {
					BUG("Eligo ne povas havi 0 objekton");
					_stop = true;
					return {};
				}
				else if (!objects->isVector()) currentObject = objects;
				else {
					BUG("Eligo devas havi 1 ero chi tiu");
					_stop = true;
					return {};
				}
			}
		}
		else {
			BUG("Malvalida ero 0 de cheno de acesoj");
			_stop = true;
			return {};
		}
		if (currentObject == nullptr) {
			BUG("Malvalida objekto");
			_stop = true;
			return {};
		}
		if (currentObject->isReference()) {
			if (currentObject->data == nullptr) {
				*builtInStandardOutput += "\nElreferenco de malpravalorizitan objekton por acesado de atribuo!\n";
				_stop = true;
				return {};
			}
			else currentObject = std::static_pointer_cast<Data>(currentObject->data);
		}
		
		for (std::vector<Expression>::size_type i(1) ; i < subAccessData.size() ; i++) {
			if (subAccessData[i].isAttributeAccess()) {
				if (currentObject->isReference()) {
					if (currentObject->data == nullptr) {
						*builtInStandardOutput += "\nElreferenco de malpravalorizitan objekton por acesado de atribuo!\n";
						_stop = true;
						return {};
					}
					else currentObject = std::static_pointer_cast<Data>(currentObject->data);
				}
				currentObject = std::static_pointer_cast<UserData>(currentObject)->attributes[subAccessData[i].getAttributePos()];
				if (i + 1 == subAccessData.size()) {
					if (currentObject->isReference() && currentObject->data != nullptr)
						return {std::static_pointer_cast<Data>(currentObject->data)};
					else
						return {currentObject};
				}
			}
			else if (subAccessData[i].isCall()) {
				const std::shared_ptr<Expression> input(subAccessData[i].getCallInput());
				if (input == nullptr) {
					BUG("Malvalida enigo por metodalvoko");
					return {nullptr};
				}
				else {
					if (currentObject->isReference()) {
						if (currentObject->data == nullptr) {
							*builtInStandardOutput += "\nElreferenco de malpravalorizitan objekton por acesado de metodo!\n";
							_stop = true;
							return {};
						}
						else currentObject = std::static_pointer_cast<Data>(currentObject->data);
					}
					objects = _call(subAccessData[i].getFunction(), _eval(*input), currentObject);
					if (objects == nullptr) return nullptr;
					else if (!objects->isVector()) currentObject = objects;
					else return objects;
				}
			}
			else {
				BUG("Malvalida ero " << i << " de cheno de acesoj");
				_stop = true;
				return {};
			}
		}
	}
	else {
		BUG("Malvalida esprimo");
		_stop = true;
		return {};
	}
	return objects;
}

void Interpreter::_assign(const std::shared_ptr<Data> &values, std::shared_ptr<Data> &objects) {
	if (!_stop && objects != nullptr)
		objects->copy(values);
}

void Interpreter::_assign(const std::vector<ObjectLocation> &objectsPos, const std::shared_ptr<Data> &values) {
	if (!_stop && values != nullptr) {
		if (!values->isVector()) {
			if (objectsPos.size() == 1) {
				std::shared_ptr<Data> object(_getObject(objectsPos[0].pos));
				if (objectsPos[0].attributeLocation.size() == 0)
					object->copy(values);
				else {
					for (const auto &attributePos : objectsPos[0].attributeLocation)
						object = std::static_pointer_cast<UserData>(object)->attributes[attributePos];
					object->copy(values);
				}
			}
			else BUG("Malkongrua esprimo");
		}
		else {
			const std::vector<std::shared_ptr<Data>> &values2(std::static_pointer_cast<VectorData>(values)->objects);
			for (std::size_t i(0) ; i < objectsPos.size() ; i++) {
				if (values2[i] == nullptr) {
					BUG("Eliga objekto estas malvalida");
					break;
				}
				else {
					std::shared_ptr<Data> object(_getObject(objectsPos[i].pos));
					if (objectsPos[i].attributeLocation.size() == 0)
						object->copy(values2[i]);
					else {
						for (const auto &attributePos : objectsPos[i].attributeLocation)
							object = std::static_pointer_cast<UserData>(object)->attributes[attributePos];
						object->copy(values2[i]);
					}
				}
			}
		}
	}
}

Interpreter::Interpreter() : _running(false), _stop(false) {
	if (IDE::get() == nullptr) {
		BUG("La PM ne estas pravalorizita");
		std::exit(-1);
	}
}

void Interpreter::reset() {
	_parser.reset();
	BuiltInCanvasConstructor::reset();
	_stop = false;
}

void Interpreter::run(const Ecii::String& path) {
	if (!_running) {
		reset();
		if (IDE::get()->getConsole() == nullptr)
			BUG("La konzolo ne estas pravalorizita!");
		else {
			IDE::get()->getConsole()->show();
			_entryPoint = _parser.parse(IDE::get()->getCurDir()/path.str());
			std::shared_ptr<ParsingData> pd(_parser.getParsingData());
			uint64_t errors(0);
			for (uint64_t i(0) ; i < pd->messages.size() ; i++) {
				*builtInStandardOutput += pd->messages[i].es();
				if (pd->messages[i].type == Message::Type::Error) errors++;
			}
			if (errors > 0) *builtInStandardOutput += std::to_string(errors) + " eraro(j) trovita\n";
			else {
				_executionThread = std::thread(&Interpreter::_start, this);
				_executionThread.detach();
			}
		}
	}
}