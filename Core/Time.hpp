#pragma once

#include <iostream>

#include "utils.hpp"

class Time
{
public:
    Time();
    Time(int hours, int minutes);

    bool operator<(const Time &rightOperand) const;
    Time operator+(const Time &rightOperand) const;
    Time operator-(const Time &rightOperand) const;
    void StringToTime(const std::string &);

    std::string GetString() const;
    int GetHours() const;
    int GetMinutes() const;

private:
    int p_hours = 0;
    int p_minutes = 0;
};
