#include "Game.hpp"

#include <Windows.h>
#include <iostream>
#include <random>

#include "SFML/Graphics.hpp"

#include "Alien.hpp"
#include "Bullet.hpp"
#include "Ship.hpp"

#include "Config.hpp"
#include "PlayUtils.hpp"
#include "RecordsUtils.hpp"
#include "SettingsUtils.hpp"

using namespace std;

// Definisco la variabile settingsValues (necessario in quanto statica)
Settings Game::settingsValues;

Game::Game()
{
    // Creo la finestra
    sf::VideoMode videoMode = sf::VideoMode(
        unsigned int(sf::VideoMode::getDesktopMode().width * Config::WINDOW_SIZE_RATIO),
        unsigned int(sf::VideoMode::getDesktopMode().height * Config::WINDOW_SIZE_RATIO)
    );
    window.create(videoMode, "Space invaders", sf::Style::Close);

    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    // Carico le impostazioni dal file all'oggetto settings
    std::vector<std::pair<std::string, int>> fileSettings = readSettingsFromFile("settings.json");
    for (int i = 0; i < fileSettings.size(); i++)
    {
        if (fileSettings[i].first == "ship speed")
        {
            this->settingsValues.shipSpeed = fileSettings[i].second;
        }
        else if (fileSettings[i].first == "ship size")
        {
            this->settingsValues.shipSize = float(fileSettings[i].second);
        }
        else if (fileSettings[i].first == "aliens speed")
        {
            this->settingsValues.aliensSpeed = fileSettings[i].second;
        }
        else if (fileSettings[i].first == "aliens size")
        {
            this->settingsValues.aliensSize = float(fileSettings[i].second);
        }
        else if (fileSettings[i].first == "bullet speed")
        {
            this->settingsValues.bulletSpeed = fileSettings[i].second;
        }
        else if (fileSettings[i].first == "bullet size")
        {
            this->settingsValues.bulletSize = float(fileSettings[i].second);
        }
    }

    // Setto la musica
    music.openFromFile("assets/music and sounds/music.ogg");
    music.setLoop(true);

    // Carico il font
    if (!this->font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Errore: impossibile caricare il font." << endl;
    }

    // Setto l'icona 
    sf::Image icon;
    icon.loadFromFile("assets/images/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().x, icon.getPixelsPtr());

    this->score = 0;
    this->time = 0;
    this->selection = START;
}

short Game::menu()
{
    // Creo il suono del click
    sf::SoundBuffer buffer;
    buffer.loadFromFile("assets/music and sounds/click.ogg");
    sf::Sound clickSound;
    clickSound.setBuffer(buffer);
    if (this->selection != START)
        clickSound.play();

    // Per la gestione del cursore
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    window.setMouseCursor(cursor);

    // Creazione del logo
    sf::Texture logoTexture;
    logoTexture.loadFromFile("assets/images/logo.png");
    sf::Sprite logoSprite;
    logoSprite.setTexture(logoTexture);
    float scaleFactor = Config::LOGO_SIZE_RATIO * window.getSize().x / logoTexture.getSize().x;
    logoSprite.setScale(scaleFactor, scaleFactor);
    sf::Vector2u imageSize = logoTexture.getSize();
    logoSprite.setOrigin(float(imageSize.x / 2), float(imageSize.y / 2));
    logoSprite.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 3));

    // Creazione dei bottoni
    sf::Text playButton;
    playButton.setFont(this->font);
    playButton.setString("PLAY");
    playButton.setOrigin(sf::Vector2f(playButton.getLocalBounds().width / 2, playButton.getLocalBounds().height / 2));
    playButton.setPosition(sf::Vector2f(float(window.getSize().x/2), logoSprite.getPosition().y + logoSprite.getLocalBounds().height / 2 + 160.0f));
    sf::Text settingsButton;
    settingsButton.setFont(this->font);
    settingsButton.setString("SETTINGS");
    settingsButton.setOrigin(sf::Vector2f(settingsButton.getLocalBounds().width / 2, settingsButton.getLocalBounds().height / 2));
    settingsButton.setPosition(sf::Vector2f(float(window.getSize().x / 2), playButton.getPosition().y + 60.0f));
    sf::Text recordsButton;
    recordsButton.setFont(this->font);
    recordsButton.setString("RECORDS");
    recordsButton.setOrigin(sf::Vector2f(recordsButton.getLocalBounds().width / 2, recordsButton.getLocalBounds().height / 2));
    recordsButton.setPosition(sf::Vector2f(float(window.getSize().x / 2), settingsButton.getPosition().y + 60.0f));
    sf::Text exitButton;
    exitButton.setFont(this->font);
    exitButton.setString("EXIT");
    exitButton.setOrigin(sf::Vector2f(exitButton.getLocalBounds().width / 2, exitButton.getLocalBounds().height / 2));
    exitButton.setPosition(sf::Vector2f(float(window.getSize().x / 2), recordsButton.getPosition().y + 60.0f));
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return EXIT;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                // Verifico se il click del mouse e' stato effettuato su uno dei tasti
                if (playButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    clickSound.play();
                    return PLAY;
                }
                else if (settingsButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    clickSound.play();
                    return SETTINGS;
                }
                else if (recordsButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    clickSound.play();
                    return RECORDS;
                }
                else if (exitButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    clickSound.play();
                    return EXIT;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                // Tutti i tasti sono di default bianchi e il cursore normale
                playButton.setFillColor(sf::Color::White);
                settingsButton.setFillColor(sf::Color::White);
                recordsButton.setFillColor(sf::Color::White);
                exitButton.setFillColor(sf::Color::White);
                cursor.loadFromSystem(sf::Cursor::Arrow);
                window.setMouseCursor(cursor);

                // Se il cursore si trova su un tasto lo colora di verde e cambia l'icona del cursore
                cursor.loadFromSystem(sf::Cursor::Hand);
                if (playButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    playButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
                else if (settingsButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    settingsButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
                else if (recordsButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    recordsButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
                else if (exitButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    exitButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
            }
        }

        window.clear();
        window.draw(logoSprite);
        window.draw(playButton);
        window.draw(settingsButton);
        window.draw(recordsButton);
        window.draw(exitButton);
        window.display();
    }

    // Solo per non generare il warning C4715
    return EXIT;
}

short Game::play()
{
    // Creo i sound per shoot, hit, specialHit, shipHit e defeat
    sf::SoundBuffer shootBuffer;
    shootBuffer.loadFromFile("assets/music and sounds/shoot.ogg");
    sf::Sound shootSound;
    shootSound.setBuffer(shootBuffer);
    sf::SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("assets/music and sounds/hit.ogg");
    sf::Sound hitSound;
    hitSound.setBuffer(hitBuffer);
    sf::SoundBuffer specialHitBuffer;
    specialHitBuffer.loadFromFile("assets/music and sounds/specialHit.ogg");
    sf::Sound specialHitSound;
    specialHitSound.setBuffer(specialHitBuffer);
    sf::SoundBuffer shipHitBuffer;
    shipHitBuffer.loadFromFile("assets/music and sounds/shipHit.ogg");
    sf::Sound shipHitSound;
    shipHitSound.setBuffer(shipHitBuffer);
    sf::SoundBuffer defeatBuffer;
    defeatBuffer.loadFromFile("assets/music and sounds/defeat.ogg");
    sf::Sound defeatSound;
    defeatSound.setBuffer(defeatBuffer);

    // Per la gestione del cursore
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    window.setMouseCursor(cursor);

    // Creo tutti gli elementi
    Ship ship("assets/images/ship.png", &window);
    Bullet bullet("assets/images/bullet.png", &window);
    Alien specialAlien("assets/images/alien4.png", &window, 4);
    Alien* aliensMatrix[Config::ALIENS_ROWS][Config::ALIENS_COLS];
    int type;
    for (int row = 0; row < Config::ALIENS_ROWS; ++row) {
        if (row == 1 || row == 2)
            type = 2;
        else if (row == 3 || row == 4)
            type = 3;
        else
            type = 1;
        for (int col = 0; col < Config::ALIENS_COLS; ++col) {
            aliensMatrix[row][col] = new Alien(Config::ALIEN_IMAGE_PATHS[row], &window, type);
        }
    }
    sf::Texture wallTexture;
    sf::Sprite wallSprite;
    wallTexture.loadFromFile("assets/images/wall.png");
    wallSprite.setTexture(wallTexture);
    wallSprite.setOrigin(sf::Vector2f(wallSprite.getLocalBounds().width / 2, wallSprite.getLocalBounds().height / 2));

    // Creo le scritte per le vite, il punteggio, il tempo e per tornare al menu
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setString("LIVES: 3");
    livesText.setCharacterSize(22);
    livesText.setPosition(sf::Vector2f(Config::SCORE_MARGIN, Config::SCORE_MARGIN));
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("SCORE: 0");
    scoreText.setCharacterSize(22);
    scoreText.setPosition(sf::Vector2f(Config::SCORE_MARGIN, livesText.getPosition().y + livesText.getLocalBounds().height + 10.0f));
    sf::Text timeText;
    timeText.setFont(font);
    timeText.setString("TIME: 0");
    timeText.setCharacterSize(22);
    timeText.setPosition(sf::Vector2f(Config::SCORE_MARGIN, scoreText.getPosition().y + scoreText.getLocalBounds().height + 10.0f));
    sf::Text menuButton;
    menuButton.setFont(this->font);
    menuButton.setString("MENU");
    menuButton.setOrigin(menuButton.getLocalBounds().width, 0.0f);
    menuButton.setPosition(float(window.getSize().x - Config::SCORE_MARGIN), float(Config::SCORE_MARGIN));
    

    bool aliensAreMovingRight = true;
    bool specialAlienIsMovingRight = false;
    bool changes = false; // Serve a sapere se ci sono stati cambiamenti gafici che quindi si devono stampare

    int score = 0;
    int lives = 3;
    int wave = 1;

    // Setto la velocita' e la percentuale di spari iniziale (poi verranno incrementate ad ogni wave)
    this->settingsValues.aliensSpeedDelay = int(Config::ALIENS_SPEED_DELAY);
    this->settingsValues.aliensShootPerc = Config::ALIEN_SHOOT_PERC;

    // Per la generazione di numeri casuali
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);

    // Faccio partire la musica
    music.play();

    // Setto i clock
    sf::Clock shipClock;
    sf::Clock aliensClock;
    sf::Clock bulletClock;
    sf::Clock aliensBulletClock;
    sf::Clock timeClock;
    sf::Clock specialAlienSpawnClock;
    sf::Clock specialAlienMovingClock;

    while (window.isOpen())
    {
        // Setto la posizione e gli stati degli elementi ai valori iniziali
        ship.getSprite().setPosition(float(window.getSize().x / 2), float(window.getSize().y * Config::Y_SHIP));
        resetAliens(aliensMatrix, &window);
        specialAlien.getSprite().setOrigin(specialAlien.getSprite().getLocalBounds().width / 2, 0.0f);
        specialAlien.setPosition(window.getSize().x + 100.0f, 50.0f);
        bullet.setOn(false);
        wallSprite.setPosition(sf::Vector2f(ship.getSprite().getPosition().x, ship.getSprite().getPosition().y - ship.getSprite().getLocalBounds().height - 30.0f));

        // Stampo per due secondi il numero dell'ondata
        sf::RectangleShape background(sf::Vector2f(float(window.getSize().x), float(window.getSize().y)));
        background.setFillColor(sf::Color(0, 0, 0, 200));
        background.setPosition(sf::Vector2f(0, 0));
        sf::Text waveText;
        waveText.setFont(font);
        waveText.setString("WAVE " + to_string(wave));
        waveText.setOrigin(waveText.getLocalBounds().width / 2, waveText.getLocalBounds().height / 2);
        waveText.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 2));
        window.clear();
        printAliens(aliensMatrix, &window);
        window.draw(specialAlien.getSprite());
        window.draw(ship.getSprite());
        window.draw(wallSprite);
        window.draw(livesText);
        window.draw(scoreText);
        window.draw(timeText);
        window.draw(menuButton);
        window.draw(background);
        window.draw(waveText);
        window.display();
        sf::sleep(sf::seconds(2));
        window.clear();
        window.display();

        // Questo ciclo rappresenta un'ondata, ogni ondata la velocità degli alieni viene aumentata
        while (true)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    music.stop();
                    return EXIT;
                }
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (menuButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                    {
                        music.stop();
                        return MENU;
                    }
                }
                else if (event.type == sf::Event::MouseMoved)
                {
                    // Tutti i tasti sono di default bianchi e il cursore normale
                    menuButton.setFillColor(sf::Color::White);
                    cursor.loadFromSystem(sf::Cursor::Arrow);
                    window.setMouseCursor(cursor);

                    // Se il cursore si trova su un tasto lo colora di verde e cambia l'icona del cursore
                    cursor.loadFromSystem(sf::Cursor::Hand);
                    if (menuButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                    {
                        menuButton.setFillColor(sf::Color::Green);
                        window.setMouseCursor(cursor);
                    }
                }
            }

            // Aggiorno timeText
            timeText.setString("TIME: " + to_string(int(timeClock.getElapsedTime().asSeconds())));

            // 1 - Controllo gli input dell'utente, le condizioni di collisione e simili
            if (bullet.getSprite().getPosition().y <= 0) {
                bullet.setOn(false);
            }
            if (checkIfAliensTouchBottom(aliensMatrix, &window, ship))
            {
                this->score = score;
                this->time = int(timeClock.getElapsedTime().asSeconds());
                music.stop();
                defeatSound.play();
                return DEFEAT;
            }
            checkIfWallHasBeenHit(aliensMatrix, &bullet, wallSprite);
            checkBulletAliensBounds(aliensMatrix, &window);
            if (checkIfAnAlienHasBeenHit(aliensMatrix, &bullet, &score, &scoreText, specialAlien, &hitSound))
            {
                specialAlienSpawnClock.restart();
                specialAlien.setPosition(specialAlienIsMovingRight ? window.getSize().x + 100.0f : -100.0f, 50.0f);
                specialHitSound.play();
            }
            if (checkIfShipHasBeenHit(aliensMatrix, &ship))
            {
                shipHitSound.play();
                if (lives <= 0)
                {
                    this->score = score;
                    this->time = int(timeClock.getElapsedTime().asSeconds());
                    music.stop();
                    defeatSound.play();
                    sf::sleep(sf::milliseconds(2000));
                    return DEFEAT;
                }
                else
                {
                    lives--;
                    livesText.setString("LIVES: " + to_string(lives));
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bullet.getOn()) {
                bullet.setOn(true);
                bullet.getSprite().setPosition(ship.getSprite().getPosition().x, ship.getSprite().getPosition().y - ship.getSprite().getLocalBounds().height / 2);
                shootSound.play();
                changes = true;
            }
            else if (bullet.getOn() && bulletClock.getElapsedTime().asMilliseconds() >= Config::BULLET_SPEED_DELAY) {
                bullet.getSprite().move(0, -Game::settingsValues.bulletSpeed * 1.5f);
                bulletClock.restart();
                changes = true;
            }

            // 2 - Aggiorno la posizione di tutti gli elementi
            if (aliensClock.getElapsedTime().asMilliseconds() >= Game::settingsValues.aliensSpeedDelay) {
                // Se qualche alieno incontra i bordi destri o sinistri faccio avanzare gli alieni e cambio la direzione degli alieni
                bool newAliensAreMovingRight = checkAliensBounds(aliensMatrix, &window, aliensAreMovingRight);
                if (newAliensAreMovingRight == aliensAreMovingRight) {
                    moveAliens(aliensMatrix, aliensAreMovingRight ? float(Game::settingsValues.aliensSpeed * 5) : float(- Game::settingsValues.aliensSpeed * 5), 0.0f);
                }
                else {
                    moveAliens(aliensMatrix, 0.0f, float(Config::ALIENS_Y_SPEED));
                    aliensAreMovingRight = newAliensAreMovingRight;
                }

                // Ogni tanto un'alieno random sparer  un proiettile
                alienShoot(Game::settingsValues.aliensShootPerc, aliensMatrix);

                aliensClock.restart();
                changes = true;
            }
            if (shipClock.getElapsedTime().asMilliseconds() >= Config::SHIP_SPEED_DELAY) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (ship.getSprite().getPosition().x > ship.getSprite().getLocalBounds().width / 2)) {
                    ship.move(false);
                    shipClock.restart();
                    changes = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (ship.getSprite().getPosition().x < window.getSize().x - ship.getSprite().getLocalBounds().width / 2)) {
                    ship.move(true);
                    shipClock.restart();
                    changes = true;
                }
            }
            if (aliensBulletClock.getElapsedTime().asMilliseconds() >= Config::ALIENS_BULLET_SPEED_DELAY) {
                moveAliensBullet(aliensMatrix, &window);
                aliensBulletClock.restart();
                changes = true;
            }
            if (
                specialAlienSpawnClock.getElapsedTime().asSeconds() >= Config::SPECIAL_ALIEN_SPAWN_DELAY &&
                specialAlienMovingClock.getElapsedTime().asMilliseconds() >= Config::SPECIAL_ALIEN_SPEED_DELAY
            )
            {
                specialAlien.move(specialAlienIsMovingRight ? float(Config::SPECIAL_ALIEN_SPEED) : float(- Config::SPECIAL_ALIEN_SPEED), 0.0f);
                specialAlienMovingClock.restart();
                if (
                    specialAlienIsMovingRight && specialAlien.getSprite().getPosition().x >= window.getSize().x + 100 ||
                    !specialAlienIsMovingRight && specialAlien.getSprite().getPosition().x <= -100
                )
                {
                    specialAlienSpawnClock.restart();
                    specialAlienIsMovingRight = !specialAlienIsMovingRight;
                }
            }

            // 3 - Se almeno un elemento ha cambiato posizione ristampo tutti gli elementi e aggiorno il display
            if (changes) {
                window.clear();

                printAliens(aliensMatrix, &window);
                window.draw(specialAlien.getSprite());
                window.draw(ship.getSprite());
                if (bullet.getOn())
                    window.draw(bullet.getSprite());
                printAliensBullet(aliensMatrix, &window);
                window.draw(wallSprite);
                window.draw(livesText);
                window.draw(scoreText);
                window.draw(timeText);
                window.draw(menuButton);

                window.display();
                changes = false;
            }
            
            if (aliensDied(aliensMatrix))
            {
                // Aumento la velocita' degli alieni ed esco da questa wave
                if (this->settingsValues.aliensSpeedDelay >= 100)
                    this->settingsValues.aliensSpeedDelay -= 100;
                if (this->settingsValues.aliensShootPerc <= 80)
                    this->settingsValues.aliensShootPerc += 20;
                wave++;
                break;
            }


        }


        
    }

    // Solo per non generare il warning C4715
    return EXIT;
}

