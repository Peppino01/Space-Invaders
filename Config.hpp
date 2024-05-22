#pragma once

#include <string>

// Schermate
#define START    0 // Rimanda sempre al menu' ma è utilizzata per sapere se si arriva al menu' per la prima volta
#define MENU     1
#define PLAY     2
#define SETTINGS 3
#define RECORDS  4
#define DEFEAT   5
#define EXIT     6

namespace Config {

    // Grafica
    const float WINDOW_SIZE_RATIO = 4.0f / 5.0f; // dimensioni della finestra rispetto allo schermo
    const float LOGO_SIZE_RATIO = 1.0f / 2.0f; // dimensioni del logo rispetto alla finestra
    const int SCORE_MARGIN = 20; // pixel di distanza tra score e il bordo della finestra
    
    // Configurazione alieni
    const int ALIENS_ROWS = 5; // numero di righe di alieni
    const int ALIENS_COLS = 11; // numero di colonne di alieni
    const float DIST_ALIENS_X = 20; // distanza tra un'alieno ed un'altro in pixel
    const float DIST_ALIENS_Y = 20; // distanza tra un'alieno ed un'altro in pixel
    const int Y_START_ALIENS = 100; // distanza iniziale tra gli alieni e il limite superiore dello schermo
    const int X_START_ALIENS = 100; // distanza iniziale tra gli alieni e il limite sinistro dello schermo

    // Alieni
    const float ALIENS_SPEED_DELAY = 600.0f; // delay iniziale dello spostamento degli alieni
    const float ALIENS_BULLET_SPEED_DELAY = 20.0f; // ogni quanti millisecondi i proiettili si muovono
    const int ALIENS_Y_SPEED = 20; // velocità di movimento degli alieni in verticale
    const int ALIEN_SHOOT_PERC = 40; // Probabilità iniziale con cui ad ogni movimento degli alieni uno a caso di loro spari
    const int ALIENS_X_BARRIER = 100; // limita la libertà di movimento degli alieni dell'asse x
    const int SPECIAL_ALIEN_SPAWN_DELAY = 10; // Ogni quanto appare l'alieno speciale (in secondi)
    const int SPECIAL_ALIEN_SPEED = 10; // Velocità di movimento dell'alieno speciale
    const float SPECIAL_ALIEN_SPEED_DELAY = 80.0f; // ogni quanti millisecondi l'alieno speciale si muove

    // Navicella
    const float SHIP_SPEED_DELAY = 10.0f; // ogni quanti millisecondi la navicella si può muovere
    const float Y_SHIP = 9.0f / 10.0f; // Posizione della navicella rispetto alla finestra

    // Proiettile
    const float BULLET_SPEED_DELAY = 10.0f; // ogni quanti millisecondi i proiettili si muovono

    // Altro
    const int MAX_LETTERS_NAME = 12; // pixel di distanza tra score e il bordo della finestra
    const std::string ALIEN_IMAGE_PATHS[ALIENS_ROWS] = {
    "assets/images/alien1.png",
    "assets/images/alien2.png",
    "assets/images/alien2.png",
    "assets/images/alien3.png",
    "assets/images/alien3.png"
    };

}
