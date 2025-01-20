#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Time.hpp"
#include "utils.hpp"

/// @brief Функтор для проверки всего лога на соотвествие формату, установленному в ТЗ
class LogParser
{
public:
    int operator()(const std::string &filepath) const;
};