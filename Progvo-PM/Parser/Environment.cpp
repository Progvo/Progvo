// (c) 2019-2020 Pttn (https://Progvo.dev)

#include "Environment.hpp"
#include "../Interpreter/BuiltIn/BuiltIn.hpp"

Class::Class(const bool &provided, const Ecii::String &name, const std::vector<std::shared_ptr<Function>> &methods, const std::vector<ObjectProperties> &attributes) :
	_builtIn(provided),
	_complete(false),
	_name(name),
	_attributes(attributes),
	_methods(methods),
	_constructor([&]() {
		std::shared_ptr<Data> data(std::make_shared<UserData>());
		for (auto &a : _attributes) {
			if (a.isReference())
				std::static_pointer_cast<UserData>(data)->attributes.push_back(std::make_shared<ReferenceData>());
			else
				std::static_pointer_cast<UserData>(data)->attributes.push_back(a.getClass()->construct());
		}
		return data;
	}) {}

Class::Class(const Ecii::String &name, const std::vector<std::shared_ptr<Function>> &methods, const std::vector<ObjectProperties> &attributes, const std::function<std::shared_ptr<Data>()> &constructor) :
	_builtIn(true),
	_complete(true),
	_name(name),
	_attributes(attributes),
	_methods(methods),
	_constructor(constructor) {}

bool Class::isAttribute(const Ecii::String &name) const {
	for (auto& a : _attributes) {if (a.getName() == name) return true;}
	return false;
}

ObjectProperties Class::getAttribute(const Ecii::String &name) const {
	for (auto& a : _attributes) {if (a.getName() == name) return a;}
	return ObjectProperties(nullptr, "ERARO");
}
uintmax_t Class::getAttributePos(const Ecii::String &name) const {
	for (uintmax_t i(0) ; i < _attributes.size() ; i++) {if (_attributes[i].getName() == name) return i;}
	return -1ULL;
}

bool Class::isMethod(const Ecii::String &name) const {
	for (auto& m : _methods) {if (m->getName() == name) return true;}
	return false;
}
std::shared_ptr<Function> Class::getMethod(const Ecii::String &name, const Type &input) const {
	for (const auto &m : _methods) {if (m->isMatching(name, input)) return m;}
	return nullptr;
}
uintmax_t Class::getMethodPos(const Ecii::String &name, const Type &input) const {
	for (uintmax_t i(0) ; i < _methods.size() ; i++) {if (_methods[i]->isMatching(name, input)) return i;}
	return -1ULL;
}
void Class::addMethod(const std::shared_ptr<Function> &method) {
	if (method == nullptr ? false : getMethod(method->getName(), method->getInputType()) == nullptr)
		_methods.push_back(method);
}

Ecii::String Type::_makeReadable(const List<std::shared_ptr<Class>>& classList, const bool &showParenthesisForSingleton) {
	Ecii::String es;
	if (classList.isLeaf()) {
		if (showParenthesisForSingleton) es  += "(";
		const std::shared_ptr<Class> &c(classList.data());
		if (c == nullptr)
			es += "ERARO";
		else
			es += c->getName();
		if (showParenthesisForSingleton) es  += ")";
	}
	else {
		es  += "(";
		const std::vector<List<std::shared_ptr<Class>>> &subClassLists(classList.children());
		for (std::vector<List<std::shared_ptr<Class>>>::size_type i(0) ; i < subClassLists.size() ; i++) {
			es += _makeReadable(subClassLists[i], false);
			if (i + 1 < subClassLists.size()) es += ", ";
		}
		es += ")";
	}
	return es;
}

List<std::shared_ptr<Class>> Type::_extractType(const List<ObjectProperties> &declarationList) {
	List<std::shared_ptr<Class>> classList;
	if (declarationList.isLeaf())
		classList.add(declarationList.data().getClass());
	else {
		for (const auto &sdl : declarationList.children())
			classList.add(_extractType(sdl));
	}
	return classList;
}

