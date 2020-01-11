// (c) 2019-2020 Pttn (https://Progvo.dev)

#include "GUI/IDE.hpp"

int main(int /*argc*/, char** argv) {
	_addBuiltInMethods();
	IDE::init(argv[0]);
	IDE::get()->start();
	return 0;
}