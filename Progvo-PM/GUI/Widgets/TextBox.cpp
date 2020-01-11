// (c) 2019 Pttn (https://Progvo.dev)

#include "Widgets.hpp"

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	sf::RectangleShape outline(sf::Vector2f(_size.x, _size.y));
	outline.setFillColor(sf::Color(255, 255, 255, 0));
	outline.setOutlineThickness(1 + active());
	
	sf::RectangleShape cursor(sf::Vector2f(1, _charSizes.y));
	cursor.setPosition(sf::Vector2f(_charSizes.x*(_curPos.column - _firstVisible.column) + _padding, (_curPos.line - _firstVisible.line)*_charSizes.y + _padding/2));
	for (std::size_t j(_firstVisible.line) ; j < std::min((std::size_t) (_firstVisible.line + _visibleLines() + 1), _lines.lineCount()) ; j++) {
		const std::vector<std::byte> chars(_lines.getLine(j));
		for (std::size_t i(_firstVisible.column) ; i < std::min((std::size_t) (_firstVisible.column + _visibleCols()), chars.size()) ; i++) {
			std::size_t i2(i - _firstVisible.column);
			sf::Sprite s(_font.getSprite((uint8_t) chars[i]));
			s.setPosition(sf::Vector2f(_charSizes.x*i2 + _padding, _padding + (j - _firstVisible.line)*_charSizes.y));
			target.draw(s, states);
		}
		
		float selStartLine(std::min(_selStart.line, _selEnd.line)), selEndLine(std::max(_selStart.line, _selEnd.line)),
		       selStartCol(_selStart.line < _selEnd.line ? _selStart.column : _selEnd.column), selEndCol(_selStart.line < _selEnd.line ? _selEnd.column : _selStart.column);
		
		if (j >= selStartLine && j <= selEndLine) {
			sf::RectangleShape highlight;
			highlight.setFillColor(sf::Color(255, 255, 255, 128));
			if (selStartLine == selEndLine) {
				highlight.setSize(sf::Vector2f(std::abs((float) _charSizes.x*(selEndCol - selStartCol)), _charSizes.y));
				highlight.setPosition(sf::Vector2f(_charSizes.x*((float) std::min(selStartCol, selEndCol) - (float) _firstVisible.column) + _padding, ((float) j - (float) _firstVisible.line)*_charSizes.y + _padding));
				if (highlight.getPosition().x < 0) {
					highlight.setPosition(sf::Vector2f(0., 0.));
					highlight.setSize(sf::Vector2f(cursor.getPosition().x, _size.y));
				}
				else if (highlight.getPosition().x + highlight.getSize().x > _size.x) {
					highlight.setSize(sf::Vector2f(_size.x - highlight.getPosition().x, _size.y));
				}
			}
			else if (j == selStartLine) {
				highlight.setSize(sf::Vector2f((float) _charSizes.x*((float) std::min(_lines.getLine(j).size(), _firstVisible.column + _visibleCols()) - selStartCol), _charSizes.y));
				highlight.setPosition(sf::Vector2f(_charSizes.x*(selStartCol - (float) _firstVisible.column) + _padding, ((float) j - (float) _firstVisible.line)*_charSizes.y + _padding));
			}
			else if (j == selEndLine) {
				highlight.setSize(sf::Vector2f((float) _charSizes.x*(selEndCol - (float) _firstVisible.column), _charSizes.y));
				highlight.setPosition(sf::Vector2f(_padding, ((float) j - (float) _firstVisible.line)*_charSizes.y + _padding));
			}
			else {
				highlight.setSize(sf::Vector2f((float) _charSizes.x*((float) std::min(_lines.getLine(j).size() - _firstVisible.column, _visibleCols()) - (float) _firstVisible.column), _charSizes.y));
				highlight.setPosition(sf::Vector2f(_padding, ((float) j - (float) _firstVisible.line)*_charSizes.y + _padding));
			}
			target.draw(highlight, states);
		}
	}
	
	target.draw(outline, states);
	target.draw(cursor, states);
}

