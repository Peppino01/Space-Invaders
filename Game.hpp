#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SettingsUtils.hpp"

class Game
{

public:
	Game();
	short selection;
	short menu();
	short play();
	short settings();
	short records();
	short defeat();
	short exit();
	static Settings settingsValues;

private:
	sf::RenderWindow window;
	sf::Font font;
	sf::Music music;
	int score;
	int time;

};
