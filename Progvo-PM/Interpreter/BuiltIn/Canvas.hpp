// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Canvas_hpp
#define HEADER_Canvas_hpp

#include <SFML/Graphics.hpp>
#include "../../Tools.hpp"

class Canvas {
	sf::Image _canvas;
	Vect2<uintmax_t> _size;
	bool _close;
public:
	Canvas(uintmax_t w, uintmax_t h) {
		if (w < 1 || w > 65535) w = 1;
		if (h < 1 || h > 65535) h = 1;
		_size = Vect2<uintmax_t>(w, h);
		_canvas.create(_size.x, _size.y);
		_close = false;
	}
	
	void close() {_close = true;}
	
	void setPixel(uint8_t r, uint8_t g, uint8_t b, Vect2<uintmax_t> pos) {
		if (pos.x + _size.x*pos.y < _size.x*_size.y)
			_canvas.setPixel(pos.x, pos.y, sf::Color(r, g, b, 255));
	}
	
	void run() {
		sf::RenderWindow canvasWindow(sf::VideoMode(_size.x, _size.y), "Kanvaso", sf::Style::Close);
		canvasWindow.setFramerateLimit(120);
		while (canvasWindow.isOpen()) {
			if (_close) canvasWindow.close();
			else {
				sf::Event event;
				while (canvasWindow.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						canvasWindow.setVisible(false);
				}
				canvasWindow.clear();
				sf::Texture texture;
				texture.loadFromImage(_canvas);
				sf::Sprite sprite;
				sprite.setTexture(texture, true);
				canvasWindow.draw(sprite);
				canvasWindow.display();
			}
		}
	}
};

#endif