void TextBox::_setFirstVisibleLine(std::size_t pos) {
	if (_visibleLines() > _lines.lineCount()) _firstVisible.line = 0;
	else if (pos > _lines.lineCount() - _visibleLines()) _firstVisible.line = _lines.lineCount() - _visibleLines();
	else _firstVisible.line = pos;
}
void TextBox::_setFirstVisibleCol(std::size_t pos) {
	if (_visibleCols() > _lines.getLine(_curPos.line).size()) _firstVisible.column = 0;
	else if (pos > _lines.getLine(_curPos.line).size() - _visibleCols()) _firstVisible.column = _lines.getLine(_curPos.line).size() - _visibleCols();
	else _firstVisible.column = pos;
}
void TextBox::_setCurPos(std::size_t line, std::size_t col, bool resetSel) {
	if (line >= _lines.lineCount()) {
		if (_lines.lineCount() == 0) _curPos.line = 0;
		else _curPos.line = _lines.lineCount() - 1;
	}
	else _curPos.line = line;
	
	if (col > _lines.getLine(_curPos.line).size()) _curPos.column = _lines.getLine(_curPos.line).size();
	else _curPos.column = col;
	
	if (_curPos.column < _firstVisible.column) _firstVisible.column = _curPos.column;
	else if (_curPos.column > _firstVisible.column + _visibleCols())
		_firstVisible.column = _curPos.column - _visibleCols();
	
	if (_curPos.line < _firstVisible.line) _firstVisible.line = _curPos.line;
	else if (_curPos.line > _firstVisible.line + _visibleLines())
		_firstVisible.line = _curPos.line - _visibleLines();
	
	if (resetSel) {
		_selStart = _curPos;
		_selEnd   = _curPos;
	}
}
void TextBox::_decCurPos() {
	if (_curPos.column != 0) _setCurPos(_curPos.line, _curPos.column - 1);
	else {
		if (_curPos.line != 0) _setCurPos(_curPos.line - 1, _lines.getLine(_curPos.line - 1).size());
	}
}
void TextBox::_incCurPos() {
	if (_curPos.column < _lines.getLine(_curPos.line).size()) _setCurPos(_curPos.line, _curPos.column + 1);
	else {
		if (_curPos.line + 1 < _lines.lineCount()) _setCurPos(_curPos.line + 1, 0);
	}
}
void TextBox::_decLine() {
	if (_curPos.line != 0) _setCurPos(_curPos.line - 1, _curPos.column, true);
	else _setCurPos(0, 0, true);
}
void TextBox::_incLine() {
	if (_curPos.line + 1 < _lines.lineCount()) _setCurPos(_curPos.line + 1, _curPos.column, true);
	else _setCurPos(_lines.lineCount() - 1, _lines.getLine(_lines.lineCount() - 1).size(), true);
}
void TextBox::_put(std::byte c) {
	if (_selStart != _selEnd) _erase();
	_lines.put(c, _curPos);
	_incCurPos();
}
void TextBox::_erase() {
	if (_selStart.line == _selEnd.line && _selStart.column == _selEnd.column) {
		uint64_t newCol(_lines.getLine(_curPos.line - 1).size());
		_lines.erase(_curPos);
		if (_curPos.column != 0)
			_curPos.column--;
		else if (_curPos.line != 0) {
			_curPos.column = newCol;
			_curPos.line--;
		}
	}
	else _del();
}
void TextBox::_del() {
	TextPos selStart(std::min(_selStart, _selEnd)), selEnd(std::max(_selStart, _selEnd));
	if (_selStart.line != _selEnd.line) {
		uint64_t firstLineToEraseSize(_lines.getLine(selStart.line).size());
		for (uint64_t i(0) ; i < selEnd.column ; i++)
			_lines.erase(TextPos(selEnd.line, 1));
		for (uint64_t i(selStart.column) ; i < firstLineToEraseSize ; i++)
			_lines.erase(TextPos(selStart.line, selStart.column + 1));
		if (_lines.getLine(selEnd.line).size() == 0) _lines.eraseLine(selEnd.line);
		for (uint64_t j(selStart.line + 1) ; j < selEnd.line ; j++)
			_lines.eraseLine(selStart.line + 1);
		_lines.erase(TextPos(selStart.line + 1, 0));
		_setCurPos(selStart.line, selStart.column, true);
	}
	else if (_selStart.column != _selEnd.column) {
		for (uint64_t i(selStart.column) ; i < selEnd.column ; i++)
			_lines.erase(TextPos(selStart.line, selStart.column + 1));
		_setCurPos(selStart.line, selStart.column, true);
	}
	else {
		_incCurPos();
		_erase();
	}
}

