// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Interpreter_hpp
#define HEADER_Interpreter_hpp

#include <thread>
#include "../Parser/Parser.hpp"

#define CALLSTACKLIMIT	1024

class Console;

class Interpreter {
	enum ExitType {NONE, Stop, Return, Break};
	Parser _parser;
	std::shared_ptr<Function> _entryPoint;
	std::vector<std::vector<std::shared_ptr<Data>>> _objects;
	std::thread _executionThread;
	bool _running, _stop;
	
	void _start();
	void _addData(const std::shared_ptr<Data>&);
	ExitType _execute(const Instruction&);
	std::shared_ptr<Data> _call(const std::shared_ptr<Function>&, const std::shared_ptr<Data>&, const std::shared_ptr<Data>&);
	std::shared_ptr<Data> _getObject(const Vect2<uintmax_t>&);
	std::shared_ptr<Data> _eval(const Expression&);
	void _assign(const std::shared_ptr<Data>&, std::shared_ptr<Data>&);
	void _assign(const std::vector<ObjectLocation>&, const std::shared_ptr<Data>&);
public:
	Interpreter();
	void reset();
	void run(const Ecii::String&);
	void stop() {if (_running) _stop = true;}
	bool isStopping() const {return _stop;}
	bool isRunning() const {return _running;}
};

#endif