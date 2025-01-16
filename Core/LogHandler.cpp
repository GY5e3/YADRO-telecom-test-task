#include "LogHandler.hpp"

LogHandler::LogHandler() : p_file("") {}

LogHandler::LogHandler(const std::string &filename) : p_file(filename)
{
    try
    {
        if (!p_file.is_open())
            throw std::runtime_error("File could not be opened");
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }
}
int LogHandler::Execute()
{
    try
    {
        parseHeader();

        std::cout << p_workTimeBegin.GetString() << std::endl;

        parseBody();

        std::vector<std::string> lastClients;
        for (const auto &lastClient : p_clientInfo)
        {
            lastClients.push_back(lastClient.first);
            if (p_clientInfo[lastClient.first].gameTableNumber != GAME_TABLE_IS_UNDEFINED)
                endGameSession(lastClient.first, p_workTimeEnd);

            p_queueClients.remove(lastClient.first);
        }
        while (p_queueClients.size() > 0)
        {
            lastClients.push_back(p_queueClients.front());
            p_queueClients.pop();
        }
        std::sort(lastClients.begin(), lastClients.end());

        for (const auto i : lastClients)
        {
            std::cout << p_workTimeEnd.GetString() + " " << OutgoingEventID::ClientHasLeftForced << " " + i << std::endl;
        }
        std::cout << p_workTimeEnd.GetString() << std::endl;
        for (size_t i = 0; i < p_gameTables.size(); i++)
        {
            std::cout << i + 1 << " " << p_gameTables[i].GetString() << std::endl;
        }
    }
    catch (const std::invalid_argument &e)
    {
        // std::cout << "Line " + std::to_string(p_logLinesCounter) + ": " + e.what() << std::endl;
        return 1;
    }

    return 0;
}

void LogHandler::startGameSession(const std::string &currentClient, int currentGameTable, const Time &currentTime)
{
    p_clientInfo[currentClient].gameTableNumber = currentGameTable;
    p_clientInfo[currentClient].startSessionTime = currentTime;
    p_gameTables[currentGameTable].SetBusy(true);
    p_freeGameTablesCount--;
}

void LogHandler::endGameSession(const std::string &currentClient, const Time &currentTime)
{
    int index = p_clientInfo[currentClient].gameTableNumber;

    Time deltaTime = currentTime - p_clientInfo[currentClient].startSessionTime;
    int profit = (deltaTime.GetHours() + (deltaTime.GetMinutes() > 0)) * m_pricePerHour;

    p_gameTables[index].AddDeltaTime(deltaTime);
    p_gameTables[index].AddProfit(profit);

    p_gameTables[index].SetBusy(false);
    p_clientInfo[currentClient].gameTableNumber = GAME_TABLE_IS_UNDEFINED;
    p_freeGameTablesCount++;
}
void LogHandler::parseHeader()
{
    std::string line;

    // Получение количества игровых столов в компьютерном клубе
    std::getline(p_file, line);
    p_freeGameTablesCount = stoi_decorator(line);
    p_gameTables.resize(p_freeGameTablesCount, GameTable());

    // Получение времени открытия и закрытия компьютерного клуба
    std::getline(p_file, line);
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token)
        tokens.push_back(token);
    p_workTimeBegin.StringToTime(tokens[0]);
    p_workTimeEnd.StringToTime(tokens[1]);

    // Получение стоимости за час игры в компьютерном клубе
    std::getline(p_file, line);
    m_pricePerHour = stoi_decorator(line);
}
// Обработка тела лога
void LogHandler::parseBody()
{
    std::string line;
    std::vector<std::string> tokens;
    std::istringstream iss;
    std::string token;

    Time currentTime = p_workTimeBegin;
    while (std::getline(p_file, line))
    {
        //  p_logLinesCounter++;
        std::cout << line << std::endl;
        tokens.clear();
        iss.str(line);
        iss.clear();

        while (iss >> token)
            tokens.push_back(token);

        handleEvent(tokens, currentTime);
    }
}

