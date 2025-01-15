#pragma once

#include <iostream>

namespace utils
{
    class StoiDecorator
    {
    public:
        int operator()(const std::string &str);
    };
    /// @brief Функтор для проверки корректности имени клиента
    class ClientNameParser
    {
    public:
        std::string operator()(const std::string &);
    };
}
