#include <iostream>
#include <unordered_map>
#include <list>
/**
 * Эта структура данных нужна для того, чтобы за константное время удалять из очереди клиента, 
 * который решил уйти до того, как занял игровой стол
*/
class FastQueue
{
public:
    void push(const std::string &value);

    void remove(const std::string &value);

    void pop();

    std::string front() const;

    bool empty() const;

    int size() const;

private:
    std::list<std::string> m_list;                                                    
    std::unordered_map<std::string, std::list<std::string>::iterator> m_position; 
};