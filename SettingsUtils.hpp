#pragma once

#include <fstream>
#include <vector>
#include <utility>
#include <string>

class Settings
{

public:
	int shipSpeed;
	float shipSize;
	int aliensSpeed;
	float aliensSize;
	int bulletSpeed;
	float bulletSize;
	int aliensSpeedDelay;
	int aliensShootPerc;

};

// Funzione per leggere le impostazioni dal file JSON e restituire un vettore di coppie chiave-valore
std::vector<std::pair<std::string, int>> readSettingsFromFile(const std::string& filename);

// Funzione per scrivere le impostazioni nel file JSON utilizzando un vettore di coppie chiave-valore
void writeSettingsToFile(const std::string& filename, const std::vector<std::pair<std::string, int>>& settings);
