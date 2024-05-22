#include "RecordsUtils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

std::vector<Record> readRecordsFromJSON(const std::string& filename)
{
    std::vector<Record> records;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file." << endl;
        return records;
    }

    std::string line;
    std::string jsonStr;
    while (std::getline(file, line)) {
        jsonStr += line;
    }
    file.close();

    // Rimozione degli spazi e dei caratteri di nuova riga
    jsonStr.erase(std::remove_if(jsonStr.begin(), jsonStr.end(), ::isspace), jsonStr.end());

    // Verifica se il JSON inizia e termina correttamente
    if (jsonStr.front() != '{' || jsonStr.back() != '}') {
        std::cerr << "Formato JSON non valido." << endl;
        return records;
    }

    // Rimozione delle parentesi graffe iniziali e finali
    jsonStr = jsonStr.substr(1, jsonStr.size() - 2);

    std::vector<std::string> stringRecords;

    std::stringstream ss(jsonStr);
    std::string item;

    // Separo la stringa in più stringhe ognuna contenente un record
    while (std::getline(ss, item, '}')) {
        if (!item.empty() && item[0] == ',') {
            item.erase(0, 1);
        }
        item += '}';
        stringRecords.push_back(item);
    }

    // Creo un record per ogni stringa creata
    for (const auto& sp : stringRecords) {
        std::stringstream ss(sp);
        std::string token;
        try {
            // Ignora l'ID poiché possiamo estrarlo direttamente dal token
            std::getline(ss, token, ':');
            token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
            int id = std::stoi(token);

            // Estrai playerName
            std::getline(ss, token, ':');
            std::getline(ss, token, ',');
            token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
            std::string playerName = token.substr(0, token.find_last_of('"') - 1);

            // Estrai score
            std::getline(ss, token, ':');
            std::getline(ss, token, ',');
            token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
            int score = std::stoi(token);

            // Estrai time
            std::getline(ss, token, ':');
            std::getline(ss, token, ',');
            token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
            int time = std::stoi(token);

            // Estrai date
            std::getline(ss, token, ':');
            std::getline(ss, token, ',');
            token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
            token.erase(std::remove(token.begin(), token.end(), '}'), token.end());
            std::string date = token.substr(token.find_first_of('"') + 1, token.find_last_of('"') - 1);

            // Crea l'oggetto Record
            Record record = { id, playerName, score, time, date };
            records.push_back(record);
        }
        catch (const std::invalid_argument& e) {
            // Gestisci l'eccezione, ad esempio ignorando questa riga o registrando un messaggio di errore
            cerr << "Errore: La stringa non contiene un numero valido.\n" << e.what() << endl;
        }
    }

    return records;
}

void removeRecord(const std::string& fileName, int id)
{
    // Apertura del file in modalità lettura
    ifstream fileInput(fileName);

    // Verifica se il file è stato aperto correttamente
    if (!fileInput.is_open()) {
        cout << "Impossibile aprire il file." << endl;
    }

    // Lettura del contenuto del file
    string contenuto;
    char carattere;
    while (fileInput.get(carattere)) {
        contenuto += carattere;
    }

    // Chiusura del file di input
    fileInput.close();

    std::string inizio = "\"" + to_string(id)+ "\":";
    std::string fine = "}";

    // Trovo la posizione della sottostringa iniziale
    size_t posInizio = contenuto.find(inizio);
    if (posInizio == std::string::npos) {
        cout << "Sottostringa iniziale non trovata." << endl;
    }

    // Trovo la posizione della sottostringa finale a partire dalla posizione della sottostringa iniziale
    size_t posFine = contenuto.find(fine, posInizio);
    if (posFine == std::string::npos) {
        cout << "Sottostringa finale non trovata." << endl;
    }

    std::string sottostringa = contenuto.substr(posInizio, posFine - posInizio + fine.length());

    // Elimino la sottostringa trovata dal contenuto e anche eventuali altri caratteri in base alla posizione del record nel file
    size_t pos = contenuto.find(sottostringa);
    if (getSize(fileName) == 1) // se c'è un solo record
    {
        contenuto.erase(pos, sottostringa.length());
    }
    else if (pos <= 3)
    {
        contenuto.erase(pos, sottostringa.length() + 3);
    }
    else
    {
        contenuto.erase(pos - 3, sottostringa.length() + 3);
    }

    // Apertura del file in modalità scrittura sovrascrivendo il contenuto
    ofstream fileOutput(fileName);

    // Verifica se il file è stato aperto correttamente
    if (!fileOutput.is_open()) {
        cout << "Impossibile scrivere sul file." << endl;
    }

    // Scrittura del contenuto nel file
    fileOutput << contenuto;

    // Chiusura del file di output
    fileOutput.close();
}

