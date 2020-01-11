// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_IDE_hpp
#define HEADER_IDE_hpp

#include <thread>
#include "Font.hpp"
#include "../Interpreter/Interpreter.hpp"
#include "Widgets/Widgets.hpp"
#include "../Interpreter/BuiltIn/BuiltIn.hpp"
#ifdef __linux__
	#define Font X11_Font // X11 is already using something named "Font" :| ...
	#include <X11/Xlib.h>
	#undef Font
#endif

#define PROGVOPM_VERSION "0.9-skizo"

class Console {
	Font _font;
	std::shared_ptr<Ecii::String> _text;
	Ecii::String _input;
	Vect2<std::size_t> _size;
	std::size_t _firstVisibleLine;
	bool _show, _close, _inputting;
public:
	Console(const Font &font) :
		_font(font),
		_text(builtInStandardOutput),
		_size(Vect2<std::size_t>(80, 25)),
		_firstVisibleLine(0),
		_show(true),
		_close(false),
		_inputting(false) {
		builtInStandardInputFunction = [&](){return input();};
	}
	
	void show() {_show = true;}
	void hide() {_show = false;}
	void close() {_close = true;}

	Ecii::String input() {
		Ecii::String es;
		_inputting = true;
		while (_inputting) {std::this_thread::sleep_for(std::chrono::milliseconds(20));}
		es = _input;
		_input = "";
		return es;
	}
	
	void run() {
		sf::RenderWindow consoleWindow(sf::VideoMode(_font.charSizes().x*_size.x, _font.charSizes().y*_size.y), "Progvo konzolo", sf::Style::Close);
		consoleWindow.setFramerateLimit(60);
		while (consoleWindow.isOpen()) {
			if (_close) consoleWindow.close();
			else {
				if (_show) consoleWindow.setVisible(true);
				sf::Event event;
				while (consoleWindow.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						_show = false;
						consoleWindow.setVisible(false);
					}
					else if (event.type == sf::Event::MouseWheelScrolled) {
						if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
							if (event.mouseWheelScroll.delta > 0) {
								if (_firstVisibleLine != 0) _firstVisibleLine--;
							}
							else if (event.mouseWheelScroll.delta < 0) {
								_firstVisibleLine++;
							}
						}
					}
					if (_inputting) {
						if (event.type == sf::Event::KeyPressed) {
							if (event.key.code == sf::Keyboard::Enter)
								_inputting = false;
						}
						if (event.type == sf::Event::TextEntered && _inputting) {
							if (event.text.unicode == 8 && _input.size() != 0) {
								_input.erase(_input.size());
								_text->erase(_text->size());
							}
							else {
								if (event.text.unicode <= 255) {
									_input.add(Ecii::charToEcii((char) event.text.unicode, sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)));
									_text->add(Ecii::charToEcii((char) event.text.unicode, sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)));
								}
								else {
									_input.add(std::byte{252});
									_text->add(std::byte{252});
								}
							}
						}
					}
				}
				
				consoleWindow.clear();
				const std::vector<std::byte> chars(_text->vb());
				TextPos tp(0, 0);
				for (std::size_t i(0) ; i < chars.size() ; i++) {
					if (chars[i] == Ecii::EOL) {
						tp.column = 0;
						tp.line++;
					}
					else {
						if (tp.line >= _firstVisibleLine) {
							sf::Sprite s(_font.getSprite((uint8_t) chars[i]));
							s.setPosition(sf::Vector2f(tp.column*_font.charSizes().x, (tp.line - _firstVisibleLine)*_font.charSizes().y));
							consoleWindow.draw(s);
						}
						tp.column++;
						if (tp.column == _size.x) {
							tp.column = 0;
							tp.line++;
						}
					}
				}
				consoleWindow.display();
			}
		}
	}
};

class IDE {
	std::filesystem::path _curDir, _binDir, _incDir;
	std::shared_ptr<Console> _console;
	
	static std::shared_ptr<IDE> _ide;
	bool _mustRefresh;
	
	void _fillFileList(const std::filesystem::path& directory, const std::shared_ptr<ListBox> &list) {
		list->clear();
		std::vector<Ecii::String> folders, files;
		for (const auto &file : std::filesystem::directory_iterator(directory)) {
			if (file.is_directory()) folders.push_back(file.path().filename().string());
			else if (file.is_regular_file()) {
				if (file.path().extension() == ".Pvo")
					files.push_back(file.path().filename().string());
			}
		}
		std::sort(folders.begin(), folders.end());
		std::sort(files.begin(), files.end());
		list->add("..");
		for (const auto &folder : folders) list->add(folder + "/");
		for (const auto &file   : files)   list->add(file);
	}
	
public:
	IDE(const std::filesystem::path &binPath) {
		_curDir = std::filesystem::current_path();
		if (!std::filesystem::exists(binPath)) {
			BUG("Malvalida dosierindiko de la plenumebla dosiero");
			_binDir = _curDir;
		}
		else if (std::filesystem::is_directory(binPath)) _binDir = binPath;
		else _binDir = binPath.parent_path();
		_incDir = _binDir;
		_console = nullptr;
		_mustRefresh = true;
	}
	
	static void init(const std::filesystem::path &binPath) {_ide = std::make_shared<IDE>(binPath);}
	static std::shared_ptr<IDE> get() {return _ide;}
	std::shared_ptr<Console> getConsole() {return _console;}
	
	std::filesystem::path getCurDir() const {return _curDir;}
	void setCurDir(std::filesystem::path dir) {
		if (!std::filesystem::exists(dir)) BUG("Malvalida dosierindiko");
		else if (std::filesystem::is_directory(dir)) _curDir = dir;
		else _curDir = dir.parent_path();
	}
	std::filesystem::path getIncDir() const {return _incDir;}
	
