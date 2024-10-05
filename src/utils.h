#pragma once
#include <string>
#include "error.h"

inline void swap(char& lhs, char& rhs) {
    char temp = lhs;
    lhs = rhs;
    rhs = temp;
}

inline std::string toLower(std::string str) {
    for(char& c : str) {
        c = (char)tolower(c);
    }
    return str;
}

ErrorOr<char*> loadFile(const char* path, size_t& length);