TextBox::TextBox(Vect2<float> dimensions, Vect2<float> position, Font font) {
	setPosition(sf::Vector2f(position.x, position.y));
	_size = dimensions;
	_font = font;
	_charSizes = _font.charSizes();
	_padding = 2.;
	_lines = Ecii::String();
	_curPos = TextPos(0, 0);
	_firstVisible = TextPos(0, 0);
	_selStart  = TextPos(0, 0);
	_selEnd   = TextPos(0, 0);
	_selecting = false;
}

void TextBox::action(const sf::Event &event) {
	Widget::action(event);
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			Vect2<float> clickPos(event.mouseButton.x, event.mouseButton.y);
			if (_contains(clickPos)) {
				_setCurPos(_firstVisible.line   + _visibleLines()*(clickPos.y - getPosition().y)/_size.y,
				           _firstVisible.column + _visibleCols()*(clickPos.x - getPosition().x)/_size.x);
				_selecting = true;
			}
			_selStart = _curPos;
			_selEnd   = _curPos;
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (_selecting) {
			Vect2<float> clickPos(event.mouseMove.x, event.mouseMove.y);
			uint64_t posX(0), posY(0);
			if (_containsX(clickPos.x))
				posX = _firstVisible.column + _visibleCols()*(clickPos.x - getPosition().x)/_size.x;
			else if (clickPos.x < getPosition().x) {
				if (_firstVisible.column > 0) _setFirstVisibleCol(_firstVisible.column - 1);
				posX = _firstVisible.column;
			}
			else {
				_setFirstVisibleCol(_firstVisible.column + 1);
				posX = _firstVisible.column + _visibleCols();
			}
			
			if (_containsY(clickPos.y))
				posY = _firstVisible.line + _visibleLines()*(clickPos.y - getPosition().y)/_size.y;
			else if (clickPos.y < getPosition().y) {
				if (_firstVisible.line > 0) _setFirstVisibleLine(_firstVisible.line - 1);
				posY = _firstVisible.line;
			}
			else {
				_setFirstVisibleLine(_firstVisible.line + 1);
				posY = _firstVisible.line + _visibleLines();
			}
			_setCurPos(posY, posX);
			_selEnd = _curPos;
		}
	}
	else if (event.type == sf::Event::MouseWheelScrolled) {
		if (_contains(Vect2<float>(event.mouseWheelScroll.x, event.mouseWheelScroll.y))) {
			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
				if (event.mouseWheelScroll.delta > 0) {
					if (_firstVisible.line != 0) _setFirstVisibleLine(_firstVisible.line - 1);
				}
				else if (event.mouseWheelScroll.delta < 0) {
					_setFirstVisibleLine(_firstVisible.line + 1);
				}
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			_selecting = false;
		}
	}
	else if (active()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) _decCurPos();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) _incCurPos();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) _incLine();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) _decLine();
		
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode == 8) _erase();
			else if (event.text.unicode == 127) _del();
			else {
				if (event.text.unicode <= 255)
					_put(Ecii::charToEcii((char) event.text.unicode, sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)));
				else _put(std::byte{252});
			}
		}
	}
}
	
void TextBox::setText(const Ecii::String &txt) {
	_lines = txt;
	if (_lines.lineCount() == 0) _curPos.line = 0;
	else _curPos.line = _lines.lineCount() - 1;
	_curPos.column = _lines.getLine(_lines.size() - 1).size();
	_selStart = _curPos;
	_selEnd   = _curPos;
	_setFirstVisibleCol(0);
	_setFirstVisibleLine(_curPos.line);
}