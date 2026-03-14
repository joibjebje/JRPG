
#include <vector>
#include <string>
#include <sstream>

namespace jrpg {

std::vector<std::string> split(std::string& s, char delim) {

    std::vector<std::string> parts;
    std::string token;
    std::stringstream ss(s);

    while (std::getline(ss, token, delim)) {
        parts.push_back(token);
    }

    return parts;

}

bool isNumber(std::string& s) {

    if (s.empty()) return false;
    for (char c : s) {
        if (c < '0' || c > '9') return false;
    }
    return true;

}


}