std::shared_ptr<Data> constructFrom(const std::vector<ObjectProperties>& op) {
	if (op.size() == 0) return nullptr;
	else if (op.size() == 1) {
		if (op[0].isReference()) return std::make_shared<ReferenceData>();
		else return op[0].getClass()->construct();
	}
	else {
		std::vector<std::shared_ptr<Data>> objects;
		for (const auto &d : op) {
			if (d.isReference()) objects.push_back(std::make_shared<ReferenceData>());
			else objects.push_back(d.getClass()->construct());
		}
		return std::make_shared<VectorData>(objects);
	}
}

Function::Function(const std::shared_ptr<Class>& c, const List<ObjectProperties>& input) : _complete(false), _constructorClass(c) {
	if (_constructorClass == nullptr)
		_name = "ERARO";
	else {
		_name = c->getName();
		_inputType = Type::from(input);
		_outputType = c;
	}
}

UserFunction::UserFunction(const Ecii::String &name, const List<ObjectProperties> &input, const List<ObjectProperties> &output, const std::vector<Instruction> &instructionSequence) :
	Function(name, input, output), 
	_instructionSequence(instructionSequence),
	_inputOP(input.linearized()),
	_outputOP(output.linearized()) {
	_complete = true;
}

UserFunction::UserFunction(const std::shared_ptr<Class> &c, const List<ObjectProperties> &input, const std::vector<Instruction> &instructionSequence) :
	Function(c, input), 
	_instructionSequence(instructionSequence),
	_inputOP(input.linearized()),
	_outputOP{ObjectProperties(c, "", false, false)} {
	_complete = true;
}

std::shared_ptr<Data> UserFunction::createInput() const {return constructFrom(_inputOP);}
std::shared_ptr<Data> UserFunction::createOutput() const {return constructFrom(_outputOP);}

std::shared_ptr<ObjectProperties> Environment::_getObject(const Ecii::String &name, Vect2<uintmax_t> &pos) const {
	for (uintmax_t i(0) ; i < _objects.size() ; i++) {
		if (_objects[i]->getName() == name) {
			pos.y = i;
			return _objects[i];
		}
	}
	if (_parent != nullptr) {
		pos.x++;
		return _parent->_getObject(name, pos);
	}
	return nullptr;
}

bool Environment::isUsedFunctionName(const Ecii::String &functionName) const {
	if (functionName == "~") return true;
	for (const auto &f : _functions) {if (functionName == f->getName()) return true;}
	if (_parent != nullptr) return _parent->isUsedFunctionName(functionName);
	return false;
}

std::shared_ptr<Class> Environment::getClass(const Ecii::String &name) const {
	for (const auto &c : _classes) {if (c->getName() == name) return c;}
	if (_parent != nullptr) return _parent->getClass(name);
	else return nullptr;
}

std::shared_ptr<Function> Environment::getFunction(const Ecii::String &name, const Type &input) const {
	if (name == "~") {
		auto inputChildren(input.getClassList().children());
		if (inputChildren.size() == 2) {
			if (inputChildren[1].data() == builtInPointer) {
				return builtInEqualsReferencePointer;
			}
		}
	}
	
	for (const auto &f : _functions) {if (f->isMatching(name, input)) return f;}
	if (_parent != nullptr) return _parent->getFunction(name, input);
	else return nullptr;
}

std::shared_ptr<ObjectProperties> Environment::getObject(const Ecii::String &name) const {
	Vect2<uintmax_t> dummy(0, 0);
	return _getObject(name, dummy);
}
std::shared_ptr<ObjectProperties> Environment::getObject(const Vect2<uintmax_t> &pos) const {
	if (pos.x == 0) {
		if (pos.y >= _objects.size())
			return nullptr;
		else
			return _objects[pos.y];
	}
	else {
		if (_parent == nullptr) return nullptr;
		else return _parent->getObject(Vect2<uintmax_t>(pos.x - 1ULL, pos.y));
	}
}
Vect2<uintmax_t> Environment::getObjectPos(const Ecii::String &name) const {
	Vect2<uintmax_t> pos(0, 0);
	_getObject(name, pos);
	return pos;
}

