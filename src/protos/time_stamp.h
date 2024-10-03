#pragma once

#include "../picoproto.h"

#include <vector>
#include <stdint.h>

struct TimeStamp {
    /*1*/ uint32_t year;
    /*2*/ uint32_t month;
    /*3*/ uint32_t day;
    /*4*/ uint32_t hour;
    /*5*/ uint32_t minute;
    /*6*/ uint32_t second;
    /*7*/ std::optional<int32_t> tz_offset;

    TimeStamp(picoproto::Message* message) :
        year ((uint32_t)message->GetUInt64(1)),
        month ((uint32_t)message->GetUInt64(2)),
        day ((uint32_t)message->GetUInt64(3)),
        hour ((uint32_t)message->GetUInt64(4)),
        minute ((uint32_t)message->GetUInt64(5)),
        second ((uint32_t)message->GetUInt64(6)),
        tz_offset (ToOptional(message->GetInt32Array(7)))
    {}
};

std::ostream& operator<<(std::ostream& os, const TimeStamp& obj)
{
    return os << "\"" << obj.year << "/" << obj.month << "/" << obj.day << " " << obj.hour << ":" << obj.minute << ":" << obj.second << "\"";
}