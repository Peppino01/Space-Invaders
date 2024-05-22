#include "PlayUtils.hpp"

#include <iostream>
#include <random>

#include "Alien.hpp"
#include "Bullet.hpp"
#include "Ship.hpp"

#include "Game.hpp"
#include "Config.hpp"

bool areSpritesTouching(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    // Ottengo i rettangoli di delimitazione globali dei due sprite
    sf::FloatRect bounds1 = sprite1.getGlobalBounds();
    sf::FloatRect bounds2 = sprite2.getGlobalBounds();

    // Verifico se i rettangoli di delimitazione si sovrappongono
    return bounds1.intersects(bounds2);
}

void setAliensPosition(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            // Calcolo la posizione dell'alieno sulla finestra in base alla sua riga e colonna
            float alienX = col * (Config::DIST_ALIENS_X + Game::settingsValues.aliensSize * 5) + Config::X_START_ALIENS;
            float alienY = row * (Config::DIST_ALIENS_Y + Game::settingsValues.aliensSize * 5) + Config::Y_START_ALIENS;
            aliensMatrix[row][col]->setPosition(alienX, alienY);
        }
    }
}

bool aliensDied(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS]) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]->getAlive())
            {
                return false;
            }
        }
    }
    return true;
}

void printAliens(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]->getAlive()) {
                window->draw(aliensMatrix[row][col]->getSprite());
            }
        }
    }
}

void moveAliens(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], float offsetX, float offsetY) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]) {
                aliensMatrix[row][col]->move(offsetX, offsetY);
            }
        }
    }
}

bool checkAliensBounds(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window, bool aliensAreMovingRight) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (
                aliensMatrix[row][col]->getSprite().getPosition().x <= Config::ALIENS_X_BARRIER &&
                aliensMatrix[row][col]->getAlive() &&
                !aliensAreMovingRight
                ) {
                // Se un alieno incontra la fine sinstra
                return true;
            }
            else if (
                aliensMatrix[row][col]->getSprite().getPosition().x >= window->getSize().x - Config::ALIENS_X_BARRIER &&
                aliensMatrix[row][col]->getAlive() &&
                aliensAreMovingRight
                ) {
                // Se un alieno incontra la fine destra
                return false;
            }
        }
    }
    return aliensAreMovingRight;
}

void printAliensBullet(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]->getBullet()->getOn()) {
                window->draw(aliensMatrix[row][col]->getBullet()->getSprite());
            }
        }
    }
}

void moveAliensBullet(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]->getBullet()->getOn()) {
                aliensMatrix[row][col]->getBullet()->getSprite().move(0, Game::settingsValues.bulletSpeed * 1.5f);
            }
        }
    }
}

void checkBulletAliensBounds(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]->getBullet()->getSprite().getPosition().y >= window->getSize().y) {
                aliensMatrix[row][col]->getBullet()->setOn(false);
            }
        }
    }
}

void alienShoot(int percentage, Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS]) {
    // Verifica se la percentuale e' valida
    if (percentage < 0 || percentage > 100) {
        cout << "ALIEN_SHOOT_PERC non valido";
        return;
    }

    // Inizializza il generatore di numeri casuali
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    // Genera un numero casuale
    int randomNumber = dis(gen);

    // Faccio sparare un alieno solo con una certa probabilità
    if (randomNumber < percentage) {
        int* randomIndex = chooseRandomAlien(aliensMatrix);
        if (randomIndex != nullptr) {
            aliensMatrix[randomIndex[0]][randomIndex[1]]->getBullet()->setOn(true);
            aliensMatrix[randomIndex[0]][randomIndex[1]]->getBullet()->getSprite().setPosition(
                aliensMatrix[randomIndex[0]][randomIndex[1]]->getSprite().getPosition().x,
                aliensMatrix[randomIndex[0]][randomIndex[1]]->getSprite().getPosition().y + aliensMatrix[randomIndex[0]][randomIndex[1]]->getSprite().getLocalBounds().height / 2
            );
        }
    }
}

int getRandomNumber(int minValue, int maxValue) {
    // Inizializza il generatore di numeri casuali
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minValue, maxValue); // Genera un numero casuale tra 0 e maxValue

    // Genera e restituisce il numero casuale
    return dis(gen);
}

