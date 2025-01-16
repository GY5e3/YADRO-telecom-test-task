#pragma once

#include <iostream>

//Клиент не сидит за игровым столом
const int GAME_TABLE_IS_UNDEFINED = -1;
//Маркер конца заголовка лога
const int LOG_HEADER_END = 4;
//Минимальное кол-во аргументов строки в теле лога
const int MIN_LOG_LINE_ARGUMENTS = 3;
//Максимальное кол-во аргументов строки в теле лога
const int MAX_LOG_LINE_ARGUMENTS = 4;

enum IncomingEventID
{
    ClientHasCome = 1,
    ClientTakeGameTable = 2,
    ClientIsWaiting = 3,
    ClientHasLeft = 4,
};
enum OutgoingEventID
{
    ClientHasLeftForced = 11,
    ClientFromQueueTakeGameTable = 12,
    EventError = 13
};

namespace utils
{
    /// @brief Класс-обёртка для std::stoi с дополнительной обработкой строки
    class StoiDecorator
    {
    public:
        int operator()(const std::string &str) const;
    };
    /// @brief Функтор для проверки корректности имени клиента
    class ClientNameParser
    {
    public:
        std::string operator()(const std::string &) const;
    };
}
