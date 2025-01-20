#pragma once

#include <string>

#include <gtest/gtest.h>

#include "utils.hpp"

class StoiDecoratorTest : public testing::Test
{
protected:
    utils::StoiDecorator stoi_decorator;
};