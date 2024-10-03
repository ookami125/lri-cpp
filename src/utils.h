#pragma once
#include <string>

inline void swap(char& lhs, char& rhs) {
    char temp = lhs;
    lhs = rhs;
    rhs = temp;
}

inline std::string toLower(std::string str) {
    for(char& c : str) {
        c = tolower(c);
    }
    return str;
}

inline char* loadFile(const char* path, size_t& length) {
    FILE* file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* data = (char*)malloc(length);
    fread(data, length, 1, file);
    fclose(file);
    return data;
}