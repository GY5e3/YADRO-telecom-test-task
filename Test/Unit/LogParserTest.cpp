#include "LogParserTest.hpp"

TEST_F(LogParserTest, File_open_test)
{
    logParser("not_found.txt");

    EXPECT_EQ(m_result.str(), "File could not be opened");
}