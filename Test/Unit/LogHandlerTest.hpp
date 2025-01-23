#pragma once

#include <sstream>
#include <iostream>
#include <streambuf>

#include <gtest/gtest.h>

#include "LogHandler.hpp"

const std::string FILENAME = "test.txt";

class LogParserTest : public testing::Test
{
protected:
    LogHandler logHandler;

    std::streambuf *m_cout;
    std::ostringstream m_result;
    std::ofstream m_testFile;

    void SetUp()
    {
        m_cout = std::cout.rdbuf();
        std::cout.rdbuf(m_result.rdbuf());

        logHandler.m_freeGameTablesCount = 2;
        logHandler.m_gameTables.resize(3);
        logHandler.m_workTimeBegin.SetTime("10:00");
        logHandler.m_workTimeEnd.SetTime("18:00");
        logHandler.m_pricePerHour = 10;
    }
    void TearDown()
    {
        std::cout.rdbuf(m_cout);
    }
    void handleEvent(const std::vector<std::string> &tokens)
    {
        logHandler.handleEvent(tokens);
    }
    void handleClientHasCome(const std::string &currentClient, const Time &currentTime)
    {
        logHandler.handleClientHasCome(currentClient, currentTime);
    }
    void handleClientTakeGameTable(const std::string &currentClient, const Time &currentTime, int currentGameTable)
    {
        logHandler.handleClientTakeGameTable(currentClient, currentTime, currentGameTable);
    }
    void handleClientIsWaiting(const std::string &currentClient, const Time &currentTime)
    {
        logHandler.handleClientIsWaiting(currentClient, currentTime);
    }
    void handleClientHasLeft(const std::string &currentClient, const Time &currentTime)
    {
        logHandler.handleClientHasLeft(currentClient, currentTime);
    }
    std::vector<GameTable> GetGameTables()
    {
        return logHandler.m_gameTables;
    }
    std::unordered_map<std::string, LogHandler::GameSessionInfo> GetClientInfo()
    {
        return logHandler.m_clientInfo;
    }

    FastQueue GetQueueClients()
    {
        return logHandler.m_queueClients;
    }
};