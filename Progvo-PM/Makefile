CXX    = g++
CFLAGS = -Wall -Wextra -ansi -pedantic -std=c++17

msys_version := $(if $(findstring Msys, $(shell uname -o)),$(word 1, $(subst ., ,$(shell uname -r))),0)
ifneq ($(msys_version), 0)
	LIBS   = -pthread -lstdc++fs -lsfml-graphics -lsfml-window -lsfml-system
else
	LIBS   = -pthread -lstdc++fs -lsfml-graphics -lsfml-window -lsfml-system -lX11
endif

all: Progvo-PM

debug: CFLAGS += -g
debug: Progvo-PM

static: CFLAGS += -D SFML_STATIC
static: LIBS = -static -pthread -lstdc++fs -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
static: Progvo-PM

Progvo-PM: main.o Widgets.o TextBoxInline.o TextBox.o Ecii.o Interpreter.o Parser.o Tokenizer.o Environment.o QNumber.o ZNumber.o NNumber.o BitSequence.o Tools.o Ecii.o
	$(CXX) $(CFLAGS) -o bin/Progvo-PM $^ $(LIBS)

main.o: main.cpp GUI/Widgets/Widgets.hpp
	$(CXX) $(CFLAGS) -c -o main.o main.cpp

Widgets.o: GUI/Widgets/Widgets.cpp
	$(CXX) $(CFLAGS) -c -o Widgets.o GUI/Widgets/Widgets.cpp

TextBoxInline.o: GUI/Widgets/TextBoxInline.cpp
	$(CXX) $(CFLAGS) -c -o TextBoxInline.o GUI/Widgets/TextBoxInline.cpp

TextBox.o: GUI/Widgets/TextBox.cpp
	$(CXX) $(CFLAGS) -c -o TextBox.o GUI/Widgets/TextBox.cpp

Interpreter.o: Interpreter/Interpreter.cpp
	$(CXX) $(CFLAGS) -c -o Interpreter.o Interpreter/Interpreter.cpp
	
Parser.o: Parser/Parser.cpp
	$(CXX) $(CFLAGS) -c -o Parser.o Parser/Parser.cpp

Tokenizer.o: Parser/Tokenizer.cpp
	$(CXX) $(CFLAGS) -c -o Tokenizer.o Parser/Tokenizer.cpp

Environment.o: Parser/Environment.cpp
	$(CXX) $(CFLAGS) -c -o Environment.o Parser/Environment.cpp

QNumber.o: Interpreter/BuiltIn/QNumber.cpp
	$(CXX) $(CFLAGS) -c -o QNumber.o Interpreter/BuiltIn/QNumber.cpp

ZNumber.o: Interpreter/BuiltIn/ZNumber.cpp
	$(CXX) $(CFLAGS) -c -o ZNumber.o Interpreter/BuiltIn/ZNumber.cpp

NNumber.o: Interpreter/BuiltIn/NNumber.cpp
	$(CXX) $(CFLAGS) -c -o NNumber.o Interpreter/BuiltIn/NNumber.cpp

BitSequence.o: Interpreter/BitSequence.cpp
	$(CXX) $(CFLAGS) -c -o BitSequence.o Interpreter/BitSequence.cpp

Tools.o: Tools.cpp
	$(CXX) $(CFLAGS) -c -o Tools.o Tools.cpp

Ecii.o: Ecii.cpp
	$(CXX) $(CFLAGS) -c -o Ecii.o Ecii.cpp

clean:
	rm -rf bin/Progvo-PM *.o