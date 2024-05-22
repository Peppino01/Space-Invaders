#include "Ship.hpp"

#include <string>

#include "SFML/Graphics.hpp"
#include "Game.hpp"

Ship::Ship(const std::string& imagePath, sf::RenderWindow* window) {
    this->texture.loadFromFile(imagePath);
    this->sprite.setTexture(texture);

    // Modifico la dimensione dello sprite
    float scaleFactor = (Game::settingsValues.shipSize - 1) * float(1 - 0.1) / float(10 - 1) + 0.1f;
    sprite.setScale(scaleFactor, scaleFactor);

    // imposto l'offset per centrare l'origine dello sprite
    sf::Vector2u imageSize = texture.getSize();
    sprite.setOrigin(float(imageSize.x) / 2.0f, float(imageSize.y) / 2.0f);
}

sf::Sprite& Ship::getSprite() {
    return sprite;
}

void Ship::move(bool toRight) {
    if (toRight) {
        sprite.move(float(Game::settingsValues.shipSpeed), 0.0f);
    }
    else {
        sprite.move(float(- Game::settingsValues.shipSpeed), 0.0f);
    }
}