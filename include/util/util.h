#pragma once

#include <string>
#include <vector>

namespace jrpg {

std::vector<std::string> split(std::string& s, char delim);

bool isNumber(std::string& s);

}