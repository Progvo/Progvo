// (c) 2019 Pttn (https://Progvo.dev)

#ifndef HEADER_Widget_hpp
#define HEADER_Widget_hpp

#include "../Font.hpp"
#include <memory>

class Widget : public sf::Drawable, public sf::Transformable, public std::enable_shared_from_this<Widget> {
	bool _active, _clicked, _unclicked;
protected:
	Vect2<float> _size;
	std::shared_ptr<Widget> _superWidget;
	std::vector<std::shared_ptr<Widget>> _subWidgets;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (uint16_t i(0) ; i < _subWidgets.size() ; i++) _subWidgets[i]->draw(target, states);
	}
	
	bool _containsX(float x) const {return (x >= getPosition().x && x <= getPosition().x + _size.x);}
	bool _containsY(float y) const {return (y >= getPosition().y && y <= getPosition().y + _size.y);}
	bool _contains(Vect2<float> pos) const {return _containsX(pos.x) && _containsY(pos.y);}
public:
	Widget() {
		_size = Vect2<float>(1, 1);
		_active = false;
		_clicked = false;
		_unclicked = false;
	}
	bool active() const {return _active;}
	bool clicked() const {return _clicked;}
	bool unclicked() const {return _unclicked;}
	virtual void action(const sf::Event &event) {
		_unclicked = false;
		for (uint16_t i(0) ; i < _subWidgets.size() ; i++) _subWidgets[i]->action(event);
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				Vect2<float> clickPos(event.mouseButton.x, event.mouseButton.y);
				if (_contains(clickPos)) {
					_active = true;
					_clicked = true;
				}
				else _active = false;
			}
		}
		if (event.type == sf::Event::MouseButtonReleased) {
			Vect2<float> clickPos(event.mouseButton.x, event.mouseButton.y);
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (_contains(clickPos) && _clicked) _unclicked = true;
				_clicked = false;
			}
		}
	}
	void setSize(const Vect2<float> &size) {
		setScale(size.x/_size.x, size.x/_size.x);
		_size = size;
		if (_size.x < 1) _size.x = 1;
		if (_size.y < 1) _size.y = 1;
	}
	void setParent(std::shared_ptr<Widget> w) {
		_superWidget = w;
		w->_subWidgets.push_back(shared_from_this());
	}
};

class Button : public Widget {
	Ecii::String _text;
	Font _font;
	Vect2<uint16_t> _charSizes;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Button(const Vect2<float> &dimensions, const Vect2<float> &position, const Ecii::String &text, const Font &font) {
		_size = dimensions;
		setPosition(sf::Vector2f(position.x, position.y));
		_text = text;
		_font = font;
		_charSizes = _font.charSizes();
	}
	
	virtual void action(const sf::Event &event) {Widget::action(event);}
	void setText(Ecii::String txt) {_text = txt;}
};

class ListBox : public Widget {
	std::vector<Ecii::String> _lines;
	Font _font;
	Vect2<uint16_t> _charSizes;
	std::size_t _selected, _firstVisibleLine;
	
	size_t _visibleLines() const {return _size.y/_charSizes.y;}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	ListBox(Vect2<float> dimensions, Vect2<float> position, Font font) {
		_size = dimensions;
		setPosition(sf::Vector2f(position.x, position.y));
		_firstVisibleLine = 0;
		_font = font;
		_charSizes = _font.charSizes();
		_selected = -1;
	}
	
	void add(const Ecii::String &e) {_lines.push_back(e);}
	void clear() {
		_lines = std::vector<Ecii::String>();
		_firstVisibleLine = 0;
		_selected = -1;
	}
	Ecii::String selected() const {
		if (_selected < _lines.size()) return _lines[_selected];
		else return Ecii::String();
	}
	
	virtual void action(const sf::Event &event);
};

class TextBoxInline : public Widget {
	Ecii::String _text;
	float _padding, _padding2;
	std::size_t _pos, _firstVisibleChar, _selStart, _selEnd;
	Font _font;
	Vect2<uint16_t> _charSizes;
	bool _selecting;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::size_t _visibleChars() const {return (_size.x - _padding2)/_charSizes.x;}
	void _setFirstVisibleChar(uint64_t pos);
	void _setCurPos(uint64_t, bool = false);
	void _decCurPos() {if (_pos != 0) _setCurPos(_pos - 1);}
	void _incCurPos() {_setCurPos(_pos + 1);}
	void _put(std::byte c);
	void _erase();
	void _del();
public:
	TextBoxInline(Vect2<float>, Vect2<float>, Font);
	void action(const sf::Event &event);
	Ecii::String getText() const {return _text;}
	void setText(Ecii::String);
};

class TextBox : public Widget {
	Ecii::String _lines;
	float _padding;
	TextPos _curPos, _firstVisible, _selStart, _selEnd;
	Font _font;
	Vect2<uint16_t> _charSizes;
	bool _selecting;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::size_t _visibleLines() const {return (_size.y - _padding)/_charSizes.y;}
	std::size_t _visibleCols() const {return (_size.x - _padding)/_charSizes.x;}
	void _setFirstVisibleLine(std::size_t pos);
	void _setFirstVisibleCol(std::size_t pos);
	void _setCurPos(std::size_t, std::size_t, bool = false);
	void _decCurPos();
	void _incCurPos();
	void _decLine();
	void _incLine();
	void _put(std::byte c);
	void _erase();
	void _del();
public:
	TextBox(Vect2<float>, Vect2<float>, Font);
	void action(const sf::Event &event);
	Ecii::String getText() const {return _lines;}
	void setText(const Ecii::String &txt);
};

#endif