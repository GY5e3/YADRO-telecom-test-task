#include "NameParserTest.hpp"

TEST_F(NameParserTest, ParseName)
{
    for (int i = 0; i <= 255; i++)
    {
        char cur = static_cast<char>(i);
        std::string name;
        EXPECT_THROW(name_parser(name), std::runtime_error);
        name.push_back(cur);
        if ('a' <= cur && cur <= 'z' || '0' <= cur && cur <= '9' || cur == '_' || cur == '-')
        {
            EXPECT_NO_THROW(name_parser(name));
            EXPECT_EQ(name_parser(name), name);
        }
        else
            EXPECT_THROW(name_parser(name), std::runtime_error);
    }
}
// Invalid client name