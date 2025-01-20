#include "TimeTest.hpp"
///////////////////////////////////////////////////////////////
TEST_F(TimeTest, Ctor_default)
{
    Time time;

    EXPECT_EQ(time.GetHours(), 0);
    EXPECT_EQ(time.GetMinutes(), 0);
}
TEST_F(TimeTest, Ctor_params_positive)
{
    EXPECT_NO_THROW(Time time(23,59));
    Time time(23,59);
    EXPECT_EQ(time.GetHours(), 23);
    EXPECT_EQ(time.GetMinutes(), 59);
}
TEST_F(TimeTest, Ctor_params_hours_not_in_the_range)
{
    EXPECT_THROW(Time time(24,0), std::runtime_error);
    EXPECT_THROW(Time time(-1,0), std::runtime_error);
}
TEST_F(TimeTest, Ctor_params_minutes_not_in_the_range)
{
    EXPECT_THROW(Time time(0,60), std::runtime_error);
    EXPECT_THROW(Time time(0,-1), std::runtime_error);
}
///////////////////////////////////////////////////////////////
TEST_F(TimeTest, SetTime_positive)
{
    Time time;
    EXPECT_NO_THROW(time.SetTime("12:30"));
    EXPECT_NO_THROW(time.SetTime("23:59"));
    EXPECT_NO_THROW(time.SetTime("00:00"));
}
TEST_F(TimeTest, SetTime_wrond_format)
{
    Time time;
    EXPECT_THROW(time.SetTime("invalid"), std::runtime_error);
    EXPECT_THROW(time.SetTime("10:000"), std::runtime_error);
    EXPECT_THROW(time.SetTime("1:00"), std::runtime_error);
    EXPECT_THROW(time.SetTime("01 00"), std::runtime_error);       
}
TEST_F(TimeTest, SetTime_hours_not_in_the_range)
{
    Time time;
    EXPECT_THROW(time.SetTime("24:00"), std::runtime_error);
    EXPECT_THROW(time.SetTime("-1:00"), std::runtime_error);    
}
TEST_F(TimeTest, SetTime_minutes_not_in_the_range)
{
    Time time;
    EXPECT_THROW(time.SetTime("00:60"), std::runtime_error);
    EXPECT_THROW(time.SetTime("00:-1"), std::runtime_error);    
}
///////////////////////////////////////////////////////////////
TEST_F(TimeTest, Add_positive) {
    Time t1(2, 24);
    Time t2(1, 35); 
    
    Time result = t1 + t2; 

    EXPECT_EQ(result.GetHours(), 3);
    EXPECT_EQ(result.GetMinutes(), 59);
}
TEST_F(TimeTest, Add_with_carry) {
    Time t1(2, 26);
    Time t2(1, 34); 
    
    Time result = t1 + t2; 

    EXPECT_EQ(result.GetHours(), 4);
    EXPECT_EQ(result.GetMinutes(), 0);
}
///////////////////////////////////////////////////////////////
TEST_F(TimeTest, Sub_positive) {
    Time t1(2, 35);
    Time t2(1, 35); 
    
    Time result = t1 - t2; 

    EXPECT_EQ(result.GetHours(), 1);
    EXPECT_EQ(result.GetMinutes(), 0);
}
TEST_F(TimeTest, Sub_with_carry) {
    Time t1(2, 26);
    Time t2(1, 34); 
    
    Time result = t1 - t2; 

    EXPECT_EQ(result.GetHours(), 0);
    EXPECT_EQ(result.GetMinutes(), 52);
}
///////////////////////////////////////////////////////////////
TEST_F(TimeTest, LessThan_positive) {
    Time t1(1, 35);
    Time t2(2, 35); 
    
    EXPECT_EQ(t1 < t2, true);
    EXPECT_EQ(t2 < t1, false);
}
TEST_F(TimeTest, LessThan_edge_case) {
    Time t1(1, 25);
    Time t2(1, 25); 
    
    EXPECT_EQ(t1 < t2, false);
}
///////////////////////////////////////////////////////////////
TEST_F(TimeTest, GetString_positive) {
    Time time(12, 30);
    
    EXPECT_EQ(time.GetString(), "12:30");
}
TEST_F(TimeTest, GetString_edge_case) {
    Time time(0, 0);
    
    EXPECT_EQ(time.GetString(), "00:00");
}