// Определения типа события, к которому относится строка лога
void LogHandler::handleEvent(const std::vector<std::string> &tokens, Time &previousTime)
{
    Time currentTime;
    currentTime.StringToTime(tokens[0]);
    int currentEvent = stoi_decorator(tokens[1]);
    std::string currentClient = name_parser(tokens[2]);
    int currentGameTable = GAME_TABLE_IS_UNDEFINED;

    switch (currentEvent)
    {
    case IncomingEventID::ClientHasCome:
        handleClientHasCome(currentClient, currentTime);
        break;
    case IncomingEventID::ClientTakeGameTable:
        handleClientTakeGameTable(tokens, currentClient, currentTime);
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
    previousTime = currentTime;
}

// ID 1. Клиент пришёл
void LogHandler::handleClientHasCome(const std::string &currentClient, Time &currentTime)
{
    if (currentTime < p_workTimeBegin || p_workTimeEnd < currentTime)
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " NotOpenYet" << std::endl;
        return;
    }
    if (p_clientInfo.find(currentClient) != p_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " YouShallNotPass" << std::endl;
        return;
    }
    p_clientInfo[currentClient].gameTableNumber = GAME_TABLE_IS_UNDEFINED;
}

// ID 2. Клиент сел за стол
void LogHandler::handleClientTakeGameTable(const std::vector<std::string> &tokens,
                                           const std::string &currentClient,
                                           Time &currentTime)
{
    int currentGameTable = stoi_decorator(tokens[3]) - 1;
    // В ТЗ это явно не прописано, поэтому при попытке клиента занять несуществующий стол будем писать в логе такую ошибку
    if (currentGameTable < 0 || currentGameTable >= p_gameTables.size())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " TableUnknown" << std::endl;
        return;
    }
    if (p_clientInfo.find(currentClient) == p_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ClientUnknown" << std::endl;
        return;
    }
    if (p_gameTables[currentGameTable].IsBusy())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " PlaceIsBusy" << std::endl;
        return;
    }
    // Закончили предыдущую игровую сессию, если таковая была
    if (p_clientInfo[currentClient].gameTableNumber != GAME_TABLE_IS_UNDEFINED)
        endGameSession(currentClient, currentTime);

    startGameSession(currentClient, currentGameTable, currentTime);
}

// ID 3. Клиент ожидает
void LogHandler::handleClientIsWaiting(const std::string &currentClient, Time &currentTime)
{
    // В ТЗ это явно не прописано, но могу предположить, что если клиент не вошёл в клуб, то и ожидать он не может
    if (p_clientInfo.find(currentClient) == p_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ClientUnknown" << std::endl;
        return;
    }
    // Опять же в ТЗ не прописано, как должна вести себя программа, если клиент уже занял стол, поэтому я добавил обработку этого условия сюда
    if (p_freeGameTablesCount > 0 || p_clientInfo[currentClient].gameTableNumber != GAME_TABLE_IS_UNDEFINED)
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ICanWaitNoLonger!" << std::endl;
        return;
    }
    if (p_queueClients.size() > p_gameTables.size())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::ClientHasLeftForced << " " + currentClient << std::endl;
        p_clientInfo.erase(currentClient);
        return;
    }
    p_queueClients.push(currentClient);
}

// ID 4. Клиент ушёл
void LogHandler::handleClientHasLeft(const std::string &currentClient, Time &currentTime)
{
    if (p_clientInfo.find(currentClient) == p_clientInfo.end())
    {
        std::cout << currentTime.GetString() + " " << OutgoingEventID::EventError << " ClientUnknown" << std::endl;
        return;
    }

    int currentGameTable = p_clientInfo[currentClient].gameTableNumber;

    if (currentGameTable != GAME_TABLE_IS_UNDEFINED)
    {
        endGameSession(currentClient, currentTime);
        if (!p_queueClients.empty())
        {
            std::string nextClient = p_queueClients.front();
            p_queueClients.pop();

            startGameSession(nextClient, currentGameTable, currentTime);
            std::cout << currentTime.GetString() + " " << OutgoingEventID::ClientFromQueueTakeGameTable << " " + nextClient + " " + std::to_string(currentGameTable + 1) << std::endl;
        }
    }
    p_clientInfo.erase(currentClient);
    p_queueClients.remove(currentClient);
}