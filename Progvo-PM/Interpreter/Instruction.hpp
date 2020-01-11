// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Instruction_hpp
#define HEADER_Instruction_hpp

#include "../Parser/Environment.hpp"

struct Data {
	std::shared_ptr<void> data;
	Data() : data(nullptr) {}
	Data(const std::shared_ptr<void> &d) : data(d) {}
	virtual bool isReference() const {return false;}
	virtual bool isVector() const {return false;}
	virtual std::vector<std::shared_ptr<Data>> get() const {return {};}
	virtual void copy(const std::shared_ptr<Data>&) = 0;
};

struct ReferenceData : public Data {
	ReferenceData() : Data() {}
	virtual bool isReference() const {return true;}
	void copy(const std::shared_ptr<Data>& d) {
		if (data == nullptr) {
			if (d != nullptr ? !d->isReference() : false)
				data = d;
		}
	}
};

template <class T> struct BuiltInData : public Data {
	BuiltInData() : Data() {}
	BuiltInData(const std::shared_ptr<void> &d) : Data(d) {}
	BuiltInData(const T &t) {data = std::make_shared<T>(t);}
	void copy(const std::shared_ptr<Data>& d) {
		data = std::make_shared<T>(*std::static_pointer_cast<T>(d->data));
	}
};

struct UserData : public Data {
	std::vector<std::shared_ptr<Data>> attributes;
	virtual void copy(const std::shared_ptr<Data> &d) {
		for (uintmax_t i(0) ; i < attributes.size() ; i++)
			std::static_pointer_cast<Data>(attributes[i])->copy(std::static_pointer_cast<UserData>(d)->attributes[i]);
	}
};

struct VectorData : public Data {
	std::vector<std::shared_ptr<Data>> objects;
	VectorData() : Data() {}
	VectorData(const std::vector<std::shared_ptr<Data>> &vd) : Data() {objects = vd;}
	virtual bool isVector() const {return true;}
	virtual std::vector<std::shared_ptr<Data>> get() const {return objects;}
	virtual void copy(const std::shared_ptr<Data> &d) {
		for (uintmax_t i(0) ; i < objects.size() ; i++)
			std::static_pointer_cast<Data>(objects[i])->copy(std::static_pointer_cast<VectorData>(d)->objects[i]);
	}
};

struct ObjectLocation {
	Vect2<uintmax_t> pos;
	std::vector<uintmax_t> attributeLocation;
	ObjectLocation(const Vect2<uintmax_t>& pos0, const std::vector<uintmax_t>& attributeLocation0) : pos(pos0), attributeLocation(attributeLocation0) {}
};

inline bool operator==(const ObjectLocation &ol1, const ObjectLocation &ol2) {return ol1.pos == ol2.pos && ol1.attributeLocation == ol2.attributeLocation;}

class Expression {
public:
	enum class Type {Invalid, Empty, Call, Object, List, AccessChain};
private:
	Type _type;
	std::vector<Expression> _children;
	std::shared_ptr<Data> _data;
	std::shared_ptr<Class> _class;
	std::shared_ptr<Function> _function;
	std::shared_ptr<Expression> _input;
	Vect2<uintmax_t> _pos;
public:
	Expression() : _type(Type::Invalid) {}
	
