#pragma once

#include "SFML/Graphics.hpp"

using namespace std;

struct Record {
    int id;
    std::string playerName;
    int score;
    int time;
    std::string date;
};

// Funzione per leggere il JSON dal file e restituire un vettore di record
std::vector<Record> readRecordsFromJSON(const std::string& filename);

// Funzione per eliminare un record
void removeRecord(const std::string& fileName, int id);

// Funzione per aggiungere un nuovo giocatore al file JSON
void addRecord(const string& nomeFile, const string& playerName, int score, int time, const string& date);

// Funzione per avere l'id dell'ultimo Record (sarà sicuramente il più alto)
int getLastId(const string& nomeFile);

// Ritorna il numero di record attualmente presenti
int getSize(const std::string& fileName);
