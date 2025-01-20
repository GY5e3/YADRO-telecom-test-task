#pragma once

#include <sstream>
#include <iostream>
#include <vector>
#include <streambuf>
#include <memory>

#include <gtest/gtest.h>

#include "LogParser.hpp"

const std::string FILENAME = "test.txt";

class LogParserTest : public testing::Test
{
protected:
    LogParser logParser;

    std::streambuf* m_cout;
    std::ostringstream m_result;
    std::ofstream m_testFile;

    void SetUp()
    {
        m_cout = std::cout.rdbuf();
        std::cout.rdbuf(m_result.rdbuf());
    }
    void TearDown()
    {
        std::cout.rdbuf(m_cout);
    }
};