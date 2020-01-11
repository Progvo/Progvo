// (c) 2019 Pttn (https://Progvo.dev)

#include "Widgets.hpp"

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Widget::draw(target, states);
	std::vector<std::byte> chars(_text.vb());
	states.transform *= getTransform();
	sf::RectangleShape outline(sf::Vector2f(_size.x, _size.y));
	if (clicked()) outline.setFillColor(sf::Color(255, 255, 255, 128));
	else outline.setFillColor(sf::Color(255, 255, 255, 0));
	outline.setOutlineThickness(1 + active());
	Vect2<float> padding(0, 0);
	if (_size.x > _text.size()*_charSizes.x) padding.x = (_size.x - _text.size()*_charSizes.x)/2;
	if (_size.y > _charSizes.y) padding.y = (_size.y - _charSizes.y)/2;
	for (std::size_t i(0) ; i < std::min(_text.size(), (std::size_t) _size.x/_charSizes.x) ; i++) {
		sf::Sprite s(_font.getSprite((uint8_t) chars[i]));
		s.setPosition(sf::Vector2f(_charSizes.x*i + padding.x, padding.y));
		target.draw(s, states);
	}
	target.draw(outline, states);
}

void ListBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Widget::draw(target, states);
	states.transform *= getTransform();
	sf::RectangleShape outline(sf::Vector2f(_size.x, _size.y));
	outline.setFillColor(sf::Color(255, 255, 255, 0));
	outline.setOutlineThickness(1 + active());
	Vect2<float> padding(2, 2);
	sf::RectangleShape highlight(sf::Vector2f(_size.x, _charSizes.y));
	highlight.setFillColor(sf::Color(255, 255, 255, 128));
	highlight.setPosition(sf::Vector2f(0., (_selected - _firstVisibleLine)*_charSizes.y + padding.y));
	for (std::size_t j(_firstVisibleLine) ; j < std::min(_firstVisibleLine + _visibleLines(), _lines.size()) ; j++) {
		std::vector<std::byte> line(_lines[j].vb());
		for (std::size_t i(0) ; i < std::min(_lines[j].size(), (std::size_t) _size.x/_charSizes.x) ; i++) {
			sf::Sprite s(_font.getSprite((uint8_t) line[i]));
			s.setPosition(sf::Vector2f(_charSizes.x*i + padding.x, (j - _firstVisibleLine)*_charSizes.y + padding.y));
			target.draw(s, states);
		}
	}
	target.draw(outline, states);
	target.draw(highlight, states);
}

void ListBox::action(const sf::Event &event) {
	Widget::action(event);
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			Vect2<float> clickPos(event.mouseButton.x, event.mouseButton.y);
			if (_contains(clickPos)) {
				_selected = _firstVisibleLine + _visibleLines()*(clickPos.y - getPosition().y)/_size.y;;
				if (_selected >= _lines.size()) _selected = _lines.size() - 1;
			}
			else _selected = -1;
		}
	}
	else if (event.type == sf::Event::MouseWheelScrolled) {
		if (_contains(Vect2<float>(event.mouseWheelScroll.x, event.mouseWheelScroll.y))) {
			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
				if (event.mouseWheelScroll.delta > 0) {
					if (_firstVisibleLine != 0) _firstVisibleLine--;
				}
				else if (event.mouseWheelScroll.delta < 0) {
					if (_firstVisibleLine != _lines.size()) _firstVisibleLine++;
				}
			}
		}
	}
	else if (active()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			if (_selected != 0) _selected--;
			if (_selected < _firstVisibleLine) _firstVisibleLine--;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (_selected + 1 != _lines.size()) _selected++;
			if (_selected > _firstVisibleLine + _visibleLines()) _firstVisibleLine++;
		}
	}
}