#ifndef TASK1_TASK1_H
#define TASK1_TASK1_H

#include <iostream>
#include <string>

constexpr short INVALID_MAP = 1;
enum e_verdict {X_WIN = 0, O_WIN = 1, XO_DRAW = 2};

void catch_error(const std::string &error_msg, const short &exit_code);
void print_map(const std::string *map, const short &size);
#endif
