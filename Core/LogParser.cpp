#include "LogParser.hpp"

int LogParser::operator()(const std::string &filepath) const
{
    std::ifstream file(filepath);
    int logLinesCounter = 0; // Счётчик строк в логе
    Time workTimeBegin;      // Время открытия компьютерного клуба
    Time workTimeEnd;
    try
    {
        if (!file.is_open())
            throw std::runtime_error("File could not be opened");

        std::string line;

        // Получение количества игровых столов в компьютерном клубе
        std::getline(file, line);
        logLinesCounter++;
        utils::stoi_decorator(line);

        // Получение времени открытия и закрытия компьютерного клуба
        std::getline(file, line);
        logLinesCounter++;
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (iss >> token)
            tokens.push_back(token);
        if (tokens.size() != 2)
            throw std::runtime_error("Incorrect log line format");
        workTimeBegin.StringToTime(tokens[0]);
        workTimeEnd.StringToTime(tokens[1]);
        if (workTimeEnd < workTimeBegin)
            throw std::runtime_error("WorkTimeEnd can't be less than workTimeBegin");

        // Получение стоимости за час игры в компьютерном клубе
        std::getline(file, line);
        logLinesCounter++;
        utils::stoi_decorator(line);

        Time previousTime = workTimeBegin;
        while (std::getline(file, line))
        {
            logLinesCounter++;
            tokens.clear();
            iss.str(line);
            iss.clear();

            while (iss >> token)
                tokens.push_back(token);

            if (tokens.size() < MIN_LOG_LINE_ARGUMENTS || tokens.size() > MAX_LOG_LINE_ARGUMENTS)
                throw std::runtime_error("Incorrect log line format");

            Time currentTime(tokens[0]);
            if (logLinesCounter > LOG_HEADER_END && currentTime < previousTime)
                throw std::runtime_error("Unsorted lines in the log");

            int currentEvent = utils::stoi_decorator(tokens[1]);
            switch (currentEvent)
            {
            case IncomingEventID::ClientHasCome:
            case IncomingEventID::ClientIsWaiting:
            case IncomingEventID::ClientHasLeft:
                if (tokens.size() != 3)
                    throw std::runtime_error("Incorrect log line format");
                break;
            case IncomingEventID::ClientTakeGameTable:
                if (tokens.size() != 4)
                    throw std::runtime_error("Incorrect log line format");
                utils::stoi_decorator(tokens[3]);
                break;
            default:
                throw std::runtime_error("Unknown incoming event");
                break;
            }

            std::string currentClient = utils::name_parser(tokens[2]);

            previousTime = currentTime;
        }
        file.close();
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "Line " + std::to_string(logLinesCounter) + ": " + e.what() << std::endl;
        return 1;
    }

    return 0;
}