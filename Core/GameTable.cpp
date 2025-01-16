#include "GameTable.hpp"

GameTable::GameTable() = default;

std::string GameTable::GetString() const
{
    return std::to_string(m_profit) + " " + m_busyTime.GetString();
}
void GameTable::AddDeltaTime(const Time &deltaTime)
{
    m_busyTime = m_busyTime + deltaTime;
}
void GameTable::AddProfit(int profit)
{
    m_profit += profit;
}
bool GameTable::IsBusy() const
{
    return m_isBusy;
}
void GameTable::SetBusy(bool isBusy)
{
    m_isBusy = isBusy;
}