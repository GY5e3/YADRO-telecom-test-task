#include "utils.hpp"

int utils::StoiDecorator::operator()(const std::string &str) const
{
    int result;
    int skipInsignificantZeros = 0;
    try
    {
        while (str[skipInsignificantZeros] == '0' && skipInsignificantZeros < str.length() - 1)
            skipInsignificantZeros++;
        result = std::stoi(str.substr(skipInsignificantZeros));
    }
    catch (...)
    {
        throw std::runtime_error("stoi_decorator");
    }
    // Проверка сделана для того, чтобы исключать строки вида: <цифры><другие_символы>(342321helloworld);
    if (std::to_string(result).length() < str.substr(skipInsignificantZeros).length() || result < 0)
        throw std::runtime_error("stoi_decorator");
    return result;
}

std::string utils::ClientNameParser::operator()(const std::string &str) const
{
    for (auto c : str)
    {
        if ('a' <= c && c <= 'z' || '0' <= c && c <= '9' || c == '_' || c == '-')
            continue;
        else
            throw std::runtime_error("Invalid client name");
    }
    return str;
}
