#pragma once
#include "../picoproto.h"

#include <stdint.h>

struct RectangleI {
    /*1*/ int32_t x;
    /*2*/ int32_t y;
    /*3*/ int32_t width;
    /*4*/ int32_t height;

    RectangleI(){}

    RectangleI(picoproto::Message* message) : 
        x (message->GetInt32(1)),
        y (message->GetInt32(2)),
        width (message->GetInt32(3)),
        height (message->GetInt32(4))
    {}
};

std::ostream& operator<<(std::ostream& os, const RectangleI& obj)
{
    return os << "{ \"x\":" << obj.x << ", \"y\":" << obj.y << ", \"width\":" << obj.width << ", \"height\":" << obj.height << " }";
}