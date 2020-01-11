// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_BuiltIn_hpp
#define HEADER_BuiltIn_hpp

#include <thread>
#include "../Instruction.hpp"
#include "NNumber.hpp"
#include "ZNumber.hpp"
#include "QNumber.hpp"
#include "Canvas.hpp"

inline std::shared_ptr<Class>
	builtInBitSequence(std::make_shared<Class>("Baz/Bitsekvenco", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<BitSequence>>();})),
	builtInBoolean(std::make_shared<Class>("Baz/Buleo", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<bool>>(false);})),
	builtInNNumber(std::make_shared<Class>("Baz/N", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<NNumber>>(NNumber::zero());})),
	builtInZNumber(std::make_shared<Class>("Baz/Z", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<ZNumber>>(ZNumber::zero());})),
	builtInQNumber(std::make_shared<Class>("Baz/Q", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<QNumber>>(QNumber::zero());})),
	builtInEcii(std::make_shared<Class>("Baz/Ekio", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<Ecii::String>>(Ecii::String());})),
	builtInPointer(std::make_shared<Class>("Ind/Adreso", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<Word>>(0ULL);})),
	builtInNatUInt(std::make_shared<Class>("Ind/N", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<Word>>(0ULL);})),
	builtInNatInt(std::make_shared<Class>("Ind/Z", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<Word>>(0LL);})),
	builtInNatReal(std::make_shared<Class>("Ind/R", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<Real>>(0.L);})),
	builtInCanvas(std::make_shared<Class>("Baz/Kanvaso", std::vector<std::shared_ptr<Function>>(), std::vector<ObjectProperties>(), [](){return std::make_shared<BuiltInData<Real>>(0.L);}));

inline std::shared_ptr<Ecii::String> builtInStandardOutput(std::make_shared<Ecii::String>());
inline std::function<Ecii::String()> builtInStandardInputFunction;

