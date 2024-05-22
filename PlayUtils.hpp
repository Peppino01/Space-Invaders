#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Alien.hpp"
#include "Config.hpp"
#include "Ship.hpp"

using namespace std;

// Se i due sprite passati si sovrappongono ritorna true
bool areSpritesTouching(const sf::Sprite& sprite1, const sf::Sprite& sprite2);

// Setta la posizione di default degli alieni
void setAliensPosition(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window);

void printAliens(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window);

void moveAliens(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], float offsetX, float offsetY);

// Se tutti gli alieni sono stati uccisi ritorna true
bool aliensDied(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS]);

bool checkAliensBounds(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window, bool aliensAreMovingRight);

void printAliensBullet(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window);

void moveAliensBullet(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window);

void checkBulletAliensBounds(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window);

// Con una certa probabilita' sceglie un alieno casualmente e lo fa sparare
void alienShoot(int percentage, Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS]);

// Funzione per generare numeri casuali tra 0 e un valore specificato
int getRandomNumber(int minValue, int maxValue);

// Ritorna true solo se è stato colpito l'alieno speciale
bool checkIfAnAlienHasBeenHit(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], Bullet* bullet, int* score, sf::Text* scoreText, Alien specialAlien, sf::Sound* hitSound);

// Se la navicella e' stata colpita ritorna true
bool checkIfShipHasBeenHit(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], Ship* ship);

// Se un alieno raggiunge la stessa altezza (o più) della navicella ritorna true
bool checkIfAliensTouchBottom(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window, Ship ship);

void checkIfWallHasBeenHit(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], Bullet* bullet, sf::Sprite wall);

// Se nessun alieno rispetta le specifiche restituisce nullptr
int* chooseRandomAlien(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS]);

void resetEverything(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window, Bullet* bullet, int* score, sf::Text* scoreText);

void resetAliens(Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS], sf::RenderWindow* window);
