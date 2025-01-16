#pragma once

#include <iostream>

#include "Time.hpp"

class GameTable
{
public:
    GameTable();
    void AddDeltaTime(const Time &time);
    void AddProfit(int profit);
    bool IsBusy() const;
    void SetBusy(bool isBusy);
    std::string GetString() const;

private:
    bool m_isBusy = false;
    int m_profit = 0;
    Time m_busyTime;
};
