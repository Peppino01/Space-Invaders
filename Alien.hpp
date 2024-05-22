#pragma once

#include "SFML/Graphics.hpp"
#include "Bullet.hpp"

class Alien {
public:
	Alien(const std::string& imagePath, sf::RenderWindow* window, int type);

private:
	int type;
	sf::Texture texture;
	sf::Sprite sprite;
	Bullet *bullet;
	bool alive;

public:
	sf::Sprite& getSprite();
	void setPosition(float x, float y);
	void move(float offsetX, float offsetY);
	Bullet* getBullet();
	bool getAlive(); // Ritorna true se l'alieno e' vivo
	void setAlive(bool newAlive);
	int getType();
	void setType(bool type);
};
