#pragma once

#include <iostream>

namespace utils
{
    class StoiDecorator
    {
    public:
        int operator()(const std::string &str);
    };
    class ClientNameParser
    {
    public:
        std::string operator()(const std::string &);
    };
}
