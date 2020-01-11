// (c) 2019 Pttn (https://Progvo.dev)

#include "Widgets.hpp"

void TextBoxInline::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	std::vector<std::byte> chars(_text.vb());
	states.transform *= getTransform();
	sf::RectangleShape outline(sf::Vector2f(_size.x, _size.y));
	outline.setFillColor(sf::Color(255, 255, 255, 0));
	outline.setOutlineThickness(1 + active());
	for (std::size_t i(_firstVisibleChar) ; i < std::min((std::size_t) (_firstVisibleChar + _visibleChars()), _text.size()) ; i++) {
		std::size_t i2(i - _firstVisibleChar);
		sf::Sprite s(_font.getSprite((uint8_t) chars[i]));
		s.setPosition(sf::Vector2f(_charSizes.x*i2 + _padding2, _padding));
		target.draw(s, states);
	}
	sf::RectangleShape cursor(sf::Vector2f(1, _charSizes.y + _padding));
	cursor.setPosition(sf::Vector2f(_charSizes.x*(_pos - _firstVisibleChar) + _padding2, _padding/2));
	
	float selStart(std::min(_selStart, _selEnd)), selEnd(std::max(_selStart, _selEnd));
	sf::RectangleShape highlight(sf::Vector2f((float) _charSizes.x*(selEnd - selStart), _charSizes.y));
	highlight.setPosition(sf::Vector2f(_charSizes.x*(selStart - (float) _firstVisibleChar) + _padding2, _padding));
	if (highlight.getPosition().x < 0) {
		highlight.setPosition(sf::Vector2f(0., 0.));
		highlight.setSize(sf::Vector2f(cursor.getPosition().x, _charSizes.y));
	}
	else if (highlight.getPosition().x + highlight.getSize().x > _size.x)
		highlight.setSize(sf::Vector2f(_size.x - highlight.getPosition().x, _charSizes.y));
	highlight.setFillColor(sf::Color(255, 255, 255, 128));
	target.draw(outline, states);
	target.draw(highlight, states);
	target.draw(cursor, states);
}

void TextBoxInline::_setFirstVisibleChar(uint64_t pos) {
	if (_visibleChars() > _text.size()) _firstVisibleChar = 0;
	else if (pos > _text.size() - _visibleChars()) _firstVisibleChar = _text.size() - _visibleChars();
	else _firstVisibleChar = pos;
}

void TextBoxInline::_setCurPos(uint64_t pos, bool resetSel) {
	if (pos > _text.size()) _pos = _text.size();
	else _pos = pos;
	
	if (_pos < _firstVisibleChar) _firstVisibleChar = _pos;
	else if (_pos > _firstVisibleChar + _visibleChars())
		_firstVisibleChar = _pos - _visibleChars();
	
	if (resetSel) {
		_selStart = _pos;
		_selEnd = _pos;
	}
}

void TextBoxInline::_put(std::byte c) {
	if (_selStart != _selEnd) _erase();
	_text.put(c, _pos);
	_incCurPos();
}

void TextBoxInline::_erase() {
	if (_selStart == _selEnd) {
		if (_pos > 0) {
			_text.erase(_pos);
			_setCurPos(_pos - 1);
		}
	}
	else _del();
}
void TextBoxInline::_del() {
	if (_selStart != _selEnd) {
		for (uint16_t i(std::min(_selStart, _selEnd)) ; i < std::max(_selStart, _selEnd) ; i++)
			_text.erase(std::min(_selStart, _selEnd) + 1);
		_setCurPos(std::min(_selStart, _selEnd));
		_selStart = _pos;
		_selEnd = _pos;
	}
	else {
		_pos++;
		_erase();
	}
}

TextBoxInline::TextBoxInline(Vect2<float> dimensions, Vect2<float> position, Font font) {
	setPosition(sf::Vector2f(position.x, position.y));
	_size = dimensions;
	_font = font;
	_charSizes = _font.charSizes();
	_padding = (_size.y - _charSizes.y)/2;
	_padding2 = std::min(_padding, 4.f);
	_text = Ecii::String();
	_pos = _text.size();
	_firstVisibleChar = 0;
	_selStart = 0;
	_selEnd = 0;
	_selecting = false;
}

void TextBoxInline::action(const sf::Event &event) {
	Widget::action(event);
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			Vect2<float> clickPos(event.mouseButton.x, event.mouseButton.y);
			if (_contains(clickPos)) {
				_setCurPos(_firstVisibleChar + _visibleChars()*(clickPos.x - getPosition().x)/_size.x, true);
				_selecting = true;
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (_selecting) {
			float x(event.mouseMove.x);
			if (_containsX(x)) {
				_setCurPos(_firstVisibleChar + _visibleChars()*(x - getPosition().x)/_size.x, false);
			}
			else if (x < getPosition().x) {
				if (_firstVisibleChar > 0) _setFirstVisibleChar(_firstVisibleChar - 1);
				_setCurPos(_firstVisibleChar, false);
			}
			else {
				_setFirstVisibleChar(_firstVisibleChar + 1);
				_setCurPos(_firstVisibleChar + _visibleChars(), false);
			}
			_selEnd = _pos;
		}
	}
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) _selecting = false;
	}
	else if (active()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) _decCurPos();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) _incCurPos();
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode == 8) _erase();
			else if (event.text.unicode == 127) _del();
			else {
				if (event.text.unicode <= 255)
				_put(Ecii::charToEcii((uint8_t) event.text.unicode, sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)));
			}
		}
	}
}
	
void TextBoxInline::setText(Ecii::String txt) {
	_text = txt;
	_pos = _text.size();
	_selStart = _pos;
	_selEnd = _pos;
	_setFirstVisibleChar(_pos);
}