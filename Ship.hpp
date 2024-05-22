#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Ship {
public:
	Ship(const std::string& imagePath, sf::RenderWindow* window);
	sf::Sprite& getSprite();
	void move(bool toRight);
private:
	sf::Texture texture;
	sf::Sprite sprite;
};