class BuiltInEqualsReferencePointer : public BuiltInFunction {
public:
	BuiltInEqualsReferencePointer() : BuiltInFunction("~", std::vector<ObjectProperties>{ObjectProperties(nullptr, "", true, true), ObjectProperties(builtInPointer, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		if (!std::static_pointer_cast<VectorData>(objects)->objects[0]->isReference()) return std::make_shared<BuiltInData<bool>>(false);
		return std::make_shared<BuiltInData<bool>>(std::static_pointer_cast<ReferenceData>(std::static_pointer_cast<VectorData>(objects)->objects[0])->data == nullptr && *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data) == 0);
	}
};

class BuiltInEqualsBooleans : public BuiltInFunction {
public:
	BuiltInEqualsBooleans() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInBoolean, "", true, true), ObjectProperties(builtInBoolean, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<bool>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<bool>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInNotBoolean : public BuiltInFunction {
public:
	BuiltInNotBoolean() : BuiltInFunction(std::byte{80}, {ObjectProperties(builtInBoolean, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(!*std::static_pointer_cast<bool>(objects->data));
	}
};

class BuiltInOrBoolean : public BuiltInFunction {
public:
	BuiltInOrBoolean() : BuiltInFunction(std::byte{81}, std::vector<ObjectProperties>{ObjectProperties(builtInBoolean, "", true, true), ObjectProperties(builtInBoolean, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}  // ∨
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<bool>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) || *std::static_pointer_cast<bool>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInAndBoolean : public BuiltInFunction {
public:
	BuiltInAndBoolean() : BuiltInFunction(std::byte{82}, std::vector<ObjectProperties>{ObjectProperties(builtInBoolean, "", true, true), ObjectProperties(builtInBoolean, "", true, true)}, {ObjectProperties(builtInBoolean)}) {} // ∧
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<bool>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) && *std::static_pointer_cast<bool>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInPrintBoolean : public BuiltInFunction {
public:
	BuiltInPrintBoolean() : BuiltInFunction("Baz/printi", {ObjectProperties(builtInBoolean, "", true, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*std::static_pointer_cast<bool>(objects->data) ? *builtInStandardOutput += "VERA" : *builtInStandardOutput += "MALVERA";
		return nullptr;
	}
};

class BuiltInInputBoolean : public BuiltInFunction {
public:
	BuiltInInputBoolean() : BuiltInFunction("Baz/enigi", {ObjectProperties(builtInBoolean, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		Ecii::String input(builtInStandardInputFunction());
		*std::static_pointer_cast<bool>(objects->data) = input == "VERA";
		return nullptr;
	}
};

class BuiltInBitSequenceFromBoolean : public BuiltInFunction {
public:
	BuiltInBitSequenceFromBoolean() : BuiltInFunction(builtInBitSequence, {ObjectProperties(builtInBoolean, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<BitSequence>>(*std::static_pointer_cast<bool>(objects->data));
	}
};

class BuiltInBitSequenceLength : public BuiltInFunction {
public:
	BuiltInBitSequenceLength() :  BuiltInFunction("longo", {}, {ObjectProperties(builtInNatUInt, "", false, false)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		return std::make_shared<BuiltInData<Word>>(std::static_pointer_cast<BitSequence>(thisObject->data)->size());
	}
};

class BuiltInBitSequenceGet : public BuiltInFunction {
public:
	BuiltInBitSequenceGet() :  BuiltInFunction("get", {ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInBoolean, "", false, false)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(std::static_pointer_cast<BitSequence>(thisObject->data)->get(*std::static_pointer_cast<Word>(objects->data)));
	}
};

class BuiltInBitSequenceSet : public BuiltInFunction {
public:
	BuiltInBitSequenceSet() :  BuiltInFunction("set", std::vector<ObjectProperties>{ObjectProperties(builtInBoolean, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>& objects) {
		std::static_pointer_cast<BitSequence>(thisObject->data)->set(*std::static_pointer_cast<bool>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data), *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
		return nullptr;
	}
};

class BuiltInEqualsBitSequences : public BuiltInFunction {
public:
	BuiltInEqualsBitSequences() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInBitSequence, "", true, true), ObjectProperties(builtInBitSequence, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<BitSequence>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<BitSequence>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInNotBitSequence : public BuiltInFunction {
public:
	BuiltInNotBitSequence() : BuiltInFunction(std::byte{80}, {ObjectProperties(builtInBitSequence, "", true, true)}, {ObjectProperties(builtInBitSequence)}) {} // ¬
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<BitSequence>>(~*std::static_pointer_cast<BitSequence>(objects->data));
	}
};

class BuiltInConcatenateBitSequence : public BuiltInFunction {
public:
	BuiltInConcatenateBitSequence() : BuiltInFunction("Baz/kunmeti", std::vector<ObjectProperties>{ObjectProperties(builtInBitSequence, "", true, true), ObjectProperties(builtInBitSequence, "", true, true)}, {ObjectProperties(builtInBitSequence)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<BitSequence>>(*std::static_pointer_cast<BitSequence>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<BitSequence>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInPrintBitsequence : public BuiltInFunction {
public:
	BuiltInPrintBitsequence() : BuiltInFunction("Baz/printi", {ObjectProperties(builtInBitSequence, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += std::static_pointer_cast<BitSequence>(objects->data)->vbool();
		return nullptr;
	}
};

class BuiltInInputBitSequence : public BuiltInFunction {
public:
	BuiltInInputBitSequence() : BuiltInFunction("Baz/enigi", {ObjectProperties(builtInBitSequence, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		std::vector<std::byte> input(builtInStandardInputFunction().vb());
		BitSequence bs;
		for (const std::byte &b : input) {
			if (b == std::byte{1}) bs.add(true);
			else if (b == std::byte{0}) bs.add(false);
			else {
				bs = BitSequence();
				break;
			}
		}
		*std::static_pointer_cast<BitSequence>(objects->data) = bs;
		return nullptr;
	}
};

class BuiltInEciiFromBitSequence : public BuiltInFunction {
public:
	BuiltInEciiFromBitSequence() : BuiltInFunction(builtInEcii, {ObjectProperties(builtInBitSequence, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Ecii::String>>(std::static_pointer_cast<BitSequence>(objects->data)->vbyte());
	}
};

class BuiltInEciiLength : public BuiltInFunction {
public:
	BuiltInEciiLength() :  BuiltInFunction("longo", {}, {ObjectProperties(builtInNatUInt, "", false, false)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		return std::make_shared<BuiltInData<Word>>(std::static_pointer_cast<Ecii::String>(thisObject->data)->size());
	}
};

class BuiltInEciiGet : public BuiltInFunction {
public:
	BuiltInEciiGet() :  BuiltInFunction("get", {ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInBitSequence, "", false, false)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<BitSequence>>(std::static_pointer_cast<Ecii::String>(thisObject->data)->getChar(*std::static_pointer_cast<Word>(objects->data)));
	}
};

class BuiltInEciiSet : public BuiltInFunction {
public:
	BuiltInEciiSet() :  BuiltInFunction("set", std::vector<ObjectProperties>{ObjectProperties(builtInBitSequence, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>& objects) {
		const BitSequence bs(*std::static_pointer_cast<BitSequence>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data));
		if (bs.size() != 8)
			*builtInStandardOutput += "Baz/Ekio/set: la bitsekvenco ne havas longo de 8!";
		else 
			std::static_pointer_cast<Ecii::String>(thisObject->data)->setChar(bs.vbyte()[0], *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
		return nullptr;
	}
};

class BuiltInEqualsEcii : public BuiltInFunction {
public:
	BuiltInEqualsEcii() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInEcii, "", true, true), ObjectProperties(builtInEcii, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Ecii::String>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<Ecii::String>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInConcatenateEcii : public BuiltInFunction {
public:
	BuiltInConcatenateEcii() : BuiltInFunction("Baz/kunmeti", std::vector<ObjectProperties>{ObjectProperties(builtInEcii, "", true, true), ObjectProperties(builtInEcii, "", true, true)}, {ObjectProperties(builtInEcii)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Ecii::String>>(*std::static_pointer_cast<Ecii::String>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<Ecii::String>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInPrintEcii : public BuiltInFunction {
public:
	BuiltInPrintEcii() : BuiltInFunction("Baz/printi", {ObjectProperties(builtInEcii, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += *std::static_pointer_cast<Ecii::String>(objects->data);
		return nullptr;
	}
};

class BuiltInInputEcii : public BuiltInFunction {
public:
	BuiltInInputEcii() : BuiltInFunction("Baz/enigi", {ObjectProperties(builtInEcii, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*std::static_pointer_cast<Ecii::String>(objects->data) = builtInStandardInputFunction();
		return nullptr;
	}
};

class BuiltInNNumberFromBitSequence : public BuiltInFunction {
public:
	BuiltInNNumberFromBitSequence() : BuiltInFunction(builtInNNumber, {ObjectProperties(builtInBitSequence, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<NNumber>>(*std::static_pointer_cast<BitSequence>(objects->data));
	}
};

class BuiltInNNumberFromEcii : public BuiltInFunction {
public:
	BuiltInNNumberFromEcii() : BuiltInFunction(builtInNNumber, {ObjectProperties(builtInEcii, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<NNumber>>(*std::static_pointer_cast<Ecii::String>(objects->data));
	}
};

class BuiltInNNumberInc : public BuiltInFunction {
public:
	BuiltInNNumberInc() :  BuiltInFunction("alk", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<NNumber>(thisObject->data))++;
		return nullptr;
	}
};

class BuiltInNNumberDec : public BuiltInFunction {
public:
	BuiltInNNumberDec() :  BuiltInFunction("dek", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<NNumber>(thisObject->data))--;
		return nullptr;
	}
};

class BuiltInEqualsNNumber : public BuiltInFunction {
public:
	BuiltInEqualsNNumber() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessThanNNumber : public BuiltInFunction {
public:
	BuiltInLessThanNNumber() : BuiltInFunction("<", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) < *std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterThanNNumber : public BuiltInFunction {
public:
	BuiltInGreaterThanNNumber() : BuiltInFunction(">", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) > *std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessOrEqualNNumber : public BuiltInFunction {
public:
	BuiltInLessOrEqualNNumber() : BuiltInFunction("<=", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) <= *std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterOrEqualNNumber : public BuiltInFunction {
public:
	BuiltInGreaterOrEqualNNumber() : BuiltInFunction(">=", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) >= *std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInAddNNumber : public BuiltInFunction {
public:
	BuiltInAddNNumber() : BuiltInFunction("+", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInNNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<NNumber>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInSubNNumber : public BuiltInFunction {
public:
	BuiltInSubNNumber() : BuiltInFunction("-", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInNNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<NNumber>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) - *std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInMulNNumber : public BuiltInFunction {
public:
	BuiltInMulNNumber() : BuiltInFunction(std::byte{12}, std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInNNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<NNumber>>((*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data))*(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInDivNNumber : public BuiltInFunction {
public:
	BuiltInDivNNumber() : BuiltInFunction(std::byte{13}, std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInNNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<NNumber>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)/(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInModNNumber : public BuiltInFunction {
public:
	BuiltInModNNumber() : BuiltInFunction("mod", std::vector<ObjectProperties>{ObjectProperties(builtInNNumber, "", true, true), ObjectProperties(builtInNNumber, "", true, true)}, {ObjectProperties(builtInNNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<NNumber>>(*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) % (*std::static_pointer_cast<NNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInPrintNNumber : public BuiltInFunction {
public:
	BuiltInPrintNNumber() : BuiltInFunction("Baz/printi", {ObjectProperties(builtInNNumber, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += std::static_pointer_cast<NNumber>(objects->data)->es(10);
		return nullptr;
	}
};

class BuiltInInputNNumber : public BuiltInFunction {
public:
	BuiltInInputNNumber() : BuiltInFunction("Baz/enigi", {ObjectProperties(builtInNNumber, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*std::static_pointer_cast<NNumber>(objects->data) = NNumber(builtInStandardInputFunction());
		return nullptr;
	}
};

class BuiltInZNumberFromEcii : public BuiltInFunction {
public:
	BuiltInZNumberFromEcii() : BuiltInFunction(builtInZNumber, {ObjectProperties(builtInEcii, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<ZNumber>>(*std::static_pointer_cast<Ecii::String>(objects->data));
	}
};

class BuiltInZNumberInc : public BuiltInFunction {
public:
	BuiltInZNumberInc() :  BuiltInFunction("alk", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<ZNumber>(thisObject->data))++;
		return nullptr;
	}
};

class BuiltInZNumberDec : public BuiltInFunction {
public:
	BuiltInZNumberDec() :  BuiltInFunction("dek", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<ZNumber>(thisObject->data))--;
		return nullptr;
	}
};

class BuiltInEqualsZNumber : public BuiltInFunction {
public:
	BuiltInEqualsZNumber() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessThanZNumber : public BuiltInFunction {
public:
	BuiltInLessThanZNumber() : BuiltInFunction("<", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) < *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterThanZNumber : public BuiltInFunction {
public:
	BuiltInGreaterThanZNumber() : BuiltInFunction(">", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) > *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessOrEqualZNumber : public BuiltInFunction {
public:
	BuiltInLessOrEqualZNumber() : BuiltInFunction("<=", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) <= *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterOrEqualZNumber : public BuiltInFunction {
public:
	BuiltInGreaterOrEqualZNumber() : BuiltInFunction(">=", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) >= *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInOppZNumber : public BuiltInFunction {
public:
	BuiltInOppZNumber() : BuiltInFunction("-", {ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInZNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<ZNumber>>(-(*std::static_pointer_cast<ZNumber>(objects->data)));
	}
};

class BuiltInAddZNumber : public BuiltInFunction {
public:
	BuiltInAddZNumber() : BuiltInFunction("+", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInZNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<ZNumber>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInSubZNumber : public BuiltInFunction {
public:
	BuiltInSubZNumber() : BuiltInFunction("-", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInZNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<ZNumber>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) - *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInMulZNumber : public BuiltInFunction {
public:
	BuiltInMulZNumber() : BuiltInFunction(std::byte{12}, std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInZNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<ZNumber>>((*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data))*(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInDivZNumber : public BuiltInFunction {
public:
	BuiltInDivZNumber() : BuiltInFunction(std::byte{13}, std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInZNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<ZNumber>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)/(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInModZNumber : public BuiltInFunction {
public:
	BuiltInModZNumber() : BuiltInFunction("mod", std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}, {ObjectProperties(builtInZNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<ZNumber>>(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) % (*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInPrintZNumber : public BuiltInFunction {
public:
	BuiltInPrintZNumber() : BuiltInFunction("Baz/printi", {ObjectProperties(builtInZNumber, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += std::static_pointer_cast<ZNumber>(objects->data)->es(10);
		return nullptr;
	}
};

class BuiltInInputZNumber : public BuiltInFunction {
public:
	BuiltInInputZNumber() : BuiltInFunction("Baz/enigi", {ObjectProperties(builtInZNumber, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*std::static_pointer_cast<ZNumber>(objects->data) = ZNumber(builtInStandardInputFunction());
		return nullptr;
	}
};

class BuiltInQNumberFromEcii : public BuiltInFunction {
public:
	BuiltInQNumberFromEcii() : BuiltInFunction(builtInQNumber, {ObjectProperties(builtInEcii, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<QNumber>>(*std::static_pointer_cast<Ecii::String>(objects->data));
	}
};

class BuiltInQNumberFromZNumbers : public BuiltInFunction {
public:
	BuiltInQNumberFromZNumbers() : BuiltInFunction(builtInQNumber, std::vector<ObjectProperties>{ObjectProperties(builtInZNumber, "", true, true), ObjectProperties(builtInZNumber, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<QNumber>>(QNumber(*std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data), *std::static_pointer_cast<ZNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInQNumberInc : public BuiltInFunction {
public:
	BuiltInQNumberInc() :  BuiltInFunction("alk", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<QNumber>(thisObject->data))++;
		return nullptr;
	}
};

class BuiltInQNumberDec : public BuiltInFunction {
public:
	BuiltInQNumberDec() :  BuiltInFunction("dek", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<QNumber>(thisObject->data))--;
		return nullptr;
	}
};

class BuiltInEqualsQNumber : public BuiltInFunction {
public:
	BuiltInEqualsQNumber() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessThanQNumber : public BuiltInFunction {
public:
	BuiltInLessThanQNumber() : BuiltInFunction("<", std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) < *std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterThanQNumber : public BuiltInFunction {
public:
	BuiltInGreaterThanQNumber() : BuiltInFunction(">", std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) > *std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessOrEqualQNumber : public BuiltInFunction {
public:
	BuiltInLessOrEqualQNumber() : BuiltInFunction("<=", std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) <= *std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterOrEqualQNumber : public BuiltInFunction {
public:
	BuiltInGreaterOrEqualQNumber() : BuiltInFunction(">=", std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) >= *std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInOppQNumber : public BuiltInFunction {
public:
	BuiltInOppQNumber() : BuiltInFunction("-", {ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInQNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<QNumber>>(-(*std::static_pointer_cast<QNumber>(objects->data)));
	}
};

class BuiltInAddQNumber : public BuiltInFunction {
public:
	BuiltInAddQNumber() : BuiltInFunction("+", std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInQNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<QNumber>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInSubQNumber : public BuiltInFunction {
public:
	BuiltInSubQNumber() : BuiltInFunction("-", std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInQNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<QNumber>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) - *std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInMulQNumber : public BuiltInFunction {
public:
	BuiltInMulQNumber() : BuiltInFunction(std::byte{12}, std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInQNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<QNumber>>((*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data))*(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInDivQNumber : public BuiltInFunction {
public:
	BuiltInDivQNumber() : BuiltInFunction(std::byte{13}, std::vector<ObjectProperties>{ObjectProperties(builtInQNumber, "", true, true), ObjectProperties(builtInQNumber, "", true, true)}, {ObjectProperties(builtInQNumber)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<QNumber>>(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)/(*std::static_pointer_cast<QNumber>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInPrintQNumber : public BuiltInFunction {
public:
	BuiltInPrintQNumber() : BuiltInFunction("Baz/printi", {ObjectProperties(builtInQNumber, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += std::static_pointer_cast<QNumber>(objects->data)->es(10);
		return nullptr;
	}
};

class BuiltInInputQNumber : public BuiltInFunction {
public:
	BuiltInInputQNumber() : BuiltInFunction("Baz/enigi", {ObjectProperties(builtInQNumber, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*std::static_pointer_cast<QNumber>(objects->data) = QNumber(builtInStandardInputFunction());
		return nullptr;
	}
};

class BuiltInEqualsPointers : public BuiltInFunction {
public:
	BuiltInEqualsPointers() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInPointer, "", true, true), ObjectProperties(builtInPointer, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInNatUIntFromBitSequence : public BuiltInFunction {
public:
	BuiltInNatUIntFromBitSequence() : BuiltInFunction(builtInNatUInt, {ObjectProperties(builtInBitSequence, "", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>(std::static_pointer_cast<BitSequence>(objects->data)->to<Word>());
	}
};

class BuiltInNatUIntInc : public BuiltInFunction {
public:
	BuiltInNatUIntInc() :  BuiltInFunction("alk", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<Word>(thisObject->data))++;
		return nullptr;
	}
};

class BuiltInNatUIntDec : public BuiltInFunction {
public:
	BuiltInNatUIntDec() :  BuiltInFunction("dek", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<Word>(thisObject->data))--;
		return nullptr;
	}
};

class BuiltInEqualsNatUInt : public BuiltInFunction {
public:
	BuiltInEqualsNatUInt() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessThanNatUInt : public BuiltInFunction {
public:
	BuiltInLessThanNatUInt() : BuiltInFunction("<", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) < *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterThanNatUInt : public BuiltInFunction {
public:
	BuiltInGreaterThanNatUInt() : BuiltInFunction(">", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) > *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessOrEqualNatUInt : public BuiltInFunction {
public:
	BuiltInLessOrEqualNatUInt() : BuiltInFunction("<=", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) <= *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterOrEqualNatUInt : public BuiltInFunction {
public:
	BuiltInGreaterOrEqualNatUInt() : BuiltInFunction(">=", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) >= *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInAddNatUInt : public BuiltInFunction {
public:
	BuiltInAddNatUInt() : BuiltInFunction("+", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInNatUInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInSubNatUInt : public BuiltInFunction {
public:
	BuiltInSubNatUInt() : BuiltInFunction("-", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInNatUInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) - *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInMulNatUInt : public BuiltInFunction {
public:
	BuiltInMulNatUInt() : BuiltInFunction(std::byte{12}, std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInNatUInt)}) {} // ×
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>((*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data))*(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInDivNatUInt : public BuiltInFunction {
public:
	BuiltInDivNatUInt() : BuiltInFunction(std::byte{13}, std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInNatUInt)}) {} // ÷
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		if (*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data) == 0) {
			*builtInStandardOutput += "Divido per nulo!";
			return std::make_shared<BuiltInData<Word>>(-1ULL);
		}
		else return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)/(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInModNatUInt : public BuiltInFunction {
public:
	BuiltInModNatUInt() : BuiltInFunction("mod", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInNatUInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		if (*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data) == 0) {
			*builtInStandardOutput += "Divido per nulo!";
			return std::make_shared<BuiltInData<Word>>(-1ULL);
		}
		else return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) % *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInRandNatUInt : public BuiltInFunction {
public:
	BuiltInRandNatUInt() : BuiltInFunction("haz", std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "", true, true), ObjectProperties(builtInNatUInt, "", true, true)}, {ObjectProperties(builtInNatUInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>(rand(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data), *std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInPrintNatUInt : public BuiltInFunction {
public:
	BuiltInPrintNatUInt() : BuiltInFunction("Ind/printi", {ObjectProperties(builtInNatUInt, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += std::to_string(*std::static_pointer_cast<Word>(objects->data));
		return nullptr;
	}
};

class BuiltInInputNatUInt : public BuiltInFunction {
public:
	BuiltInInputNatUInt() : BuiltInFunction("Ind/enigi", {ObjectProperties(builtInNatUInt, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		try {
			*std::static_pointer_cast<Word>(objects->data) = std::stoul(builtInStandardInputFunction().str());
		}
		catch (...) {
			*std::static_pointer_cast<Word>(objects->data) = -1ULL;
		}
		return nullptr;
	}
};

class BuiltInNatIntInc : public BuiltInFunction {
public:
	BuiltInNatIntInc() :  BuiltInFunction("alk", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<int64_t>(thisObject->data))++;
		return nullptr;
	}
};

class BuiltInNatIntDec : public BuiltInFunction {
public:
	BuiltInNatIntDec() :  BuiltInFunction("dek", std::vector<ObjectProperties>{}, std::vector<ObjectProperties>{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>&) {
		(*std::static_pointer_cast<int64_t>(thisObject->data))--;
		return nullptr;
	}
};

class BuiltInEqualsNatInt : public BuiltInFunction {
public:
	BuiltInEqualsNatInt() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessThanNatInt : public BuiltInFunction {
public:
	BuiltInLessThanNatInt() : BuiltInFunction("<", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) < *std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterThanNatInt : public BuiltInFunction {
public:
	BuiltInGreaterThanNatInt() : BuiltInFunction(">", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) > *std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessOrEqualNatInt : public BuiltInFunction {
public:
	BuiltInLessOrEqualNatInt() : BuiltInFunction("<=", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) <= *std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterOrEqualNatInt : public BuiltInFunction {
public:
	BuiltInGreaterOrEqualNatInt() : BuiltInFunction(">=", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) >= *std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInOppNatInt : public BuiltInFunction {
public:
	BuiltInOppNatInt() : BuiltInFunction("-", {ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInNatInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>(-(*std::static_pointer_cast<int64_t>(objects->data)));
	}
};

class BuiltInAddNatInt : public BuiltInFunction {
public:
	BuiltInAddNatInt() : BuiltInFunction("+", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInNatInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInSubNatInt : public BuiltInFunction {
public:
	BuiltInSubNatInt() : BuiltInFunction("-", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInNatInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) - *std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInMulNatInt : public BuiltInFunction {
public:
	BuiltInMulNatInt() : BuiltInFunction(std::byte{12}, std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInNatInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Word>>((*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data))*(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInDivNatInt : public BuiltInFunction {
public:
	BuiltInDivNatInt() : BuiltInFunction(std::byte{13}, std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInNatInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		if (*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data) == 0) {
			*builtInStandardOutput += "Divido per nulo!";
			return std::make_shared<BuiltInData<Word>>(-1ULL);
		}
		return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)/(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInModNatInt : public BuiltInFunction {
public:
	BuiltInModNatInt() : BuiltInFunction("mod", std::vector<ObjectProperties>{ObjectProperties(builtInNatInt, "", true, true), ObjectProperties(builtInNatInt, "", true, true)}, {ObjectProperties(builtInNatInt)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		if (*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data) == 0) {
			*builtInStandardOutput += "Divido per nulo!";
			return std::make_shared<BuiltInData<Word>>(-1ULL);
		}
		return std::make_shared<BuiltInData<Word>>(*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) % (*std::static_pointer_cast<int64_t>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInPrintNatInt : public BuiltInFunction {
public:
	BuiltInPrintNatInt() : BuiltInFunction("Ind/printi", {ObjectProperties(builtInNatInt, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += std::to_string(*std::static_pointer_cast<int64_t>(objects->data));
		return nullptr;
	}
};

class BuiltInInputNatInt : public BuiltInFunction {
public:
	BuiltInInputNatInt() : BuiltInFunction("Ind/enigi", {ObjectProperties(builtInNatInt, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		try {
			*std::static_pointer_cast<int64_t>(objects->data) = std::stol(builtInStandardInputFunction().str());
		}
		catch (...) {
			*std::static_pointer_cast<int64_t>(objects->data) = -1ULL;
		}
		return nullptr;
	}
};

class BuiltInEqualsNatReal : public BuiltInFunction {
public:
	BuiltInEqualsNatReal() : BuiltInFunction("=", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) == *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessThanNatReal : public BuiltInFunction {
public:
	BuiltInLessThanNatReal() : BuiltInFunction("<", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) < *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterThanNatReal : public BuiltInFunction {
public:
	BuiltInGreaterThanNatReal() : BuiltInFunction(">", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) > *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInLessOrEqualNatReal : public BuiltInFunction {
public:
	BuiltInLessOrEqualNatReal() : BuiltInFunction("<=", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) <= *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInGreaterOrEqualNatReal : public BuiltInFunction {
public:
	BuiltInGreaterOrEqualNatReal() : BuiltInFunction(">=", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInBoolean)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<bool>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) >= *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInAddNatReal : public BuiltInFunction {
public:
	BuiltInAddNatReal() : BuiltInFunction("+", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInNatReal)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Real>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) + *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInSubNatReal : public BuiltInFunction {
public:
	BuiltInSubNatReal() : BuiltInFunction("-", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInNatReal)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Real>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data) - *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
	}
};

class BuiltInMulNatReal : public BuiltInFunction {
public:
	BuiltInMulNatReal() : BuiltInFunction(std::byte{12}, std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInNatReal)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Real>>((*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data))*(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInDivNatReal : public BuiltInFunction {
public:
	BuiltInDivNatReal() : BuiltInFunction(std::byte{13}, std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInNatReal)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Real>>(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)/(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInRandNatReal : public BuiltInFunction {
public:
	BuiltInRandNatReal() : BuiltInFunction("haz", std::vector<ObjectProperties>{ObjectProperties(builtInNatReal, "", true, true), ObjectProperties(builtInNatReal, "", true, true)}, {ObjectProperties(builtInNatReal)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<Real>>(rand(*std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data), *std::static_pointer_cast<Real>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)));
	}
};

class BuiltInPrintNatReal : public BuiltInFunction {
public:
	BuiltInPrintNatReal() : BuiltInFunction("Ind/printi", {ObjectProperties(builtInNatReal, "", true, true)}, {}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		*builtInStandardOutput += std::to_string(*std::static_pointer_cast<Real>(objects->data));
		return nullptr;
	}
};

class BuiltInInputNatReal : public BuiltInFunction {
public:
	BuiltInInputNatReal() : BuiltInFunction("Ind/enigi", {ObjectProperties(builtInNatReal, "", false, true)}, {}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		try {
			*std::static_pointer_cast<Real>(objects->data) = std::stold(builtInStandardInputFunction().str());
		}
		catch (...) {
			*std::static_pointer_cast<Real>(objects->data) = -1ULL;
		}
		return nullptr;
	}
};

class BuiltInLoadFile : public BuiltInFunction {
public:
	BuiltInLoadFile() : BuiltInFunction("Baz/" + ES(38) + "argi", {ObjectProperties(builtInEcii, "", true, true)}, {ObjectProperties(builtInBitSequence, "", false, false)}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		return std::make_shared<BuiltInData<BitSequence>>(load(*std::static_pointer_cast<Ecii::String>(objects->data)));
	}
};

class BuiltInSaveFile : public BuiltInFunction {
public:
	BuiltInSaveFile() : BuiltInFunction("Baz/skribi", std::vector<ObjectProperties>{ObjectProperties(builtInBitSequence, "", false, false), ObjectProperties(builtInEcii, "", true, true)}, std::vector<ObjectProperties>{}) {};
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		save(std::static_pointer_cast<BitSequence>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)->vbyte(), *std::static_pointer_cast<Ecii::String>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
		return nullptr;
	}
};

class BuiltInCanvasConstructor : public BuiltInFunction {
	static std::vector<std::shared_ptr<Canvas>> _canvasses;
	static std::vector<std::thread> _canvassesThreads;
public:
	BuiltInCanvasConstructor() : BuiltInFunction(builtInCanvas, std::vector<ObjectProperties>{ObjectProperties(builtInNatUInt, "l", true, true), ObjectProperties(builtInNatUInt, "a", true, true)}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>&, const std::shared_ptr<Data>& objects) {
		const Word &w(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)), &h(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data));
		std::shared_ptr<Canvas> canvas(std::make_shared<Canvas>(w, h));
		_canvasses.push_back(canvas);
		_canvassesThreads.push_back(std::thread(&Canvas::run, canvas));
		std::shared_ptr<BuiltInData<Canvas>> output(std::make_shared<BuiltInData<Canvas>>());
		output->data = canvas;
		return output;
	}
	
	static void reset() {
		for (auto &c  : _canvasses) c->close();
		for (auto &ct : _canvassesThreads) ct.join();
		_canvasses = std::vector<std::shared_ptr<Canvas>>();
		_canvassesThreads = std::vector<std::thread>();
	}
};

inline std::vector<std::shared_ptr<Canvas>> BuiltInCanvasConstructor::_canvasses;
inline std::vector<std::thread> BuiltInCanvasConstructor::_canvassesThreads;

class BuiltInCanvasSetPixel : public BuiltInFunction {
public:
	BuiltInCanvasSetPixel() : BuiltInFunction(
		"setBilderon",
		std::vector<ObjectProperties>{
			ObjectProperties(builtInNatUInt, "r", true, true),
			ObjectProperties(builtInNatUInt, "v", true, true),
			ObjectProperties(builtInNatUInt, "b", true, true),
			ObjectProperties(builtInNatUInt, "x", true, true),
			ObjectProperties(builtInNatUInt, "y", true, true)},
		{}) {}
	virtual std::shared_ptr<Data> operator()(const std::shared_ptr<Data>& thisObject, const std::shared_ptr<Data>& objects) {
		const Word& r(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[0]->data)),
		            g(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[1]->data)),
		            b(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[2]->data)),
		            x(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[3]->data)),
		            y(*std::static_pointer_cast<Word>(std::static_pointer_cast<VectorData>(objects)->objects[4]->data));
		std::static_pointer_cast<Canvas>(thisObject->data)->setPixel(r, g, b, Vect2<Word>(x, y));
		return nullptr;
	}
};

inline const std::shared_ptr<Function>
	builtInEqualsReferencePointer(std::make_shared<BuiltInEqualsReferencePointer>()),
	
	builtInEqualsBooleans(std::make_shared<BuiltInEqualsBooleans>()),
	builtInNotBoolean(std::make_shared<BuiltInNotBoolean>()),
	builtInOrBoolean(std::make_shared<BuiltInOrBoolean>()),
	builtInAndBoolean(std::make_shared<BuiltInAndBoolean>()),
	builtInPrintBoolean(std::make_shared<BuiltInPrintBoolean>()),
	builtInInputBoolean(std::make_shared<BuiltInInputBoolean>()),
	
	builtInEqualsBitSequence(std::make_shared<BuiltInEqualsBitSequences>()),
	builtInNotBitSequence(std::make_shared<BuiltInNotBitSequence>()),
	builtInConcatenateBitSequence(std::make_shared<BuiltInConcatenateBitSequence>()),
	builtInPrintBitsequence(std::make_shared<BuiltInPrintBitsequence>()),
	builtInInputBitSequence(std::make_shared<BuiltInInputBitSequence>()),
	
	builtInEqualsEcii(std::make_shared<BuiltInEqualsEcii>()),
	builtInConcatenateEcii(std::make_shared<BuiltInConcatenateEcii>()),
	builtInPrintEcii(std::make_shared<BuiltInPrintEcii>()),
	builtInInputEcii(std::make_shared<BuiltInInputEcii>()),
	
	builtInEqualsNNumber(std::make_shared<BuiltInEqualsNNumber>()),
	builtInLessThanNNumber(std::make_shared<BuiltInLessThanNNumber>()),
	builtInGreaterThanNNumber(std::make_shared<BuiltInGreaterThanNNumber>()),
	builtInLessOrEqualNNumber(std::make_shared<BuiltInLessOrEqualNNumber>()),
	builtInGreaterOrEqualNNumber(std::make_shared<BuiltInGreaterOrEqualNNumber>()),
	builtInAddNNumber(std::make_shared<BuiltInAddNNumber>()),
	builtInSubNNumber(std::make_shared<BuiltInSubNNumber>()),
	builtInMulNNumber(std::make_shared<BuiltInMulNNumber>()),
	builtInDivNNumber(std::make_shared<BuiltInDivNNumber>()),
	builtInModNNumber(std::make_shared<BuiltInModNNumber>()),
	builtInPrintNNumber(std::make_shared<BuiltInPrintNNumber>()),
	builtInInputNNumber(std::make_shared<BuiltInInputNNumber>()),
	
	builtInEqualsZNumber(std::make_shared<BuiltInEqualsZNumber>()),
	builtInLessThanZNumber(std::make_shared<BuiltInLessThanZNumber>()),
	builtInGreaterThanZNumber(std::make_shared<BuiltInGreaterThanZNumber>()),
	builtInLessOrEqualZNumber(std::make_shared<BuiltInLessOrEqualZNumber>()),
	builtInGreaterOrEqualZNumber(std::make_shared<BuiltInGreaterOrEqualZNumber>()),
	builtInOppZNumber(std::make_shared<BuiltInOppZNumber>()),
	builtInAddZNumber(std::make_shared<BuiltInAddZNumber>()),
	builtInSubZNumber(std::make_shared<BuiltInSubZNumber>()),
	builtInMulZNumber(std::make_shared<BuiltInMulZNumber>()),
	builtInDivZNumber(std::make_shared<BuiltInDivZNumber>()),
	builtInModZNumber(std::make_shared<BuiltInModZNumber>()),
	builtInPrintZNumber(std::make_shared<BuiltInPrintZNumber>()),
	builtInInputZNumber(std::make_shared<BuiltInInputZNumber>()),
	
	builtInEqualsQNumber(std::make_shared<BuiltInEqualsQNumber>()),
	builtInLessThanQNumber(std::make_shared<BuiltInLessThanQNumber>()),
	builtInGreaterThanQNumber(std::make_shared<BuiltInGreaterThanQNumber>()),
	builtInLessOrEqualQNumber(std::make_shared<BuiltInLessOrEqualQNumber>()),
	builtInGreaterOrEqualQNumber(std::make_shared<BuiltInGreaterOrEqualQNumber>()),
	builtInOppQNumber(std::make_shared<BuiltInOppQNumber>()),
	builtInAddQNumber(std::make_shared<BuiltInAddQNumber>()),
	builtInSubQNumber(std::make_shared<BuiltInSubQNumber>()),
	builtInMulQNumber(std::make_shared<BuiltInMulQNumber>()),
	builtInDivQNumber(std::make_shared<BuiltInDivQNumber>()),
	builtInPrintQNumber(std::make_shared<BuiltInPrintQNumber>()),
	builtInInputQNumber(std::make_shared<BuiltInInputQNumber>()),
	
	builtInEqualsPointers(std::make_shared<BuiltInEqualsPointers>()),
	
	builtInEqualsNatUInt(std::make_shared<BuiltInEqualsNatUInt>()),
	builtInLessThanNatUInt(std::make_shared<BuiltInLessThanNatUInt>()),
	builtInGreaterThanNatUInt(std::make_shared<BuiltInGreaterThanNatUInt>()),
	builtInLessOrEqualNatUInt(std::make_shared<BuiltInLessOrEqualNatUInt>()),
	builtInGreaterOrEqualNatUInt(std::make_shared<BuiltInGreaterOrEqualNatUInt>()),
	builtInAddNatUInt(std::make_shared<BuiltInAddNatUInt>()),
	builtInSubNatUInt(std::make_shared<BuiltInSubNatUInt>()),
	builtInMulNatUInt(std::make_shared<BuiltInMulNatUInt>()),
	builtInDivNatUInt(std::make_shared<BuiltInDivNatUInt>()),
	builtInModNatUInt(std::make_shared<BuiltInModNatUInt>()),
	builtInRandNatUInt(std::make_shared<BuiltInRandNatUInt>()),
	builtInPrintNatUInt(std::make_shared<BuiltInPrintNatUInt>()),
	builtInInputNatUInt(std::make_shared<BuiltInInputNatUInt>()),
	
	builtInEqualsNatInt(std::make_shared<BuiltInEqualsNatInt>()),
	builtInLessThanNatInt(std::make_shared<BuiltInLessThanNatInt>()),
	builtInGreaterThanNatInt(std::make_shared<BuiltInGreaterThanNatInt>()),
	builtInLessOrEqualNatInt(std::make_shared<BuiltInLessOrEqualNatInt>()),
	builtInGreaterOrEqualNatInt(std::make_shared<BuiltInGreaterOrEqualNatInt>()),
	builtInOppNatInt(std::make_shared<BuiltInOppNatInt>()),
	builtInAddNatInt(std::make_shared<BuiltInAddNatInt>()),
	builtInSubNatInt(std::make_shared<BuiltInSubNatInt>()),
	builtInMulNatInt(std::make_shared<BuiltInMulNatInt>()),
	builtInDivNatInt(std::make_shared<BuiltInDivNatInt>()),
	builtInModNatInt(std::make_shared<BuiltInModNatInt>()),
	builtInPrintNatInt(std::make_shared<BuiltInPrintNatInt>()),
	builtInInputNatInt(std::make_shared<BuiltInInputNatInt>()),
	
	builtInEqualsNatReal(std::make_shared<BuiltInEqualsNatReal>()),
	builtInLessThanNatReal(std::make_shared<BuiltInLessThanNatReal>()),
	builtInGreaterThanNatReal(std::make_shared<BuiltInGreaterThanNatReal>()),
	builtInLessOrEqualNatReal(std::make_shared<BuiltInLessOrEqualNatReal>()),
	builtInGreaterOrEqualNatReal(std::make_shared<BuiltInGreaterOrEqualNatReal>()),
	builtInAddNatReal(std::make_shared<BuiltInAddNatReal>()),
	builtInSubNatReal(std::make_shared<BuiltInSubNatReal>()),
	builtInMulNatReal(std::make_shared<BuiltInMulNatReal>()),
	builtInDivNatReal(std::make_shared<BuiltInDivNatReal>()),
	builtInRandNatReal(std::make_shared<BuiltInRandNatReal>()),
	builtInPrintNatReal(std::make_shared<BuiltInPrintNatReal>()),
	builtInInputNatReal(std::make_shared<BuiltInInputNatReal>()),
	
	builtInLoadFile(std::make_shared<BuiltInLoadFile>()),
	builtInSaveFile(std::make_shared<BuiltInSaveFile>());

inline void _addBuiltInMethods() {
	builtInBitSequence->addMethod(std::make_shared<BuiltInBitSequenceFromBoolean>());
	builtInBitSequence->addMethod(std::make_shared<BuiltInBitSequenceLength>());
	builtInBitSequence->addMethod(std::make_shared<BuiltInBitSequenceGet>());
	builtInBitSequence->addMethod(std::make_shared<BuiltInBitSequenceSet>());
	builtInEcii->addMethod(std::make_shared<BuiltInEciiLength>());
	builtInEcii->addMethod(std::make_shared<BuiltInEciiFromBitSequence>());
	builtInEcii->addMethod(std::make_shared<BuiltInEciiGet>());
	builtInEcii->addMethod(std::make_shared<BuiltInEciiSet>());
	builtInNNumber->addMethod(std::make_shared<BuiltInNNumberFromBitSequence>());
	builtInNNumber->addMethod(std::make_shared<BuiltInNNumberFromEcii>());
	builtInNNumber->addMethod(std::make_shared<BuiltInNNumberInc>());
	builtInNNumber->addMethod(std::make_shared<BuiltInNNumberDec>());
	builtInZNumber->addMethod(std::make_shared<BuiltInZNumberFromEcii>());
	builtInZNumber->addMethod(std::make_shared<BuiltInZNumberInc>());
	builtInZNumber->addMethod(std::make_shared<BuiltInZNumberDec>());
	builtInQNumber->addMethod(std::make_shared<BuiltInQNumberFromEcii>());
	builtInQNumber->addMethod(std::make_shared<BuiltInQNumberFromZNumbers>());
	builtInQNumber->addMethod(std::make_shared<BuiltInQNumberInc>());
	builtInQNumber->addMethod(std::make_shared<BuiltInQNumberDec>());
	builtInNatUInt->addMethod(std::make_shared<BuiltInNatUIntFromBitSequence>());
	builtInNatUInt->addMethod(std::make_shared<BuiltInNatUIntInc>());
	builtInNatUInt->addMethod(std::make_shared<BuiltInNatUIntDec>());
	builtInNatInt->addMethod(std::make_shared<BuiltInNatIntInc>());
	builtInNatInt->addMethod(std::make_shared<BuiltInNatIntDec>());
	builtInCanvas->addMethod(std::make_shared<BuiltInCanvasConstructor>());
	builtInCanvas->addMethod(std::make_shared<BuiltInCanvasSetPixel>());
}

inline const std::vector<std::shared_ptr<Class>> builtInClasses = {
	builtInBitSequence,
	builtInBoolean,
	builtInNNumber,
	builtInZNumber,
	builtInQNumber,
	builtInEcii,
	builtInPointer,
	builtInNatUInt,
	builtInNatInt,
	builtInNatReal,
	builtInCanvas,
};

inline const std::vector<std::shared_ptr<Function>> builtInFunctions = {
	builtInEqualsBooleans,
	builtInNotBoolean,
	builtInOrBoolean,
	builtInAndBoolean,
	builtInPrintBoolean,
	builtInInputBoolean,
	
	builtInEqualsBitSequence,
	builtInNotBitSequence,
	builtInConcatenateBitSequence,
	builtInPrintBitsequence,
	builtInInputBitSequence,
	
	builtInEqualsEcii,
	builtInConcatenateEcii,
	builtInPrintEcii,
	builtInInputEcii,
	
	builtInEqualsNNumber,
	builtInLessThanNNumber,
	builtInGreaterThanNNumber,
	builtInLessOrEqualNNumber,
	builtInGreaterOrEqualNNumber,
	builtInAddNNumber,
	builtInSubNNumber,
	builtInMulNNumber,
	builtInDivNNumber,
	builtInModNNumber,
	builtInPrintNNumber,
	builtInInputNNumber,
	
	builtInEqualsZNumber,
	builtInLessThanZNumber,
	builtInGreaterThanZNumber,
	builtInLessOrEqualZNumber,
	builtInGreaterOrEqualZNumber,
	builtInOppZNumber,
	builtInAddZNumber,
	builtInSubZNumber,
	builtInMulZNumber,
	builtInDivZNumber,
	builtInModZNumber,
	builtInPrintZNumber,
	builtInInputZNumber,
	
	builtInEqualsQNumber,
	builtInLessThanQNumber,
	builtInGreaterThanQNumber,
	builtInLessOrEqualQNumber,
	builtInGreaterOrEqualQNumber,
	builtInOppQNumber,
	builtInAddQNumber,
	builtInSubQNumber,
	builtInMulQNumber,
	builtInDivQNumber,
	builtInPrintQNumber,
	builtInInputQNumber,
	
	builtInEqualsPointers,
	
	builtInEqualsNatUInt,
	builtInLessThanNatUInt,
	builtInGreaterThanNatUInt,
	builtInLessOrEqualNatUInt,
	builtInGreaterOrEqualNatUInt,
	builtInAddNatUInt,
	builtInSubNatUInt,
	builtInMulNatUInt,
	builtInDivNatUInt,
	builtInModNatUInt,
	builtInRandNatUInt,
	builtInPrintNatUInt,
	builtInInputNatUInt,
	
	builtInEqualsNatInt,
	builtInLessThanNatInt,
	builtInGreaterThanNatInt,
	builtInLessOrEqualNatInt,
	builtInGreaterOrEqualNatInt,
	builtInOppNatInt,
	builtInAddNatInt,
	builtInSubNatInt,
	builtInMulNatInt,
	builtInDivNatInt,
	builtInModNatInt,
	builtInPrintNatInt,
	builtInInputNatInt,
	
	builtInEqualsNatReal,
	builtInLessThanNatReal,
	builtInGreaterThanNatReal,
	builtInLessOrEqualNatReal,
	builtInGreaterOrEqualNatReal,
	builtInAddNatReal,
	builtInSubNatReal,
	builtInMulNatReal,
	builtInDivNatReal,
	builtInRandNatReal,
	builtInPrintNatReal,
	builtInInputNatReal,
	
	builtInLoadFile,
	builtInSaveFile
};

inline bool isBuiltIn(const std::shared_ptr<Class> &c0) {
	if (c0 == nullptr) return false;
	for (const auto &c : builtInClasses) {if (c0 == c) return true;}
	return false;
}

inline bool isBuiltInClass(Ecii::String name) {
	for (const auto &c : builtInClasses) {if (c->getName() == name) return true;}
	return false;
}

inline std::shared_ptr<Class> getBuiltInClass(Ecii::String name) {
	for (const auto &c : builtInClasses) {if (c->getName() == name) return c;}
	return nullptr;
}

inline bool isBuiltIn(const std::shared_ptr<Function> &f0) {
	if (f0 == nullptr) return false;
	for (const auto &f : builtInFunctions) {if (f0 == f) return true;}
	return false;
}

inline bool isBuiltInFunction(const Ecii::String &functionName, const Type& inputClasses) {
	for (const auto &f : builtInFunctions) {
		if (f->isMatching(functionName, inputClasses)) return true;
	}
	return false;
}

inline std::shared_ptr<Function> getBuiltInFunction(const Ecii::String &functionName, const Type& inputClasses) {
	for (const auto &f : builtInFunctions) {if (f->isMatching(functionName, inputClasses)) return f;}
	return nullptr;
}

#endif