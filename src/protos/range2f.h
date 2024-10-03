#pragma once

#include "../picoproto.h"

#include <stdint.h>

struct Range2F {
    /*1*/ float min_val;
    /*2*/ float max_val;
    
    Range2F(){}

    Range2F(picoproto::Message* message) : 
        min_val (message->GetFloat(1)),
        max_val (message->GetFloat(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Range2F& obj)
{
    return os << "{ \"min_val\":" << obj.min_val << ", \"max_val\":" << obj.max_val << " }";
}