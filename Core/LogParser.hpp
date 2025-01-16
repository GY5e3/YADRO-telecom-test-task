#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Time.hpp"
#include "utils.hpp"

class LogParser
{
public:
    int operator()(const std::string &filepath) const;
};