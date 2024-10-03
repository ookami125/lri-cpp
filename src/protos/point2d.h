#pragma once

#include "../picoproto.h"

#include <stdint.h>
#include <iostream>

struct Point2D {
    /*1*/ double x;
    /*2*/ double y;
    
    Point2D(){}

    Point2D(picoproto::Message* message) : 
        x (message->GetDouble(1)),
        y (message->GetDouble(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Point2D& obj)
{
    return os << "{ \"x\":" << obj.x << ", \"y\":" << obj.y << " }";
}