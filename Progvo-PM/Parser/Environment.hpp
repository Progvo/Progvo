// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Environment_hpp
#define HEADER_Environment_hpp

#include <functional>
#include <memory>
#include "../Tools.hpp"

class Instruction;
class Expression;
class Class;
class Type;
class Function;
class Data;

class ObjectProperties {
	std::shared_ptr<Class> c;
	Ecii::String name;
	bool constant, reference;
public:
	ObjectProperties(const std::shared_ptr<Class> &c0 = nullptr, const Ecii::String &name0 = "", const bool &constant0 = true, const bool &reference0 = false) : c(c0), name(name0), constant(constant0), reference(reference0) {}
	std::shared_ptr<Class> getClass() const {return c;}
	Ecii::String getName() const {return name;}
	bool isConstant() const {return constant;}
	bool isReference() const {return reference;}
};

class Class {
	bool _builtIn, _complete;
	Ecii::String _name;
	std::vector<ObjectProperties> _attributes;
	std::vector<std::shared_ptr<Function>> _methods;
	std::function<std::shared_ptr<Data>()> _constructor;
	
public:
	Class(const bool&, const Ecii::String&, const std::vector<std::shared_ptr<Function>>& = std::vector<std::shared_ptr<Function>>(), const std::vector<ObjectProperties>& = std::vector<ObjectProperties>());
	Class(const Ecii::String&, const std::vector<std::shared_ptr<Function>>&, const std::vector<ObjectProperties>&, const std::function<std::shared_ptr<Data>()>&);
	
	bool isBuiltIn() const {return _builtIn;}
	bool isComplete() const {return _complete;}
	void setComplete() {_complete = true;}
	Ecii::String getName() const {return _name;}
	bool isMember(const Ecii::String &name) const {return isAttribute(name) || isMethod(name);}
	bool isAttribute(const Ecii::String&) const;
	ObjectProperties getAttribute(const Ecii::String&) const;
	ObjectProperties getAttribute(const uintmax_t& pos) const {return pos < _attributes.size() ? _attributes[pos] : ObjectProperties(nullptr, "ERARO");}
	uintmax_t getAttributePos(const Ecii::String&) const;
	std::vector<ObjectProperties> getAttributes() const {return _attributes;}
	void addAttribute(const ObjectProperties &op) {if (op.getClass() != nullptr) _attributes.push_back(op);}
	bool isMethod(const Ecii::String&) const;
	std::shared_ptr<Function> getMethod(const Ecii::String&, const Type&) const;
	std::shared_ptr<Function> getMethod(const uintmax_t &pos) const {return pos < _methods.size() ? _methods[pos] : nullptr;}
	uintmax_t getMethodPos(const Ecii::String &name, const Type &input) const;
	void addMethod(const std::shared_ptr<Function> &method);
	
	std::function<std::shared_ptr<Data>()> getConstructor() const {return _constructor;}
	std::shared_ptr<Data> construct() const {return _constructor();}
};

class Type {
	List<std::shared_ptr<Class>> _classList;
	
	static Ecii::String _makeReadable(const List<std::shared_ptr<Class>>&, const bool&);
	static List<std::shared_ptr<Class>> _extractType(const List<ObjectProperties>&);
public:
	Type() : _classList(List<std::shared_ptr<Class>>()) {}
	Type(const std::shared_ptr<Class> &c) : _classList(c) {}
	Type(const List<std::shared_ptr<Class>> &classList) : _classList(classList) {}
	
	bool isSimple() const {return _classList.isLeaf();}
	List<std::shared_ptr<Class>> getClassList() const {return _classList;}
	std::shared_ptr<Class> getSimpleClass() const {return _classList.data();}
	void add(const Type &subType) {_classList.add(subType._classList);}
	
	Ecii::String es(bool showParenthesisForSingleton = true) const {return _makeReadable(_classList, showParenthesisForSingleton);}
	static Type from(const List<ObjectProperties> &objectPropertiesList) {return Type(_extractType(objectPropertiesList));}
	friend bool operator==(const Type &t1, const Type &t2);
};

inline bool operator==(const Type &t1, const Type &t2) {return t1._classList == t2._classList;}
inline bool operator!=(const Type &t1, const Type &t2) {return !(t1 == t2);}

