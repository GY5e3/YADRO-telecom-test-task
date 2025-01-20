#pragma once

#include <iostream>

#include "utils.hpp"

class Time
{
public:
    Time();
    Time(int hours, int minutes);
    Time(const std::string &data);

    bool operator<(const Time &rightOperand) const;
    Time operator+(const Time &rightOperand) const;
    Time operator-(const Time &rightOperand) const;
    void SetTime(const std::string &data);

    std::string GetString() const;
    int GetHours() const;
    int GetMinutes() const;

private:
    int m_hours = 0;
    int m_minutes = 0;
};
