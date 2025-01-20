#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Time.hpp"
#include "utils.hpp"

//Маркер конца заголовка лога
const int LOG_HEADER_END = 4;
//Минимальное кол-во аргументов строки в теле лога
const int MIN_LOG_LINE_ARGUMENTS = 3;
//Максимальное кол-во аргументов строки в теле лога
const int MAX_LOG_LINE_ARGUMENTS = 4;

/// @brief Функтор для проверки всего лога на соотвествие формату, установленному в ТЗ
class LogParser
{
public:
    int operator()(const std::string &filepath) const;
};