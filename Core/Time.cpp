#include "Time.hpp"

Time::Time() = default;

Time::Time(int hours, int minutes) : m_hours(hours), m_minutes(minutes) {}

Time::Time(const std::string &data) { StringToTime(data); }

int Time::GetHours() const { return m_hours; };

int Time::GetMinutes() const { return m_minutes; };

std::string Time::GetString() const
{
    return (m_hours < 10 ? "0" : "") + std::to_string(m_hours) + ":" +
           (m_minutes < 10 ? "0" : "") + std::to_string(m_minutes);
}

bool Time::operator<(const Time &rightOperand) const
{
    return m_hours * 60 + m_minutes < rightOperand.m_hours * 60 + rightOperand.m_minutes;
}

Time Time::operator+(const Time &rightOperand) const
{
    int totalMinutes = (m_hours * 60 + m_minutes) + (rightOperand.m_hours * 60 + rightOperand.m_minutes);
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return Time(hours, minutes);
}

Time Time::operator-(const Time &rightOperand) const
{
    int totalMinutes = (m_hours * 60 + m_minutes) - (rightOperand.m_hours * 60 + rightOperand.m_minutes);
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return Time(hours, minutes);
}

void Time::StringToTime(const std::string &data)
{
    if (data.length() != 5 || data[2] != ':')
        throw std::runtime_error("Can`t be converted from string to Time");
    utils::StoiDecorator safeStoi;
    m_hours = safeStoi(data.substr(0, 2));
    if (m_hours < 0 || m_hours > 23)
        throw std::runtime_error("Hours must be in the range from 0 to 23");
    m_minutes = safeStoi(data.substr(3, 2));
    if (m_minutes < 0 || m_minutes > 59)
        throw std::runtime_error("Minutes must be in the range from 0 to 59");
}