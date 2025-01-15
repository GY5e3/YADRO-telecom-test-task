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
    bool p_isBusy = false;
    int p_profit;
    Time p_busyTime;
};
