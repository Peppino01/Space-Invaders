#include "SettingsUtils.hpp"

#include <vector>
#include <iostream>
#include <sstream>
#include "SFML/Graphics.hpp"

using namespace std;

std::vector<std::pair<std::string, int>> readSettingsFromFile(const std::string& filename) {
    std::vector<std::pair<std::string, int>> settings;
    std::vector<std::string> keys;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file " << filename << endl;
        return settings; // Restituisce un vettore vuoto in caso di errore
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Estrazione dei valori dal JSON
    buffer.seekg(0);
    std::string line;
    while (std::getline(buffer, line)) {
        if (line.find(':') != std::string::npos) {
            // Trova le coppie chiave-valore nel formato "chiave": valore
            size_t colonPos = line.find(':');
            std::string key = line.substr(0, colonPos);
            std::string valueStr = line.substr(colonPos + 1);
            key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
            key.erase(std::remove(key.begin(), key.end(), '\t'), key.end());
            int value = std::stoi(valueStr);
            settings.emplace_back(key, value);
            keys.push_back(key); // Memorizza l'ordine delle chiavi
        }
    }

    return settings;
}

void writeSettingsToFile(const std::string& filename, const std::vector<std::pair<std::string, int>>& settings) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file " << filename << " per la scrittura" << endl;
        return;
    }

    // Scrive le impostazioni nel formato JSON
    file << "{\n";
    for (size_t i = 0; i < settings.size(); ++i) {
        file << "\t\"" << settings[i].first << "\": " << settings[i].second;
        if (i < settings.size() - 1) {
            file << ",";
        }
        file << "\n";
    }
    file << "}";
    file.close();
}
