#include "Time.hpp"

Time::Time() = default;

Time::Time(int hours, int minutes) : p_hours(hours), p_minutes(minutes) {}

int Time::GetHours() const { return p_hours; };

int Time::GetMinutes() const { return p_minutes; };

std::string Time::GetString() const
{
    return (p_hours < 10 ? "0" : "") + std::to_string(p_hours) + ":" +
           (p_minutes < 10 ? "0" : "") + std::to_string(p_minutes);
}

bool Time::operator<(const Time &rightOperand) const
{
    return p_hours * 60 + p_minutes < rightOperand.p_hours * 60 + rightOperand.p_minutes;
}

Time Time::operator+(const Time &rightOperand) const
{
    int totalMinutes = (p_hours * 60 + p_minutes) + (rightOperand.p_hours * 60 + rightOperand.p_minutes);
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return Time(hours, minutes);
}

Time Time::operator-(const Time &rightOperand) const
{
    int totalMinutes = (p_hours * 60 + p_minutes) - (rightOperand.p_hours * 60 + rightOperand.p_minutes);
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return Time(hours, minutes);
}

void Time::StringToTime(const std::string &data)
{
    if (data.length() != 5 || data[2] != ':')
        throw std::invalid_argument("Can`t be converted from string to Time");
    utils::StoiDecorator safeStoi;
    p_hours = safeStoi(data.substr(0, 2));
    if (p_hours < 0 || p_hours > 23)
        throw std::invalid_argument("Hours must be in the range from 0 to 23");
    p_minutes = safeStoi(data.substr(3, 2));
    if (p_minutes < 0 || p_minutes > 59)
        throw std::invalid_argument("Minutes must be in the range from 0 to 59");
}