void addRecord(const string& fileName, const string& playerName, int score, int time, const string& date)
{
    int lastId = getLastId(fileName);

    // Apertura del file in modalità lettura
    ifstream fileInput(fileName);

    // Verifica se il file è stato aperto correttamente
    if (!fileInput.is_open()) {
        cout << "Impossibile aprire il file." << endl;
    }

    // Lettura del contenuto del file
    string contenuto;
    char carattere;
    while (fileInput.get(carattere)) {
        contenuto += carattere;
    }

    // Chiusura del file di input
    fileInput.close();

    // Eliminazione dell'ultimo carattere (supponendo che sia '}')
    if (contenuto.size() >= 2) {
        contenuto.resize(contenuto.size() - 2);
    }

    // Aggiunta della stringa alla fine
    if(lastId != 0)
        contenuto += ",\n";
    contenuto += getSize(fileName) == 0 ? "" : "\t";
    contenuto += "\"" + to_string(lastId + 1) + "\": {\n";
    contenuto += "\t\t\"playername\": \"" + playerName + "\",\n";
    contenuto += "\t\t\"score\": \"" + to_string(score )+ "\",\n";
    contenuto += "\t\t\"time\": \"" + to_string(time )+ "\",\n";
    contenuto += "\t\t\"date\": \"" + date + "\"\n";
    contenuto += "\t}\n";
    contenuto += "}";

    // Apertura del file in modalità scrittura sovrascrivendo il contenuto
    ofstream fileOutput(fileName);

    // Verifica se il file è stato aperto correttamente
    if (!fileOutput.is_open()) {
        cout << "Impossibile scrivere sul file." << endl;
    }

    // Scrittura del contenuto nel file
    fileOutput << contenuto;

    // Chiusura del file di output
    fileOutput.close();

}

int getLastId(const string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file." << endl;
    }

    std::string line;
    std::string jsonStr;
    while (std::getline(file, line)) {
        jsonStr += line;
    }
    file.close();

    // Rimozione degli spazi e dei caratteri di nuova riga
    jsonStr.erase(std::remove_if(jsonStr.begin(), jsonStr.end(), ::isspace), jsonStr.end());

    // Verifica se il JSON inizia e termina correttamente
    if (jsonStr.front() != '{' || jsonStr.back() != '}') {
        std::cerr << "Formato JSON non valido." << endl;
    }

    // Rimozione delle parentesi graffe iniziali e finali
    jsonStr = jsonStr.substr(1, jsonStr.size() - 2);

    // Se non ci sono elementi ritorna 0
    if (jsonStr == "")
    {
        return 0;
    }

    std::vector<std::string> stringRecords;

    std::stringstream ss(jsonStr);
    std::string item;

    // Separo la stringa in più stringhe ognuna contenente un record
    while (std::getline(ss, item, '}')) {
        if (!item.empty() && item[0] == ',') {
            item.erase(0, 1);
        }
        item += '}';
        stringRecords.push_back(item);
    }
    std::string lastRecord = stringRecords[stringRecords.size() - 1];
    size_t pos = lastRecord.find(':');
    std::string stringLastId = lastRecord.substr(0, pos);
    stringLastId.erase(std::remove(stringLastId.begin(), stringLastId.end(), '\"'), stringLastId.end());

    return stoi(stringLastId);
}

int getSize(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << fileName << endl;
        return -1;
    }

    int playerCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        for (char c : line) {
            if (c == '{') {
                ++playerCount;
            }
        }
    }

    file.close();

    // Tolgo uno perchè conta anche la parentesi iniziale
    return playerCount - 1;
}
