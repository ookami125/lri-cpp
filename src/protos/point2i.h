#pragma once

#include "../picoproto.h"

#include <stdint.h>
#include <iostream>

struct Point2I {
    /*1*/ int32_t x;
    /*2*/ int32_t y;

    Point2I(){}

    Point2I(picoproto::Message* message) : 
        x (message->GetInt64(1)),
        y (message->GetInt64(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Point2I& obj)
{
    return os << "{ \"x\":" << obj.x << ", \"y\":" << obj.y << " }";
}