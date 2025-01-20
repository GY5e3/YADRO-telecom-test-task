#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <streambuf>
#include <memory>

#include <gtest/gtest.h>

#include "utils.hpp"

class StoiDecoratorTest : public testing::Test
{
protected:
    utils::StoiDecorator stoi_decorator;
};