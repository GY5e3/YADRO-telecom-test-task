#pragma once

#include <iostream>

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
