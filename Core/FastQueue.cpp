#include "FastQueue.hpp"

void FastQueue::push(const std::string &value)
{
    m_position[value] = m_list.insert(m_list.end(), value);
}

void FastQueue::remove(const std::string &value)
{
    auto it = m_position.find(value);
    if (m_position.find(value) != m_position.end())
    {
        m_list.erase(it->second);
        m_position.erase(it);
    }
}

void FastQueue::pop()
{
    if (!m_list.empty())
    {
        auto it = m_list.begin();
        m_position.erase(*it);
        m_list.erase(it);
    }
}

std::string FastQueue::front() const
{
    if (!m_list.empty())
    {
        return m_list.front();
    }
    throw std::runtime_error("Queue is empty");
}

bool FastQueue::empty() const
{
    return m_list.empty();
}

int FastQueue::size() const
{
    return m_position.size();
}