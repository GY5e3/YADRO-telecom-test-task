#include "GameTable.hpp"

GameTable::GameTable() : p_profit(0),
                         p_isBusy(false),
                         p_client("") {}

std::string GameTable::GetString() const
{
    return std::to_string(p_profit) + " " + p_busyTime.GetString();
}
void GameTable::AddDeltaTime(const Time &deltaTime)
{
    p_busyTime = p_busyTime + deltaTime;
}
void GameTable::AddProfit(int profit)
{
    p_profit += profit;
}
bool GameTable::IsBusy() const
{
    return p_isBusy;
}
void GameTable::SetBusy(bool isBusy)
{
    p_isBusy = isBusy;
}