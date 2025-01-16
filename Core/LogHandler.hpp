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

    void Execute(const std::string &filename);

private:
    struct ClientSession
    {
    public:
        int GameTableNumber;
        Time StartSessionTime;

        ClientSession() : GameTableNumber(GAME_TABLE_IS_UNDEFINED) {}

        ClientSession(int gameTableNumber, Time startSessionTime) : GameTableNumber(gameTableNumber),
                                                                    StartSessionTime(startSessionTime) {}
    };

    int m_pricePerHour;
    Time m_workTimeBegin; // Время открытия компьютерного клуба
    Time m_workTimeEnd;   // Время закрытия компьютерного клуба

    std::vector<GameTable> m_gameTables;                         // Перечень всех игровых столов
    int m_freeGameTablesCount;                                   // Количество незанятых игровых столов
    std::unordered_map<std::string, ClientSession> m_clientInfo; // Отображение для клиента и данных о его игровой сессии
    FastQueue m_queueClients;                                    // Очередь клиентов, ожидающих освобождения любого игрового стола

    utils::StoiDecorator stoi_decorator;
    utils::ClientNameParser name_parser;

    void startGameSession(const std::string &currentClient, int currentGameTable, const Time &currentTime);
    void endGameSession(const std::string &currentClient, const Time &currentTime);
    
    void handleEvent(const std::vector<std::string> &tokens);
    void handleClientHasCome(const std::string &currentClient, const Time &currentTime);
    void handleClientTakeGameTable(const std::string &currentClient, const Time &currentTime, int currentGameTable);
    void handleClientIsWaiting(const std::string &currentClient, const Time &currentTime);
    void handleClientHasLeft(const std::string &currentClient, const Time &currentTime);
};