#pragma once

#include "../picoproto.h"

#include <stdint.h>
#include <iostream>

struct Point2F {
    /*1*/ float x;
    /*2*/ float y;
    
    Point2F(){}

    Point2F(picoproto::Message* message) : 
        x (message->GetFloat(1)),
        y (message->GetFloat(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Point2F& obj)
{
    return os << "{ \"x\":" << obj.x << ", \"y\":" << obj.y << " }";
}