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

//Клиент не сидит за игровым столом
const int GAME_TABLE_IS_UNDEFINED = -1;

class LogHandler
{
public:

    LogHandler();

    void Execute(const std::string &filename);

private:
    /// @brief Вспомогательная структура с данными об игровой сессии
    struct GameSessionInfo
    {
    public:
        int GameTableNumber;
        Time StartSessionTime;

        GameSessionInfo() : GameTableNumber(GAME_TABLE_IS_UNDEFINED) {}

        GameSessionInfo(int gameTableNumber, Time startSessionTime) : GameTableNumber(gameTableNumber),
                                                                    StartSessionTime(startSessionTime) {}
    };

    int m_pricePerHour;   // Соимость часа игры в компьютерном клубе
    Time m_workTimeBegin; // Время открытия компьютерного клуба
    Time m_workTimeEnd;   // Время закрытия компьютерного клуба

    std::vector<GameTable> m_gameTables;                           // Список всех игровых столов
    int m_freeGameTablesCount;                                     // Количество незанятых игровых столов
    std::unordered_map<std::string, GameSessionInfo> m_clientInfo; // Отображение: имя клиента -> данные о текущей игровой сессии
    FastQueue m_queueClients;                                      // Очередь клиентов, ожидающих освобождения любого игрового стола

    utils::StoiDecorator stoi_decorator;
    utils::ClientNameParser name_parser;

    // Логика для начала игровой сессии клиента
    bool startGameSession(const std::string &currentClient, int currentGameTable, const Time &currentTime);
    // Логика для завершения игровой сессии клиента
    bool endGameSession(const std::string &currentClient, const Time &currentTime);

    // Определение типа события, к которому относится строка лога
    void handleEvent(const std::vector<std::string> &tokens);
    // Обработка события ID 1. Клиент пришёл
    void handleClientHasCome(const std::string &currentClient, const Time &currentTime);
    // Обработка события ID 2. Клиент сел за стол
    void handleClientTakeGameTable(const std::string &currentClient, const Time &currentTime, int currentGameTable);
    // Обработка события ID 3. Клиент ожидает
    void handleClientIsWaiting(const std::string &currentClient, const Time &currentTime);
    // Обработка событияID 4. Клиент ушёл
    void handleClientHasLeft(const std::string &currentClient, const Time &currentTime);
};