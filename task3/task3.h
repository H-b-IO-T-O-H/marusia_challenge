#ifndef MARUS_TASK3_TASK3_H
#define MARUS_TASK3_TASK3_H

#include <iostream>
#include <vector>

constexpr int MAP_MIN_SIZE = 10;
constexpr int MAP_MAX_SIZE = 100;
constexpr short INVALID_MAP = 1;

void catch_error(const std::string &error_msg, const short &exit_code);
enum e_directions {PATH_SEPARATOR = -1, UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};

#endif
