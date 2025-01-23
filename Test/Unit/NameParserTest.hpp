#pragma once

#include <string>

#include <gtest/gtest.h>

#include "utils.hpp"

class NameParserTest : public testing::Test
{
protected:
    utils::ClientNameParser name_parser;
};