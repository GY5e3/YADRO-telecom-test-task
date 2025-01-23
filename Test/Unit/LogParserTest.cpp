#include "LogParserTest.hpp"

TEST_F(LogParserTest, File_open_test)
{
    logParser("not_found.txt");

    EXPECT_EQ(m_result.str(), "File could not be opened");
}
TEST_F(LogParserTest, Header_positive)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, Header_first_line)
{
    std::stringstream test;
    test << "1 1" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 1: stoi_decorator");
}
TEST_F(LogParserTest, Header_second_line_1)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 2: Incorrect log line format");
}
TEST_F(LogParserTest, Header_second_line_2)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00 20:00" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 2: Incorrect log line format");
}
TEST_F(LogParserTest, Header_second_line_3)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "18:00 10:00" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 2: WorkTimeEnd can't be less than workTimeBegin");
}
TEST_F(LogParserTest, Header_second_line_4)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 10:00" << std::endl;
    test << "3" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, Header_third_line)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3 3" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 3: stoi_decorator");
}
TEST_F(LogParserTest, Body_positive_1)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "10:01 1 usr_me";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, Body_positive_2)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "10:01 2 usr_me 4";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, Body_wrong_format_1)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "10:01 3";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: Incorrect log line format");
}
TEST_F(LogParserTest, Body_wrong_format_2)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "10:01 4 usr .";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: Incorrect log line format");
}
TEST_F(LogParserTest, Body_wrong_format_3)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "10:01 2 usr";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: Incorrect log line format");
}
TEST_F(LogParserTest, Body_wrong_format_4)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "10:01 2 usr 4 .";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: Incorrect log line format");
}
TEST_F(LogParserTest, Body_wrong_format_5)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "2 10:01 usr 4";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: Can`t be converted from string to Time");
}
TEST_F(LogParserTest, Unknown_event)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "10:01 5 usr";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: Unknown incoming event");
}
TEST_F(LogParserTest, Unsorted_lines_positive)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "09:00 1 usr";
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, Unsorted_lines)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "11:00 4 usr1"<< std::endl;
    test << "10:59 4 usr2"<< std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 5: Unsorted lines in the log");
}
TEST_F(LogParserTest, Unsorted_lines_edge_case)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "11:00 4 usr1" << std::endl;
    test << "11:00 4 usr2" << std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, event_2_in_nonworking_hour)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "19:00 2 usr1"<< std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: This type of event cannot happen in non-working hours");
}
TEST_F(LogParserTest, event_3_in_nonworking_hour)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "19:00 3 usr1"<< std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: This type of event cannot happen in non-working hours");
}
TEST_F(LogParserTest, event_4_in_nonworking_hour)
{
    std::stringstream test;
    test << "1" << std::endl;
    test << "10:00 18:00" << std::endl;
    test << "3" << std::endl;
    test << "19:00 4 usr1"<< std::endl;
    m_testFile.open(FILENAME);
    m_testFile << test.str();
    m_testFile.close();

    logParser(FILENAME);

    EXPECT_EQ(m_result.str(), "Line 4: This type of event cannot happen in non-working hours");
}