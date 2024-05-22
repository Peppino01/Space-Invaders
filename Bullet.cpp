#include "Bullet.hpp"

#include "SFML/Graphics.hpp"
#include "Game.hpp"

Bullet::Bullet(const std::string& imagePath, sf::RenderWindow* window) {
    texture.loadFromFile(imagePath);
    sprite.setTexture(texture);
    
    // Modifico la dimensione dello sprite
    float scaleFactor = (Game::settingsValues.bulletSize - 1) * float(1 - 0.1) / float(10 - 1) + 0.1f;
    sprite.setScale(scaleFactor, scaleFactor);

    // imposto l'offset per centrare l'origine dello sprite
    sf::Vector2u imageSize = texture.getSize();
    sprite.setOrigin(float(imageSize.x) / 2.0f, float(imageSize.y) / 2.0f);

    on = false;
}

sf::Sprite& Bullet::getSprite() {
    return sprite;
}

bool Bullet::getOn() {
    return on;
}

void Bullet::setOn(bool on) {
    this->on = on;
}