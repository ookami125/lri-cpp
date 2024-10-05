#include "utils.h"
#include "error.h"

ErrorOr<char*> loadFile(const char* path, size_t& length)
{
    FILE* file = fopen(path, "rb");
    if(file == nullptr) {
        return {{"Failed to open file ("+std::string(path)+")"}};
    }
    fseek(file, 0, SEEK_END);
    length = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);
    char* data = (char*)malloc(length);
    if(data == nullptr) {
        return {{"Failed allocate ("+std::to_string(length)+" bytes) for file ("+std::string(path)+")"}};
    }
    size_t rlength = fread(data, length, 1, file);
    if(rlength != 1) {
        return {{"Failed to read whole file ("+std::string(path)+")"}};
    }
    fclose(file);
    return data;
};