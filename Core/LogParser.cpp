#include "LogParser.hpp"

int LogParser::operator()(const std::string &filepath) const
{
    std::ifstream file(filepath);
    int logLinesCounter = 0;
    Time workTimeBegin;
    Time workTimeEnd;
    utils::StoiDecorator stoi_decorator;
    utils::ClientNameParser name_parser;
    try
    {
        if (!file.is_open())
            throw std::invalid_argument("File could not be opened");

        std::string line;

        // Проверка корректности строки с числом игровых столов
        std::getline(file, line);
        logLinesCounter++;
        stoi_decorator(line);

        // Проверка корректности строки со временем открытия и закрытия компьютерного клуба
        std::getline(file, line);
        logLinesCounter++;
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (iss >> token)
            tokens.push_back(token);
        if (tokens.size() != 2)
            throw std::runtime_error("Incorrect log line format");
        workTimeBegin.SetTime(tokens[0]);
        workTimeEnd.SetTime(tokens[1]);
        if (workTimeEnd < workTimeBegin)
            throw std::runtime_error("WorkTimeEnd can't be less than workTimeBegin");

        // Проверка корректности строки со стоимостью за час игры в компьютерном клубе
        std::getline(file, line);
        logLinesCounter++;
        stoi_decorator(line);

        // Проверка корректности остальных строк лога
        Time previousTime = workTimeBegin;
        while (std::getline(file, line))
        {
            logLinesCounter++;
            tokens.clear();
            iss.str(line);
            iss.clear();

            while (iss >> token)
                tokens.push_back(token);

            // Проверка корректности количества аргументов строки лога
            if (tokens.size() < MIN_LOG_LINE_ARGUMENTS || tokens.size() > MAX_LOG_LINE_ARGUMENTS)
                throw std::runtime_error("Incorrect log line format");

            // Строки должны быть отсортированы по времени
            Time currentTime(tokens[0]);
            if (logLinesCounter > LOG_HEADER_END && currentTime < previousTime)
                throw std::runtime_error("Unsorted lines in the log");

            // Проверка корректности входящего события
            int currentEvent = stoi_decorator(tokens[1]);
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
                // В случае этого события нужно дополнительно проверить корректность номера занимаемого стола
                stoi_decorator(tokens[3]);
                break;
            default:
                throw std::runtime_error("Unknown incoming event");
                break;
            }
            // Проверка корректности имени клиента
            std::string currentClient = name_parser(tokens[2]);

            previousTime = currentTime;
        }
        file.close();
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "Line " + std::to_string(logLinesCounter) + ": " + e.what();
        return 1;
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}