	bool isValid() const {return _type != Type::Invalid;}
	bool isEmpty() const {return _type == Type::Empty;}
	bool isConstant() const {return _type == Type::Object && _data != nullptr && _class != nullptr;}
	bool isVariable() const {return _type == Type::Object && _pos.x != -1ULL && _pos.y != -1ULL;}
	bool isObject() const {return isConstant() || isVariable();}
	bool isAttributeAccess() const {return _type == Type::Object && _pos.x == -1ULL && _pos.y != -1ULL;}
	bool isCall() const {return _type == Type::Call && _function != nullptr && _input != nullptr;}
	bool isList() const {return _type == Type::List;}
	bool isAccessChain() const {return _type == Type::AccessChain;}
	bool isOnlyAttributeAccesses() const {
		if (_type == Type::AccessChain) {
			for (std::vector<Expression>::size_type i(1) ; i < _children.size() ; i++) {
				if (!_children[i].isAttributeAccess())
					return false;
			}
			return true;
		}
		return false;
	}
	const std::vector<Expression>& children() const {return _children;}
	std::shared_ptr<Data> getConstant() const {return _data;}
	std::shared_ptr<Class> getConstantClass() const {return _class;}
	Vect2<uintmax_t> getObjectPos() const {return _pos;}
	uintmax_t getAttributePos() const {return _pos.y;}
	std::shared_ptr<Function> getFunction() const {return _function;}
	std::shared_ptr<Expression> getCallInput() const {return _input;}
	ObjectLocation getAttributesPos() const {
		if (isOnlyAttributeAccesses()) {
			std::vector<uintmax_t> attributesPos;
			for (std::vector<Expression>::size_type i(1) ; i < _children.size() ; i++)
				attributesPos.push_back(_children[i].getAttributePos());
			return ObjectLocation(_children[0].getObjectPos(), attributesPos);
		}
		else return ObjectLocation(Vect2<uintmax_t>(-1ULL, -1ULL), std::vector<uintmax_t>{});
	}
	void setEmpty() {
		*this = Expression();
		_type = Type::Empty;
	}
	void setConstant(const std::shared_ptr<Class> &c, const std::shared_ptr<Data> &data) {
		*this = Expression();
		if (data != nullptr) {
			_class = c;
			_data = data;
			_type = Type::Object;
		}
	}
	void setObjectPos(const Vect2<uintmax_t> &pos) {
		*this = Expression();
		_pos = pos;
		_type = Type::Object;
	}
	void setCall(const std::shared_ptr<Function> &function, const Expression &input) {
		*this = Expression();
		_function = function;
		_input = std::make_shared<Expression>(input);
		_type = Type::Call;
	}
	void addSubExpression(const Expression &subexpression) {
		if (_type == Type::Object || _type == Type::Call || _type == Type::AccessChain) {
			Expression temp(*this);
			*this = Expression();
			_children.push_back(temp);
			_type = Type::List;
		}
		if (_type == Type::List)
			_children.push_back(subexpression);
		else if (_type == Type::Empty) *this = subexpression;
		else *this = Expression();
	}
	void setAttributeAccess(const uintmax_t &pos) {
		*this = Expression();
		_pos = Vect2<uintmax_t>(-1ULL, pos);
		_type = Type::Object;
	}
	void addAccess(const Expression &accessData) {
		if (_type == Type::Invalid || _type == Type::Empty || _type == Type::List || (!accessData.isAttributeAccess() && !accessData.isCall())) {
			*this = Expression();
			return;
		}
		else if (isObject() || isCall()) {
			Expression temp(*this);
			*this = Expression();
			_children.push_back(temp);
			_type = Type::AccessChain;
		}
		if (_type == Type::AccessChain) {
			_children.push_back(accessData);
		}
		else *this = Expression();
	}
	
	std::vector<ObjectLocation> toObjectLocations() const {
		if (isVariable())
			return {ObjectLocation(getObjectPos(), std::vector<uintmax_t>{})};
		else if (isOnlyAttributeAccesses())
			return {getAttributesPos()};
		else if (isList()) {
			std::vector<ObjectLocation> objectPos;
			for (const auto &c : children()) {
				std::vector<ObjectLocation> childObjectsPos(c.toObjectLocations());
				for (const auto &cop : childObjectsPos)
					objectPos.push_back(cop);
			}
			return objectPos;
		}
		else return {};
	}
};



class Instruction {
public:
	enum class Type {Invalid, Declaration, Assignment, Evaluation, Condition, Loop, Stop, Return, Break};
	Instruction() : _type(Type::Invalid) {}
	
	Type getType() const {return _type;}
	void setDeclaration(const ObjectProperties &declaration) {
		*this = Instruction();
		_type = Type::Declaration;
		if (declaration.isReference()) _constructor = [](){return std::make_shared<ReferenceData>();};
		else _constructor = declaration.getClass()->getConstructor();
	}
	const std::vector<ObjectLocation>& getAssignmentObjectsPos() const {return _objectsPos;}
	const Expression& getAssignmentExpression() const {return _expression;}
	void setAssignment(const std::vector<ObjectLocation> &objectsPos, const Expression &input) {
		*this = Instruction();
		_type = Type::Assignment;
		_objectsPos = objectsPos;
		_expression = input;
	}
	const Expression& getEvaluationExpression() const {
		return _expression;
	}
	void setEvaluation(const Expression &expression) {
		*this = Instruction();
		_type = Type::Evaluation;
		_expression = expression;
	}
	const std::vector<std::pair<Expression, std::vector<Instruction>>>& getConditionStructure() const {return _conditionStructure;}
	void addCondition(const Expression &condition, const std::vector<Instruction> &instructionSequence) {
		_type = Type::Condition;
		_conditionStructure.push_back(std::make_pair(condition, instructionSequence));
	}
	const auto& getLoopStructure() const {return _loopData;}
	void setLoop(const Expression &condition, const std::vector<Instruction> &instructionSequence) {
		_type = Type::Loop;
		_loopData = std::make_pair(condition, instructionSequence);
	}
	void setStop() {*this = Instruction(); _type = Type::Stop;}
	void setReturn() {*this = Instruction();_type = Type::Return;}
	void setBreak() { *this = Instruction(); _type = Type::Break;}
	std::shared_ptr<Data> construct() const {return _constructor();}
protected:
	Type _type;
	std::vector<ObjectLocation> _objectsPos;
	Expression _expression;
	std::function<std::shared_ptr<Data>()> _constructor;
	
	std::pair<Expression, std::vector<Instruction>> _loopData;
	std::vector<std::pair<Expression, std::vector<Instruction>>> _conditionStructure;
};

#endif