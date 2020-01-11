// (c) 2019 Pttn (https://Progvo.dev)

#ifndef HEADER_Font_hpp
#define HEADER_Font_hpp

#include <SFML/Graphics.hpp>
#include "../Tools.hpp"

class Font {
	bool _loaded;
	sf::Texture _tilesSrc;
	std::vector<sf::Sprite> _charSprites;
	sf::Sprite _dummy;
	Vect2<uint16_t> _charSizes;
public:
	Font() {
		_loaded = false;
		_charSprites = std::vector<sf::Sprite>();
		_charSizes = Vect2<uint16_t>(1, 1);
		_dummy.setTextureRect(sf::IntRect(0, 0, 1, 1));
	}
	Font(const std::string& tileset, Vect2<uint16_t> charSizes) {
		*this = Font();
		if (!_tilesSrc.loadFromFile(tileset))
			std::cerr << "Ne eblas shargi la kahelaron de tiparo :| ..." << std::endl;
		else {
			_charSizes = charSizes;
			uint16_t w(_tilesSrc.getSize().x/_charSizes.x), h(_tilesSrc.getSize().y/_charSizes.y);
			
			for (uint16_t j(0) ; j < h ; j++) {
				for (uint16_t i(0) ; i < w ; i++) {
					sf::Sprite s;
					s.setTexture(_tilesSrc);
					s.setTextureRect(sf::IntRect(i*_charSizes.x, j*_charSizes.y, _charSizes.x, _charSizes.y));
					_charSprites.push_back(s);
				}
			}
			if (_charSprites.size() > 0) _loaded = true;
		}
	}
	
	bool isLoaded() const {return _loaded;}
	Vect2<uint16_t> charSizes() const {return _charSizes;}
	sf::Sprite getSprite(uint16_t i) const {
		if (i < _charSprites.size()) return _charSprites[i];
		else return _dummy;
	};
	std::size_t getSpriteCount() const {return _charSprites.size();};
};

#endif