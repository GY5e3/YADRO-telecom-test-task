#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <algorithm>

#include "GameTable.hpp"
#include "Time.hpp"
#include "FastQueue.hpp"

class LogHandler
{
public:
    friend class LogParserTest;

    LogHandler();
    LogHandler(const std::string &filename);

    int Execute();

private:
    struct ClientSession
    {
    public:
        int gameTableNumber;
        Time startSessionTime;

        ClientSession() : gameTableNumber(GAME_TABLE_IS_UNDEFINED) {}

        ClientSession(int gameTableNumber, Time startSessionTime) : gameTableNumber(gameTableNumber),
                                                                    startSessionTime(startSessionTime) {}
    };

    std::ifstream p_file;

   // int p_logLinesCounter; // Счётчик строк в логе
    int m_pricePerHour;
    Time p_workTimeBegin; // Время открытия компьютерного клуба
    Time p_workTimeEnd;   // Время закрытия компьютерного клуба

    std::vector<GameTable> p_gameTables;                         // Перечень всех игровых столов
    int p_freeGameTablesCount;                                   // Количество незанятых игровых столов
    std::unordered_map<std::string, ClientSession> p_clientInfo; // Отображение для клиента и данных о его игровой сессии
    FastQueue p_queueClients;                                    // Очередь клиентов, ожидающих освобождения любого игрового стола

    utils::StoiDecorator stoi_decorator;
    utils::ClientNameParser name_parser;

    void parseHeader();
    void parseBody();

    void startGameSession(const std::string &currentClient, int currentGameTable, const Time &currentTime);
    void endGameSession(const std::string &currentClient, const Time &currentTime);
    

    void handleEvent(const std::vector<std::string> &tokens, Time &previousTime);
    void handleClientHasCome(const std::string &currentClient, Time &currentTime);
    void handleClientTakeGameTable(const std::vector<std::string> &tokens, const std::string &currentClient, Time &currentTime);
    void handleClientIsWaiting(const std::string &currentClient, Time &currentTime);
    void handleClientHasLeft(const std::string &currentClient, Time &currentTime);
};