#include "LogHandler.hpp"

LogHandler::LogHandler() = default;

void LogHandler::Execute(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("File could not be opened");

    std::string bufferLine;

    // Получение количества игровых столов в компьютерном клубе
    std::getline(file, bufferLine);
    m_freeGameTablesCount = stoi_decorator(bufferLine);
    m_gameTables.resize(m_freeGameTablesCount + 1, GameTable());

    // Получение времени открытия и закрытия компьютерного клуба
    std::getline(file, bufferLine);
    std::istringstream iss(bufferLine);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token)
        tokens.push_back(token);
    m_workTimeBegin.SetTime(tokens[0]);
    m_workTimeEnd.SetTime(tokens[1]);

    // Получение стоимости за час игры в компьютерном клубе
    std::getline(file, bufferLine);
    m_pricePerHour = stoi_decorator(bufferLine);

    std::cout << m_workTimeBegin.GetString() << std::endl;

    // Обработка событий в теле лога
    while (std::getline(file, bufferLine))
    {
        std::cout << bufferLine << std::endl;
        tokens.clear();
        iss.str(bufferLine);
        iss.clear();

        while (iss >> token)
            tokens.push_back(token);

        handleEvent(tokens);
    }

    // Клиенты, которые не покинули компьютерный клуб к моменту его закрытия
    std::vector<std::string> lastClients;
    // Получение списка оставшихся клиентов и завершение всех игровых сессий
    for (const auto &lastClient : m_clientInfo)
    {
        lastClients.push_back(lastClient.first);
        endGameSession(lastClient.first, m_workTimeEnd);
    }

    std::sort(lastClients.begin(), lastClients.end());
    for (const auto client : lastClients)
    {
        std::cout << m_workTimeEnd.GetString() + " " << OutgoingEventID::ClientHasLeftForced << " " + client << std::endl;
    }
    std::cout << m_workTimeEnd.GetString() << std::endl;
    // Вывод информации по всем игровым столам по завершению рабочего дня
    for (size_t i = 1; i < m_gameTables.size(); i++)
    {
        std::cout << i << " " << m_gameTables[i].GetString() << std::endl;
    }
}

bool LogHandler::startGameSession(const std::string &currentClient, int currentGameTable, const Time &currentTime)
{
    if (currentGameTable == GAME_TABLE_IS_UNDEFINED)
        return false;

    m_clientInfo[currentClient].GameTableNumber = currentGameTable;
    m_clientInfo[currentClient].StartSessionTime = currentTime;
    m_gameTables[currentGameTable].SetBusy(true);
    m_freeGameTablesCount--;

    return true;
}

bool LogHandler::endGameSession(const std::string &currentClient, const Time &currentTime)
{
    int currentGameTable = m_clientInfo[currentClient].GameTableNumber;

    if (currentGameTable == GAME_TABLE_IS_UNDEFINED)
        return false;

    Time deltaTime = currentTime - m_clientInfo[currentClient].StartSessionTime;
    int profit = (deltaTime.GetHours() + (deltaTime.GetMinutes() > 0)) * m_pricePerHour;
    m_gameTables[currentGameTable].AddDeltaTime(deltaTime);
    m_gameTables[currentGameTable].AddProfit(profit);

    m_clientInfo[currentClient].GameTableNumber = GAME_TABLE_IS_UNDEFINED;
    m_clientInfo[currentClient].StartSessionTime = Time("00:00");
    m_gameTables[currentGameTable].SetBusy(false);
    m_freeGameTablesCount++;

    return true;
}

void LogHandler::handleEvent(const std::vector<std::string> &tokens)
{
    Time currentTime;
    currentTime.SetTime(tokens[0]);
    int currentEvent = stoi_decorator(tokens[1]);
    std::string currentClient = name_parser(tokens[2]);
    int currentGameTable = GAME_TABLE_IS_UNDEFINED;

    switch (currentEvent)
    {
    case IncomingEventID::ClientHasCome:
        handleClientHasCome(currentClient, currentTime);
        break;
    case IncomingEventID::ClientTakeGameTable:
        currentGameTable = stoi_decorator(tokens[3]);
        handleClientTakeGameTable(currentClient, currentTime, currentGameTable);
        break;
    case IncomingEventID::ClientIsWaiting:
        handleClientIsWaiting(currentClient, currentTime);
        break;
    case IncomingEventID::ClientHasLeft:
        handleClientHasLeft(currentClient, currentTime);
        break;
    default:
        throw std::invalid_argument("The event does not exist");
    }
}

void LogHandler::handleClientHasCome(const std::string &currentClient, const Time &currentTime)
{
    if (currentTime < m_workTimeBegin || m_workTimeEnd < currentTime)
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " NotOpenYet" << std::endl;
        return;
    }
    if (m_clientInfo.find(currentClient) != m_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " YouShallNotPass" << std::endl;
        return;
    }
    m_clientInfo[currentClient].GameTableNumber = GAME_TABLE_IS_UNDEFINED;
}

void LogHandler::handleClientTakeGameTable(const std::string &currentClient,
                                           const Time &currentTime,
                                           int currentGameTable)
{
    // В ТЗ это явно не прописано, поэтому при попытке клиента занять несуществующий стол будем писать в логе такую ошибку
    if (currentGameTable <= 0 || currentGameTable >= m_gameTables.size())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " TableUnknown" << std::endl;
        return;
    }
    if (m_clientInfo.find(currentClient) == m_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ClientUnknown" << std::endl;
        return;
    }
    if (m_gameTables[currentGameTable].IsBusy())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " PlaceIsBusy" << std::endl;
        return;
    }
    // Закончили предыдущую игровую сессию, если таковая была
    endGameSession(currentClient, currentTime);

    startGameSession(currentClient, currentGameTable, currentTime);
}

void LogHandler::handleClientIsWaiting(const std::string &currentClient, const Time &currentTime)
{
    // В ТЗ это явно не прописано, но могу предположить, что если клиент не вошёл в клуб, то и ожидать он не может
    if (m_clientInfo.find(currentClient) == m_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ClientUnknown" << std::endl;
        return;
    }
    // Опять же в ТЗ не прописано, как должна вести себя программа, если клиент уже занял стол, поэтому я добавил обработку этого условия сюда
    if (m_freeGameTablesCount > 0 || m_clientInfo[currentClient].GameTableNumber != GAME_TABLE_IS_UNDEFINED)
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ICanWaitNoLonger!" << std::endl;
        return;
    }
    // Здесь используется знак '>=' из-за того, что размер gameTables на 1 больше фактического кол-ва столов, чтобы избежать 0-индексацию в коде
    if (m_queueClients.size() >= m_gameTables.size())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::ClientHasLeftForced << " " + currentClient << std::endl;
        m_clientInfo.erase(currentClient);
        return;
    }
    m_queueClients.push(currentClient);
}

void LogHandler::handleClientHasLeft(const std::string &currentClient, const Time &currentTime)
{
    if (m_clientInfo.find(currentClient) == m_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ClientUnknown" << std::endl;
        return;
    }

    int currentGameTable = m_clientInfo[currentClient].GameTableNumber;

    endGameSession(currentClient, currentTime);
    m_clientInfo.erase(currentClient);
    m_queueClients.remove(currentClient);

    if (m_queueClients.size())
    {
        std::string nextClient = m_queueClients.front();
        m_queueClients.pop();

        if (startGameSession(nextClient, currentGameTable, currentTime))
            std::cout << currentTime.GetString() + " " << OutgoingEventID::ClientFromQueueTakeGameTable
                      << " " + nextClient + " " + std::to_string(currentGameTable) << std::endl;
    }
}