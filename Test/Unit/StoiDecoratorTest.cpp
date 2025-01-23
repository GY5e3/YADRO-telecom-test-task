#include "StoiDecoratorTest.hpp"

TEST_F(StoiDecoratorTest, Positive_test_case)
{
    std::string test = "11";

    EXPECT_NO_THROW(stoi_decorator(test));
    EXPECT_EQ(stoi_decorator(test), 11);
}
TEST_F(StoiDecoratorTest, Empty_str)
{
    std::string test = "";

    EXPECT_THROW(stoi_decorator(test), std::runtime_error);
}
TEST_F(StoiDecoratorTest, Number_and_letters)
{
    std::string test = "10aa";

    EXPECT_THROW(stoi_decorator(test), std::runtime_error);
}
TEST_F(StoiDecoratorTest, Number_space_number)
{
    std::string test = "1 1";

    EXPECT_THROW(stoi_decorator(test), std::runtime_error);
}
TEST_F(StoiDecoratorTest, Insignificant_zero)
{
    std::string test = "011";

    EXPECT_NO_THROW(stoi_decorator(test));
    EXPECT_EQ(stoi_decorator(test), 11);
}
TEST_F(StoiDecoratorTest, Few_insignificant_zeros)
{
    std::string test = "00000000000000011";

    EXPECT_NO_THROW(stoi_decorator(test));
    EXPECT_EQ(stoi_decorator(test), 11);
}