short Game::settings()
{
    // Creo il suono del click
    sf::SoundBuffer buffer;
    buffer.loadFromFile("assets/music and sounds/click.ogg");
    sf::Sound clickSound;
    clickSound.setBuffer(buffer);
    // Questo click rappresenta quello che è stato fatto per entrare in questa pagina
    clickSound.play();
    
    // Per la gestione del cursore
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    window.setMouseCursor(cursor);

    // Prendo le impostazioni attuali
    std::vector<std::pair<std::string, int>> settings = readSettingsFromFile("settings.json");

    // Creo la scritta per dire all'utente come modificare i valori
    sf::Text infoText;
    infoText.setFont(this->font);
    infoText.setCharacterSize(20);
    infoText.setString("you can increase or decrease the values by\nclicking with the left or right mouse buttons on them");
    infoText.setPosition(sf::Vector2f(50.0f, 150.0f));
    
    // Creo una scritta per ogni coppia chiave valore (quindi per ogni impostazione) presente nel json
    std::vector<sf::Text> settingsTexts = {};
    for (int i = 0; i < settings.size(); i++)
    {
        sf::Text text;
        text.setFont(this->font);
        text.setString(settings[i].first + ": " + to_string(settings[i].second));
        text.setPosition(sf::Vector2f(50.0f, (i + 1) * 60.0f + 200.0f));
        settingsTexts.push_back(text);
    }

    // Creo il bottone per tornare indietro
    sf::Text menuButton;
    menuButton.setFont(this->font);
    menuButton.setString("MENU");
    menuButton.setPosition(sf::Vector2f(50.0f, 50.0f));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return EXIT;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                // Verifico se il click del mouse e' stato effettuato sul tasto menu
                if (menuButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    return MENU;
                }
                // Verifico se il click del mouse e' stato effettuato su uno dei valori delle impostazioni
                int i = 0;
                for (auto& s : settings) {
                    sf::Text sText = settingsTexts[i];
                    if (sText.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                    {
                        if (event.mouseButton.button == sf::Mouse::Left && s.second < 20)
                        {
                            s.second = s.second + 1;
                            clickSound.play();
                            writeSettingsToFile("settings.json", settings);
                        }
                        else if (event.mouseButton.button == sf::Mouse::Right && s.second > 1)
                        {
                            s.second = s.second - 1;
                            clickSound.play();
                            writeSettingsToFile("settings.json", settings);
                        }
                    }
                    // Aggiorno i text ai nuovi valori delle impostazioni
                    for (int i = 0; i < settings.size(); i++)
                    {
                        settingsTexts[i].setString(settings[i].first + ": " + to_string(settings[i].second));
                    }
                    // Carico le impostazioni dal file all'oggetto settings
                    std::vector<std::pair<std::string, int>> fileSettings = readSettingsFromFile("settings.json");
                    for (int i = 0; i < fileSettings.size(); i++)
                    {
                        if (fileSettings[i].first == "ship speed")
                        {
                            this->settingsValues.shipSpeed = fileSettings[i].second;
                        }
                        else if (fileSettings[i].first == "ship size")
                        {
                            this->settingsValues.shipSize = float(fileSettings[i].second);
                        }
                        else if (fileSettings[i].first == "aliens speed")
                        {
                            this->settingsValues.aliensSpeed = fileSettings[i].second;
                        }
                        else if (fileSettings[i].first == "aliens size")
                        {
                            this->settingsValues.aliensSize = float(fileSettings[i].second);
                        }
                        else if (fileSettings[i].first == "bullet speed")
                        {
                            this->settingsValues.bulletSpeed = fileSettings[i].second;
                        }
                        else if (fileSettings[i].first == "bullet size")
                        {
                            this->settingsValues.bulletSize = float(fileSettings[i].second);
                        }
                    }
                    i++;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                // Tutti i tasti sono di default bianchi e il cursore normale
                menuButton.setFillColor(sf::Color::White);
                cursor.loadFromSystem(sf::Cursor::Arrow);
                window.setMouseCursor(cursor);

                // Se il cursore si trova su un tasto lo colora di verde e cambia l'icona del cursore
                cursor.loadFromSystem(sf::Cursor::Hand);
                if (menuButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    menuButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
                for (auto& settingText : settingsTexts)
                {
                    if (settingText.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                    {
                        window.setMouseCursor(cursor);
                    }
                }
            }
        }


        window.clear();
        window.draw(menuButton);
        window.draw(infoText);
        for (const auto& s : settingsTexts) {
            window.draw(s);
        }
        window.display();
    }

    // Solo per non generare il warning C4715
    return EXIT;
}

short Game::records()
{
    // Creo il suono del click
    sf::SoundBuffer buffer;
    buffer.loadFromFile("assets/music and sounds/click.ogg");
    sf::Sound clickSound;
    clickSound.setBuffer(buffer);
    // Questo click rappresenta quello che è stato fatto per entrare in questa pagina
    clickSound.play();

    // Per la gestione del cursore
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    window.setMouseCursor(cursor);

    // Creo il bottone per tornare indietro
    sf::Text menuButton;
    menuButton.setFont(this->font);
    menuButton.setString("MENU");
    menuButton.setPosition(sf::Vector2f(50.0f, 50.0f));

    // Creo la scritta per dire all'utente come eliminare i record
    sf::Text infoText;
    infoText.setFont(this->font);
    infoText.setCharacterSize(20);
    infoText.setString("you can delete records by clicking on them");
    infoText.setPosition(sf::Vector2f(50.0f, menuButton.getPosition().y + menuButton.getLocalBounds().height + 50.0f));

    std::vector<Record> records = readRecordsFromJSON("Records.json");

    // Creo una scritta per ogni record
    std::vector<sf::Text> recordsTexts = {};
    int col = 0;
    int row = 0;
    for (int i = 0; i < records.size(); i++)
    {
        sf::Text text;
        text.setFont(this->font);
        text.setCharacterSize(20);
        text.setString(records[i].playerName + "\nSCORE: " + to_string(records[i].score) + "\nTIME: " + to_string(records[i].time));
        text.setPosition(sf::Vector2f(50.0f + (col * 200.0f), infoText.getPosition().y + infoText.getLocalBounds().height + 50.0f + (row * 100.0f)));
        recordsTexts.push_back(text);
        row++;
        if (text.getPosition().y > window.getSize().y - 200) // Quando l'ultimo record si avvicina al bordo inferiore dello schermo scrivo gli altri su una nuova colonna
        {
            col++;
            row = 1;
        }
    }

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return EXIT;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                // Verifico se il click del mouse e' stato effettuato sul tasto menu
                if (menuButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    return MENU;
                }
                // Verifico se il click del mouse e' stato fatto su uno dei record
                else
                {
                    for (int i = 0; i < records.size(); i++)
                    {
                        if (recordsTexts[i].getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                        {
                            clickSound.play();
                            // Rimuovo il record e ricarico tutti i record dal file
                            removeRecord("Records.json", records[i].id);
                            records = readRecordsFromJSON("Records.json");

                            // Creo una scritta per ogni record
                            recordsTexts = {};
                            int col = 0;
                            int row = 1;
                            for (int i = 0; i < records.size(); i++)
                            {
                                sf::Text text;
                                text.setFont(this->font);
                                text.setCharacterSize(20);
                                text.setString(records[i].playerName + "\nSCORE: " + to_string(records[i].score) + "\nTIME: " + to_string(records[i].time));
                                text.setPosition(sf::Vector2f(50.0f + (col * 200.0f), 50.0f + (row * 100.0f)));
                                recordsTexts.push_back(text);
                                row++;
                                if (text.getPosition().y > window.getSize().y - 200) // Quando l'ultimo record si avvicina al bordo inferiore dello schermo scrivo gli altri su una nuova colonna
                                {
                                    col++;
                                    row = 1;
                                }
                            }
                        }
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                // Tutti i tasti sono di default bianchi e il cursore normale
                menuButton.setFillColor(sf::Color::White);
                for (int i = 0; i < records.size(); i++)
                {
                    recordsTexts[i].setFillColor(sf::Color::White);
                }
                cursor.loadFromSystem(sf::Cursor::Arrow);
                window.setMouseCursor(cursor);

                // Se il cursore si trova su un tasto lo colora di verde e cambia l'icona del cursore
                cursor.loadFromSystem(sf::Cursor::Hand);
                if (menuButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    menuButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
                for (int i = 0; i < records.size(); i++)
                {
                    if (recordsTexts[i].getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                    {
                        recordsTexts[i].setFillColor(sf::Color::Red);
                        window.setMouseCursor(cursor);
                    }
                }
            }
        }



        window.clear();
        window.draw(menuButton);
        window.draw(infoText);
        for (const auto& s : recordsTexts) {
            window.draw(s);
        }
        window.display();
    }

    // Solo per non generare il warning C4715
    return EXIT;
}

short Game::defeat()
{
    // Setto la musica
    sf::Music defeatMusic;
    defeatMusic.openFromFile("assets/music and sounds/defeatMusic.ogg");
    defeatMusic.setLoop(true);
    defeatMusic.play();

    // Per la gestione del cursore
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Arrow);
    window.setMouseCursor(cursor);

    // Creo il testo con lo score
    sf::Text defeatText;
    defeatText.setFont(this->font);
    defeatText.setString("GAME OVER! score: " + to_string(this->score));
    defeatText.setCharacterSize(64);
    defeatText.setOrigin(sf::Vector2f(defeatText.getLocalBounds().width / 2, defeatText.getLocalBounds().height / 2));
    defeatText.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 150.0f));

    // Creo il campo di imput testuale per inserire il nome e il bottone per salvare
    sf::Text nameText;
    nameText.setFont(this->font);
    nameText.setString("Enter your name to save your score");
    nameText.setOrigin(sf::Vector2f(nameText.getLocalBounds().width / 2, nameText.getLocalBounds().height / 2));
    nameText.setPosition(sf::Vector2f(float(window.getSize().x / 2), defeatText.getPosition().y + defeatText.getLocalBounds().height + 100.0f));
    sf::Text saveButton;
    saveButton.setFont(this->font);
    saveButton.setFillColor(sf::Color(255, 255, 255, 100));
    saveButton.setString("SAVE SCORE");
    saveButton.setOrigin(sf::Vector2f(saveButton.getLocalBounds().width / 2, saveButton.getLocalBounds().height / 2));
    saveButton.setPosition(sf::Vector2f(float(window.getSize().x / 2), nameText.getPosition().y + nameText.getLocalBounds().height + 30.0f));

    // Creo la scritta per il salvataggio del record
    sf::Text recordSavedText;
    recordSavedText.setFont(this->font);
    recordSavedText.setFillColor(sf::Color::Green);
    recordSavedText.setString("RECORD SAVED");
    recordSavedText.setOrigin(sf::Vector2f(recordSavedText.getLocalBounds().width / 2, recordSavedText.getLocalBounds().height / 2));
    recordSavedText.setPosition(saveButton.getPosition());

    // Creo i bottoni per giocare di nuovo e per tornare al menu
    sf::Text playButton;
    playButton.setFont(this->font);
    playButton.setString("PLAY AGAIN");
    playButton.setOrigin(sf::Vector2f(playButton.getLocalBounds().width / 2, playButton.getLocalBounds().height / 2));
    playButton.setPosition(sf::Vector2f(float(window.getSize().x / 2), saveButton.getPosition().y + saveButton.getLocalBounds().height + 100.0f));
    sf::Text menuButton;
    menuButton.setFont(this->font);
    menuButton.setString("MENU");
    menuButton.setOrigin(sf::Vector2f(menuButton.getLocalBounds().width / 2, menuButton.getLocalBounds().height / 2));
    menuButton.setPosition(sf::Vector2f(float(window.getSize().x / 2), playButton.getPosition().y + playButton.getLocalBounds().height + 30.0f));


    bool isNameValid = false;
    bool recordSaved = false;
    std::string inputText;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return EXIT;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                // Verifico se il click del mouse e' stato effettuato su uno dei tasti
                if (nameText.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)) && !isNameValid && !recordSaved)
                {
                    nameText.setString("_");
                    nameText.setOrigin(sf::Vector2f(nameText.getLocalBounds().width / 2, nameText.getLocalBounds().height / 2));
                }
                else if (playButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    defeatMusic.stop();
                    return PLAY;
                }
                else if (saveButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)) && isNameValid && !recordSaved)
                {
                    addRecord("Records.json", inputText, this->score, this->time, "00/00/0000");
                    recordSaved = true;
                }
                else if (menuButton.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y)))
                {
                    defeatMusic.stop();
                    return MENU;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                // Tutti i tasti sono di default bianchi e il cursore normale
                saveButton.setFillColor(isNameValid ? sf::Color::White : sf::Color(255, 255, 255, 100));
                playButton.setFillColor(sf::Color::White);
                menuButton.setFillColor(sf::Color::White);
                cursor.loadFromSystem(sf::Cursor::Arrow);
                window.setMouseCursor(cursor);

                // Se il cursore si trova su un tasto lo colora di verde e cambia l'icona del cursore
                cursor.loadFromSystem(sf::Cursor::Hand);
                if (saveButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)) && isNameValid && !recordSaved)
                {
                    saveButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
                else if (playButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    playButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
                else if (menuButton.getGlobalBounds().contains(float(event.mouseMove.x), float(event.mouseMove.y)))
                {
                    menuButton.setFillColor(sf::Color::Green);
                    window.setMouseCursor(cursor);
                }
            }
            // Verifica se l'utente ha premuto un tasto
            else if (event.type == sf::Event::TextEntered && !recordSaved)
            {
                // Verifica se il tasto premuto è un carattere stampabile
                if (event.text.unicode < 128)
                {
                    // Se è un carattere valido e la lunghezza massima non è stata raggiunta, aggiungilo alla stringa di testo
                    if (event.text.unicode != '\b' && inputText.size() < Config::MAX_LETTERS_NAME) // '\b' è il codice del tasto backspace
                    {
                        inputText += static_cast<char>(event.text.unicode);
                    }
                    else if (!inputText.empty() && event.text.unicode == '\b') // Se è il tasto backspace e la stringa non è vuota, rimuovi l'ultimo carattere
                    {
                        inputText.pop_back();
                    }

                    // Aggiorna il testo visualizzato sulla finestra
                    nameText.setString(inputText + "_");
                    nameText.setOrigin(sf::Vector2f(nameText.getLocalBounds().width / 2, nameText.getLocalBounds().height / 2));
                    if (!inputText.empty())
                    {
                        isNameValid = true;
                        saveButton.setFillColor(sf::Color::White);
                    }
                    else
                    {
                        isNameValid = false;
                        nameText.setString("Enter your name to save your score");
                        nameText.setOrigin(sf::Vector2f(nameText.getLocalBounds().width / 2, nameText.getLocalBounds().height / 2));
                        saveButton.setFillColor(sf::Color(255, 255, 255, 100));
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed && isNameValid && !recordSaved) {
                if (event.key.code == sf::Keyboard::Enter) {
                    addRecord("Records.json", inputText, this->score, this->time, "00/00/0000");
                    recordSaved = true;
                }
            }
        }

        window.clear();
        window.draw(defeatText);
        window.draw(nameText);
        recordSaved ? window.draw(recordSavedText) : window.draw(saveButton);
        window.draw(playButton);
        window.draw(menuButton);
        window.display();
    }

    // Solo per non generare il warning C4715
    return EXIT;
}

short Game::exit()
{
    // Creo il suono del click
    sf::SoundBuffer buffer;
    buffer.loadFromFile("assets/music and sounds/click.ogg");
    sf::Sound clickSound;
    clickSound.setBuffer(buffer);
    // Questo click rappresenta quello che è stato fatto per entrare in questa pagina
    clickSound.play();
    sf::sleep(sf::milliseconds(200));

    return NULL;
}
