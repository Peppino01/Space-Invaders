#include <iostream>
#include "Game.hpp"
#include "Config.hpp"

using namespace std;

int main() {

	/*
	* 
	* Game è la classe che gestisce tutte le schermate del gioco.
	* Ogni schermata è un metodo di game e la schermata di default è il menu'.
	* Quando l'utente chiede di cambiare pagina (passare ad un'altra schermata)
	* il metodo attuale ritorna uno short che rappresenta la prossima schermata
	* (associazione tra short e schermate in Config.h).
	* 
	*/
	Game game;

	do
	{
		switch (game.selection)
		{
		case START:
		case MENU:
			game.selection = game.menu();
			break;
		case PLAY:
			game.selection = game.play();
			break;
		case SETTINGS:
			game.selection = game.settings();
			break;
		case RECORDS:
			game.selection = game.records();
			break;
		case DEFEAT:
			game.selection = game.defeat();
			break;
		case EXIT:
			game.selection = game.exit();
			break;
		default:
			cout << "Error: selection '" << game.selection << "' not valid" << endl;
			game.selection = EXIT;
			break;
		}

	} while (game.selection != NULL);

	return 0;

}