bool checkIfAnAlienHasBeenHit(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], Bullet* bullet, int* score, sf::Text* scoreText, Alien specialAlien, sf::Sound* hitSound) {
    if (areSpritesTouching(specialAlien.getSprite(), bullet->getSprite()) && bullet->getOn()) {
        bullet->setOn(false);
        score[0] += 150;
        return true;
    }
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (areSpritesTouching(aliensMatrix[row][col]->getSprite(), bullet->getSprite()) && aliensMatrix[row][col]->getAlive() && bullet->getOn()) {
                bullet->setOn(false);
                aliensMatrix[row][col]->setAlive(false);
                switch (aliensMatrix[row][col]->getType())
                {
                case 1:
                    score[0] += 40;
                    break;
                case 2:
                    score[0] += 20;
                    break;
                case 3:
                    score[0] += 10;
                    break;
                default:
                    break;
                }
                col = Config::ALIENS_COLS;
                row = Config::ALIENS_ROWS;
                hitSound->play();
            }
        }
    }
    string scoreString = "SCORE: " + to_string(*score);
    scoreText->setString(scoreString);
    return false;
}

bool checkIfShipHasBeenHit(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], Ship* ship) {
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (areSpritesTouching(aliensMatrix[row][col]->getBullet()->getSprite(), ship->getSprite()) && aliensMatrix[row][col]->getBullet()->getOn()) {
                aliensMatrix[row][col]->getBullet()->setOn(false);
                return true;
            }
        }
    }
    return false;
}

void checkIfWallHasBeenHit(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], Bullet* bullet, sf::Sprite wall)
{
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (areSpritesTouching(aliensMatrix[row][col]->getBullet()->getSprite(), wall) && aliensMatrix[row][col]->getBullet()->getOn()) {
                aliensMatrix[row][col]->getBullet()->setOn(false);
            }
        }
    }
    if (areSpritesTouching(bullet->getSprite(), wall) && bullet->getOn()) {
        bullet->setOn(false);
    }
}

bool checkIfAliensTouchBottom(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window, Ship ship)
{
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]->getSprite().getPosition().y >= ship.getSprite().getPosition().y && aliensMatrix[row][col]->getAlive()) {
                return true;
            }
        }
    }
    return false;
}

int* chooseRandomAlien(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS]) {
    struct AlienIndex {
        int pair[2];
        struct AlienIndex* next;
        AlienIndex(int row, int col) {
            AlienIndex::pair[0] = row;
            AlienIndex::pair[1] = col;
            AlienIndex::next = nullptr;
        }
    };

    // Creo la lista di alieni candidati per sparare
    AlienIndex* head = nullptr, * ptr = nullptr;
    int listSize = 0; // Numero di elementi nella lista
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            if (aliensMatrix[row][col]->getAlive() && !aliensMatrix[row][col]->getBullet()->getOn()) {
                if (head == nullptr) {
                    head = new AlienIndex(row, col);
                    ptr = head;
                }
                else {
                    ptr->next = new AlienIndex(row, col);
                    ptr = ptr->next;
                }
                listSize++;
            }
        }
    }

    // Se la lista e' vuota restituisco nullptr
    if (listSize == 0) {
        return nullptr;
    }

    // genero un nunmero casuale corrispondente ad un alienIndex
    int randomNumber = getRandomNumber(0, listSize - 1);

    // Scorro la lista fin quando non trovo l'alieno interessato
    for (int i = 0; i < randomNumber; i++) {
        head = head->next;
    }

    return head->pair;
}

void resetEverything(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window, Bullet* bullet, int* score, sf::Text* scoreText) {
    score[0] = 0;
    bullet->setOn(false);
    setAliensPosition(aliensMatrix, window);
    scoreText->setString("SCORE: 0");
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            aliensMatrix[row][col]->setAlive(true);
            aliensMatrix[row][col]->getBullet()->setOn(false);
        }
    }
}

void resetAliens(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window)
{
    setAliensPosition(aliensMatrix, window);
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            aliensMatrix[row][col]->setAlive(true);
            aliensMatrix[row][col]->getBullet()->setOn(false);
        }
    }
}