void Environment::add(const List<ObjectProperties> &objects) {
	if (objects.isLeaf()) add(objects.data());
	else {
		auto subObjects(objects.children());
		for (const auto & so : subObjects) add(so);
	}
}

std::shared_ptr<Class> Environment::classOf(const Ecii::String &objectName) const {
	for (const auto &o : _objects) {if (objectName == o->getName()) return o->getClass();}
	if (_parent != nullptr) return _parent->classOf(objectName);
	return nullptr;
}

Type Environment::typeOf(const Expression &expression) const {
	Type classes;
	if (expression.isEmpty())
		return classes;
	else if (expression.isConstant())
		classes.add(expression.getConstantClass());
	else if (expression.isVariable()) {
		const std::shared_ptr<ObjectProperties> object0(getObject(expression.getObjectPos()));
		if (object0 == nullptr)
			return Type(nullptr);
		else classes.add(object0->getClass());
	}
	else if (expression.isCall())
		classes.add(expression.getFunction()->getOutputType());
	else if (expression.isList()) {
		const std::vector<Expression> &subExpressions(expression.children());
		for (const auto &se : subExpressions)
			classes.add(typeOf(se));
	}
	else if (expression.isAccessChain()) {
		const std::vector<Expression> &subAccessData(expression.children());
		std::shared_ptr<Class> currentClass(nullptr);
		for (std::vector<Expression>::size_type i(0) ; i < subAccessData.size() ; i++) {
			if (subAccessData[i].isConstant())
				currentClass = subAccessData[i].getConstantClass();
			else if (subAccessData[i].isVariable()) {
				const std::shared_ptr<ObjectProperties> object0(getObject(subAccessData[i].getObjectPos()));
				if (object0 == nullptr)
					return Type(nullptr);
				currentClass = object0->getClass();
			}
			else if (subAccessData[i].isAttributeAccess()) {
				currentClass = currentClass->getAttribute(subAccessData[i].getAttributePos()).getClass();
				if (i + 1 == subAccessData.size()) return {currentClass};
			}
			else if (subAccessData[i].isCall()) {
				const Type outputClass(subAccessData[i].getFunction()->getOutputType());
				if (outputClass.isSimple())
					currentClass = outputClass.getSimpleClass();
				else {
					classes.add(outputClass);
					return classes;
				}
			}
			else
				return Type(nullptr);
		}
		classes.add(currentClass);
	}
	else
		return Type(nullptr);
	return classes;
}

bool Environment::isAssignable(const Expression &expression) const {
	if (expression.isVariable()) {
		const std::shared_ptr<ObjectProperties> object(getObject(expression.getObjectPos()));
		if (object == nullptr) {
			BUG("Malvalida objekto");
			return false;
		}
		else if (object->isConstant()) return false;
		else return true;
	}
	else if (expression.isAccessChain()) {
		const std::vector<Expression> &accessData(expression.children());
		std::shared_ptr<ObjectProperties> object(getObject(accessData[0].getObjectPos()));
		if (object == nullptr) return false;
		else if (object->isConstant()) return false;
		else return expression.isOnlyAttributeAccesses();
	}
	else if (expression.isList()) {
		const std::vector<Expression>& children(expression.children());
		for (const auto &c : children) {
			if (!isAssignable(c)) return false;
		}
		return true;
	}
	else return false;
}

std::vector<std::shared_ptr<Class>> Environment::getIncompleteClasses() const {
	std::vector<std::shared_ptr<Class>> incompleteClasses;
	for (const auto &c : _classes) {
		if (!c->isComplete())
			incompleteClasses.push_back(c);
	}
	return incompleteClasses;
}
std::vector<std::shared_ptr<Function>> Environment::getIncompleteFunctions() const {
	std::vector<std::shared_ptr<Function>> incompleteFunctions;
	for (const auto &f : _functions) {
		if (!f->isComplete())
			incompleteFunctions.push_back(f);
	}
	return incompleteFunctions;
}