	void start() {
#ifdef __linux__
		XInitThreads();
#endif
		uint64_t margin(4), rightPanelWidth(3*96 + 4*margin), rightPanelPosX(0);
		sf::RenderWindow window(sf::VideoMode(800, 600), "Progvo Programada Medio " + std::string(PROGVOPM_VERSION));
		window.setFramerateLimit(60);
		
		std::shared_ptr<Widget> widgetGroup(std::make_shared<Widget>());
		std::filesystem::path fontPath("EkioTiparo.png");
		Font font((_binDir/fontPath).string(), Vect2<uint16_t>(8, 16));
		_console = std::make_shared<Console>(font);
		
		if (!font.isLoaded()) {
			BUG("Ne eblas shargi la Ekio tiparon");
			std::exit(-1);
		}
		else if (font.getSpriteCount() != 256) {
			BUG("Malghusta nombro de kaheloj " << font.getSpriteCount() << ", devas esti 256");
			std::exit(-1);
		}
		
		std::shared_ptr<TextBox> textBox(std::make_shared<TextBox>(Vect2<float>(1, 1), Vect2<float>(margin, margin), font));
		textBox->setParent(widgetGroup);
		std::shared_ptr<ListBox> fileList(std::make_shared<ListBox>(Vect2<float>(1, 1), Vect2<float>(0, 0), font));
		fileList->setParent(widgetGroup);
		std::shared_ptr<TextBoxInline> pathField = std::make_shared<TextBoxInline>(Vect2<float>(rightPanelWidth - 2*margin, 24), Vect2<float>(1, 1), font);
		pathField->setParent(widgetGroup);
		std::shared_ptr<Button> loadButton(std::make_shared<Button>(Vect2<float>(96, 24), Vect2<float>(0, 0), Ecii::String(std::byte{70}) + "argi", font)); // Ŝargi
		loadButton->setParent(widgetGroup);
		std::shared_ptr<Button> saveButton(std::make_shared<Button>(Vect2<float>(96, 24), Vect2<float>(0, 0), "Konservi", font));
		saveButton->setParent(widgetGroup);
		std::shared_ptr<Button> executeButton(std::make_shared<Button>(Vect2<float>(96, 24), Vect2<float>(0, 0), "Plenumi", font));
		executeButton->setParent(widgetGroup);
		
		_fillFileList(_curDir, fileList);
		Interpreter interpreter;
		
		std::thread consoleThread(&Console::run, _console);
		
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					interpreter.reset();
					_console->close();
					consoleThread.join();
					window.close();
				}
				if (event.type == sf::Event::Resized) {
					if (window.getSize().x < 2*rightPanelWidth)
						window.setSize(sf::Vector2u(2*rightPanelWidth, window.getSize().y));
					sf::FloatRect visibleArea(0, 0, window.getSize().x, window.getSize().y);
					window.setView(sf::View(visibleArea));
					_mustRefresh = true;
				}
				widgetGroup->action(event);
				
				if (fileList->unclicked()) {
					std::string selectedPath(fileList->selected().str());
						if (std::filesystem::is_directory(_curDir/selectedPath)) {
							pathField->setText("");
							if (fileList->selected().str() == "..") _curDir = _curDir.parent_path();
							else _curDir /= selectedPath;
							_fillFileList(_curDir, fileList);
						}
						else
							pathField->setText(fileList->selected());
				}
				
				if (saveButton->unclicked()) {
					save(textBox->getText().vb(), (_curDir/pathField->getText().str()).string());
					_fillFileList(_curDir, fileList);
				}
				if (loadButton->unclicked())
					textBox->setText(load((_curDir/std::filesystem::path(pathField->getText().str())).string()));
				if (executeButton->unclicked()) {
					save(textBox->getText().vb(), (_curDir/pathField->getText().str()).string());
					if (!interpreter.isRunning()) {
						std::cout << "Plenumanta " << pathField->getText() << "..." << std::endl;
						interpreter.run(pathField->getText());
					}
					else if (!interpreter.isStopping()) {
						std::cout << "Haltanta de la programo..." << std::endl;
						interpreter.stop();
					}
				}
			}
			
			rightPanelPosX = window.getSize().x - rightPanelWidth;
			textBox->setSize(Vect2<float>(window.getSize().x - rightPanelWidth - margin, window.getSize().y - 2*margin));
			fileList->setSize(Vect2<float>(rightPanelWidth - 2*margin, window.getSize().y - 2*24 - 4*margin));
			fileList->setPosition(sf::Vector2f(rightPanelPosX + margin, margin));
			pathField->setPosition(sf::Vector2f(rightPanelPosX + margin, window.getSize().y - 2*24 - 2*margin));
			loadButton->setPosition(sf::Vector2f(rightPanelPosX + margin, window.getSize().y - 24 - margin));
			saveButton->setPosition(sf::Vector2f(rightPanelPosX + 96 + 2*margin, window.getSize().y - 24 - margin));
			executeButton->setPosition(sf::Vector2f(rightPanelPosX + 2*96 + 3*margin, window.getSize().y - 24 - margin));
			if (!interpreter.isRunning()) executeButton->setText("Plenumi");
			else if (interpreter.isStopping()) executeButton->setText("Haltanta...");
			else executeButton->setText("Halti");
			window.clear();
			window.draw(*widgetGroup);
			window.display();
		}
	}
};

inline std::shared_ptr<IDE> IDE::_ide = nullptr;

#endif