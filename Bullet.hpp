#pragma once

#include "SFML/Graphics.hpp"
#include <string>

class Bullet {
public:
	Bullet(const std::string& imagePath, sf::RenderWindow* window);
	sf::Sprite& getSprite();
	bool getOn();
	void setOn(bool on);
private:
	sf::Texture texture;
	sf::Sprite sprite;
	bool on;
};
