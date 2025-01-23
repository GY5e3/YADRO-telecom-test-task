#include "LogHandlerTest.hpp"

TEST_F(LogParserTest, handleClientHasCome_positive)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));

    EXPECT_EQ(m_result.str(), "");
    ASSERT_TRUE(GetClientInfo().find("client1") != end(GetClientInfo()));
    EXPECT_TRUE(GetClientInfo()["client1"].GameTableNumber == GAME_TABLE_IS_UNDEFINED);
}
TEST_F(LogParserTest, handleClientHasCome_before_opening)
{
    handleClientHasCome(std::string("client1"), Time("09:00"));

    EXPECT_EQ(m_result.str(), "09:00 13 NotOpenYet\n");
    EXPECT_TRUE(GetClientInfo().find("client1") == end(GetClientInfo()));
}
TEST_F(LogParserTest, handleClientHasCome_after_closing)
{
    handleClientHasCome(std::string("client1"), Time("21:00"));

    EXPECT_EQ(m_result.str(), "21:00 13 NotOpenYet\n");
    EXPECT_TRUE(GetClientInfo().find("client1") == end(GetClientInfo()));
}
TEST_F(LogParserTest, handleClientHasCome_befor_opening_edge_case)
{
    handleClientHasCome(std::string("client1"), Time("10:00"));

    EXPECT_EQ(m_result.str(), "");
    EXPECT_TRUE(GetClientInfo().find("client1") != end(GetClientInfo()));
}
TEST_F(LogParserTest, handleClientHasCome_after_closing_edge_case)
{
    handleClientHasCome(std::string("client1"), Time("18:00"));

    EXPECT_EQ(m_result.str(), "");
    EXPECT_TRUE(GetClientInfo().find("client1") != end(GetClientInfo()));
}
TEST_F(LogParserTest, handleClientHasCome_edge_case)
{
    handleClientHasCome(std::string("client1"), Time("10:00"));

    EXPECT_EQ(m_result.str(), "");
    ASSERT_TRUE(GetClientInfo().find("client1") != end(GetClientInfo()));
    EXPECT_TRUE(GetClientInfo()["client1"].GameTableNumber == GAME_TABLE_IS_UNDEFINED);
}
TEST_F(LogParserTest, handleClientHasCome_while_he_in_the_club)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasCome(std::string("client1"), Time("11:01"));

    EXPECT_EQ(m_result.str(), "11:01 13 YouShallNotPass\n");
    ASSERT_TRUE(GetClientInfo().find("client1") != end(GetClientInfo()));
    EXPECT_TRUE(GetClientInfo()["client1"].GameTableNumber == GAME_TABLE_IS_UNDEFINED);
}
//////////////////////////////////////////////////////////
TEST_F(LogParserTest, handleClientHasLeft_positive)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));

    handleClientHasLeft(std::string("client1"), Time("11:00"));
    EXPECT_EQ(m_result.str(), "");
    ASSERT_TRUE(GetClientInfo().find("client1") == end(GetClientInfo()));
}
TEST_F(LogParserTest, handleClientHasLeft_but_he_is_unknown)
{
    handleClientHasLeft(std::string("client1"), Time("11:00"));
    EXPECT_EQ(m_result.str(), "11:00 13 ClientUnknown\n");
}
//////////////////////////////////////////////////////////
TEST_F(LogParserTest, handleClientTakeGameTable_positive)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));

    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);

    EXPECT_EQ(m_result.str(), "");
    ASSERT_TRUE(GetClientInfo().find("client1") != end(GetClientInfo()));
    EXPECT_TRUE(GetClientInfo()["client1"].GameTableNumber == 1);
    EXPECT_TRUE(GetClientInfo()["client1"].StartSessionTime.GetString() == "12:00");
}
TEST_F(LogParserTest, handleClientTakeGameTable_but_unknown_client)
{
    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);

    EXPECT_EQ(m_result.str(), "12:00 13 ClientUnknown\n");
}
TEST_F(LogParserTest, handleClientTakeGameTable_but_unknown_table_left_bound)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));

    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 0);

    EXPECT_EQ(m_result.str(), "12:00 13 TableUnknown\n");
}
TEST_F(LogParserTest, handleClientTakeGameTable_but_unknown_table_right_bound)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));

    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 3);

    EXPECT_EQ(m_result.str(), "12:00 13 TableUnknown\n");
}
TEST_F(LogParserTest, handleClientTakeGameTable_but_its_busy)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasCome(std::string("client2"), Time("11:00"));

    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);

    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);
    handleClientTakeGameTable(std::string("client2"), Time("12:00"), 1);
    EXPECT_EQ(m_result.str(), "12:00 13 PlaceIsBusy\n12:00 13 PlaceIsBusy\n");
    EXPECT_EQ(GetClientInfo()["client1"].GameTableNumber, 1);
    EXPECT_EQ(GetClientInfo()["client2"].GameTableNumber, GAME_TABLE_IS_UNDEFINED);
}
/////////////////////////////////////////////////////////////////
TEST_F(LogParserTest, handleClientIsWaiting_positive)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasCome(std::string("client2"), Time("11:00"));
    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);
    handleClientTakeGameTable(std::string("client2"), Time("12:00"), 2);

    handleClientHasCome(std::string("client3"), Time("12:00"));
    handleClientIsWaiting(std::string("client3"), Time("12:00"));

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, handleClientIsWaiting_positive_ICanWaitNoLonger)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));

    handleClientIsWaiting(std::string("client1"), Time("12:00"));

    EXPECT_EQ(m_result.str(), "12:00 13 ICanWaitNoLonger!\n");
}
TEST_F(LogParserTest, handleClientIsWaiting_but_he_is_unknown)
{
    handleClientIsWaiting(std::string("client1"), Time("12:00"));

    EXPECT_EQ(m_result.str(), "12:00 13 ClientUnknown\n");
}
TEST_F(LogParserTest, handleClientIsWaiting_but_full_queue)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasCome(std::string("client2"), Time("11:00"));
    handleClientHasCome(std::string("client3"), Time("11:00"));
    handleClientHasCome(std::string("client4"), Time("11:00"));
    handleClientHasCome(std::string("client5"), Time("11:00"));
    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);
    handleClientTakeGameTable(std::string("client2"), Time("12:00"), 2);
    handleClientIsWaiting(std::string("client3"), Time("12:00"));
    handleClientIsWaiting(std::string("client4"), Time("12:00"));
    handleClientIsWaiting(std::string("client5"), Time("12:00"));

    handleClientHasCome(std::string("client6"), Time("11:00"));
    handleClientIsWaiting(std::string("client6"), Time("12:00"));

    EXPECT_EQ(m_result.str(), "12:00 11 client6\n");
    ASSERT_TRUE(GetClientInfo().find("client6") == end(GetClientInfo()));
}
TEST_F(LogParserTest, handleClientIsWaiting_but_already_taken_the_table)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));

    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);
    handleClientIsWaiting(std::string("client1"), Time("12:00"));

    EXPECT_EQ(m_result.str(), "12:00 13 ICanWaitNoLonger!\n");
}
/////////////////////////////////////////////////////////////////
TEST_F(LogParserTest, handleClientTakeGameTable_after_smth_free_up_it)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasCome(std::string("client2"), Time("11:00"));
    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);
    handleClientTakeGameTable(std::string("client2"), Time("12:00"), 2);

    handleClientHasCome(std::string("client3"), Time("13:00"));
    handleClientIsWaiting(std::string("client3"), Time("13:00"));
    handleClientHasLeft(std::string("client1"), Time("14:00"));

    EXPECT_EQ(m_result.str(), "14:00 12 client3 1\n");
    EXPECT_TRUE(GetQueueClients().empty());
    EXPECT_EQ(GetClientInfo()["client3"].GameTableNumber, 1);
    EXPECT_EQ(GetClientInfo()["client3"].StartSessionTime.GetString(), "14:00");
}
TEST_F(LogParserTest, handleClientTakeGameTable_after_release_it)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasCome(std::string("client2"), Time("11:00"));

    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);
    handleClientHasLeft(std::string("client1"), Time("12:00"));
    handleClientTakeGameTable(std::string("client2"), Time("12:00"), 1);

    EXPECT_EQ(m_result.str(), "");
}
TEST_F(LogParserTest, handleClientHasCome_again)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasLeft(std::string("client1"), Time("11:01"));
    handleClientHasCome(std::string("client1"), Time("11:02"));

    EXPECT_EQ(m_result.str(), "");
    ASSERT_TRUE(GetClientInfo().find("client1") != end(GetClientInfo()));
    EXPECT_TRUE(GetClientInfo()["client1"].GameTableNumber == GAME_TABLE_IS_UNDEFINED);
}
TEST_F(LogParserTest, handleClientHasLeft_from_queue)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientHasCome(std::string("client2"), Time("11:00"));
    handleClientTakeGameTable(std::string("client1"), Time("12:00"), 1);
    handleClientTakeGameTable(std::string("client2"), Time("12:00"), 2);

    handleClientHasCome(std::string("client3"), Time("13:00"));
    handleClientIsWaiting(std::string("client3"), Time("13:00"));
    handleClientHasLeft(std::string("client3"), Time("14:00"));

    EXPECT_EQ(m_result.str(), "");
    EXPECT_TRUE(GetClientInfo().find("client3") == end(GetClientInfo()));
    EXPECT_TRUE(GetQueueClients().empty());
}
/////////////////////////////////////////////////////////////////
TEST_F(LogParserTest, GameSession_results_positive)
{
    handleClientHasCome(std::string("client1"), Time("11:00"));
    handleClientTakeGameTable(std::string("client1"), Time("11:00"), 1);
    EXPECT_TRUE(GetGameTables()[1].IsBusy());
    handleClientHasLeft(std::string("client1"), Time("12:00"));

    EXPECT_TRUE(GetGameTables()[1].GetProfit() == 10);
    EXPECT_FALSE(GetGameTables()[1].IsBusy());
}
TEST_F(LogParserTest, GameSession_edge_case_1)
{
    handleClientHasCome(std::string("client1"), Time("13:00"));
    handleClientTakeGameTable(std::string("client1"), Time("13:00"), 1);
    handleClientHasLeft(std::string("client1"), Time("13:01"));

    EXPECT_TRUE(GetGameTables()[1].GetProfit() == 10);
    EXPECT_FALSE(GetGameTables()[1].IsBusy());
}
TEST_F(LogParserTest, GameSession_edge_case_2)
{
    handleClientHasCome(std::string("client1"), Time("13:00"));
    handleClientTakeGameTable(std::string("client1"), Time("13:00"), 1);
    handleClientHasLeft(std::string("client1"), Time("13:59"));

    EXPECT_TRUE(GetGameTables()[1].GetProfit() == 10);
    EXPECT_FALSE(GetGameTables()[1].IsBusy());
}
TEST_F(LogParserTest, GameSession_edge_case_3)
{
    handleClientHasCome(std::string("client1"), Time("13:00"));
    handleClientTakeGameTable(std::string("client1"), Time("13:00"), 1);
    handleClientHasLeft(std::string("client1"), Time("13:00"));

    EXPECT_TRUE(GetGameTables()[1].GetProfit() == 0);
    EXPECT_FALSE(GetGameTables()[1].IsBusy());
}