std::shared_ptr<Data> constructFrom(const std::vector<ObjectProperties>&);

class Function {
protected:
	bool _complete;
	Ecii::String _name;
	Type _inputType, _outputType;
	std::shared_ptr<Class> _constructorClass;
	
	Function(const Ecii::String& name, const List<ObjectProperties> &input, const List<ObjectProperties> &output) : _complete(false), _name(name), _inputType(Type::from(input)), _outputType(Type::from(output)), _constructorClass(nullptr) {}
	Function(const std::shared_ptr<Class>&, const List<ObjectProperties>&);
public:
	virtual bool isBuiltIn() const {return false;}
	bool isComplete() const {return _complete;}
	bool isConstructor() const {return _constructorClass != nullptr;}
	bool isMatching(const Ecii::String &name, const Type &input) const {return name == _name && input == _inputType;}
	Ecii::String getName() const {return _name;}
	Type getInputType()  const {return _inputType;}
	Type getOutputType() const {return _outputType;}
};

class UserFunction : public Function {
	std::vector<Instruction> _instructionSequence;
	std::vector<ObjectProperties> _inputOP, _outputOP;
public:
	UserFunction(const Ecii::String&, const List<ObjectProperties>&, const List<ObjectProperties>&, const std::vector<Instruction>&);
	UserFunction(const std::shared_ptr<Class>&, const List<ObjectProperties>&, const std::vector<Instruction>&);
	UserFunction(const Ecii::String& name, const List<ObjectProperties>& input, const List<ObjectProperties>& output) : Function(name, input, output), _inputOP(input.linearized()), _outputOP(output.linearized()) {}
	UserFunction(const std::shared_ptr<Class> &c, const List<ObjectProperties> &input) : Function(c, input), _inputOP(input.linearized()), _outputOP{ObjectProperties(c, "", false, false)} {}
	
	const std::vector<Instruction>& instructions() const {return _instructionSequence;}
	
	std::shared_ptr<Data> createInput() const;
	std::shared_ptr<Data> createOutput() const;
};

class BuiltInFunction : public Function {
public:
	BuiltInFunction(const Ecii::String& name, const List<ObjectProperties> &input, const List<ObjectProperties> &output) : Function(name, input, output) {_complete = true;}
	BuiltInFunction(const std::shared_ptr<Class> &c, const List<ObjectProperties> &input) : Function(c, input) {_complete = true;}
	bool isBuiltIn() const {return true;}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) = 0;
};

class Environment {
	std::shared_ptr<Environment> _parent;
	std::vector<std::shared_ptr<Class>> _classes;
	std::vector<std::shared_ptr<Function>> _functions;
	std::vector<std::shared_ptr<ObjectProperties>> _objects;
	
	std::shared_ptr<ObjectProperties> _getObject(const Ecii::String&, Vect2<uintmax_t>&) const;
public:
	Environment(const std::shared_ptr<Environment> &parent = nullptr) : _parent(parent) {}
	
	bool isUsedFunctionName(const Ecii::String&) const;
	
	std::shared_ptr<Class> getClass(const Ecii::String&) const;
	std::shared_ptr<Function> getFunction(const Ecii::String&, const Type&) const;
	std::shared_ptr<ObjectProperties> getObject(const Ecii::String&) const;
	std::shared_ptr<ObjectProperties> getObject(const Vect2<uintmax_t>&) const;
	Vect2<uintmax_t> getObjectPos(const Ecii::String&) const;
	void add(const std::shared_ptr<Function> &f) {if (f == nullptr ? false : getFunction(f->getName(), f->getInputType()) == nullptr) _functions.push_back(f);}
	void add(const std::shared_ptr<Class> &c) {if (c == nullptr ? false : getClass(c->getName()) == nullptr) _classes.push_back(c);}
	void add(const ObjectProperties &object) {if (getObject(object.getName()) == nullptr) _objects.push_back(std::make_shared<ObjectProperties>(object));}
	void add(const List<ObjectProperties>&);
	
	std::shared_ptr<Class> classOf(const Ecii::String&) const;
	Type typeOf(const Expression&) const;
	bool isAssignable(const Expression&) const;
	std::vector<std::shared_ptr<Class>> getIncompleteClasses() const;
	std::vector<std::shared_ptr<Function>> getIncompleteFunctions() const;
};

#endif