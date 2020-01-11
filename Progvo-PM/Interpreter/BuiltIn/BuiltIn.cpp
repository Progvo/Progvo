// (c) 2019-2020 Pttn (https://Progvo.dev)

#include "BuiltIn.hpp"

BuiltInFunction::BuiltInFunction(const Ecii::String& name, List<ObjectProperties> input, List<ObjectProperties> output) :
	Function(name, input, output) {
	_complete = true;
	_inputType = Type::from(input);
	_outputType = Type::from(output);
}
BuiltInFunction::BuiltInFunction(std::shared_ptr<Class> c, List<ObjectProperties> input) :
	Function(c, input, std::vector<Instruction>()) {
	_complete = true;
	_inputType = Type::from(input);
	_outputType = c;
}

BuiltInEqualsBooleans::BuiltInEqualsBooleans() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInBoolean, "", true, true), ObjectProperties(builtInBoolean, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
std::shared_ptr<Data> BuiltInEqualsBooleans::operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
	return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<bool>(std::static_pointer_cast<MultiData>(objects)->objects[0]->data) == *std::static_pointer_cast<bool>(std::static_pointer_cast<MultiData>(objects)->objects[1]->data));
}