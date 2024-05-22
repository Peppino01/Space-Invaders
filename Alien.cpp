#include "Alien.hpp"

#include "Game.hpp"

using namespace std;

Alien::Alien(const std::string& imagePath, sf::RenderWindow* window, int type) {
    texture.loadFromFile(imagePath);
    sprite.setTexture(texture);

    // Modifico la dimensione dello sprite
    float scaleFactor = (Game::settingsValues.aliensSize - 1) * float(0.4 - 0.1) / float(10 - 1) + 0.1f;
    sprite.setScale(scaleFactor, scaleFactor);

    // Imposto l'offset per centrare l'origine dello sprite
    sf::Vector2u imageSize = texture.getSize();
    sprite.setOrigin(float(imageSize.x) / 2.0f, float(imageSize.y) / 2.0f);

    // Inizializzo bullet
    bullet = new Bullet("assets/images/bullet" + to_string(type) + ".png", window);

    this->type = type;
    alive = true;
}

sf::Sprite& Alien::getSprite() {
    return sprite;
}

void Alien::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Alien::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

bool Alien::getAlive() {
    return alive;
}

void Alien::setAlive(bool newAlive) {
    alive = newAlive;
}

int Alien::getType()
{
    return this->type;
}

void Alien::setType(bool type)
{
    this->type = type;
}

Bullet* Alien::getBullet() {
    return bullet;
}