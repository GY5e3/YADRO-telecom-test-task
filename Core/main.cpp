#include <iostream>

#include "LogParser.hpp"
#include "LogHandler.hpp"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << "Argument with file name is missing" << std::endl;
        return 1;
    }

    LogParser parser;
    if(parser(argv[1]))
        return 1;
        
    LogHandler handler;
    handler.Execute(argv[1]);
    return 0;
}
