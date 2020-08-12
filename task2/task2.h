#ifndef TASK2_TASK2_H
#define TASK2_TASK2_H

#include <iostream>
#include <string>
#include <array>

constexpr int MAP_SIZE = 9;
constexpr int SHOT_CNT = 10;
constexpr short INVALID_MAP = 1;
constexpr short INVALID_COORD = 2;

void catch_error(const std::string &error_msg, const short &exit_code);
#endif