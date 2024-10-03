#pragma once

#include "../picoproto.h"

#include <stdint.h>

#include "utils.h"

enum ReferenceNorth {
    REFERENCE_NORTH_MAGNETIC = 0,
    REFERENCE_NORTH_TRUE = 1,
};

struct Track {
    /*1*/ double value;
    /*2*/ ReferenceNorth ref;

    Track(picoproto::Message* message) :
        value (message->GetDouble(1)),
        ref ((ReferenceNorth)message->GetUInt64(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Track& obj)
{
    os << "{" << std::endl;
    os << "\"value\": " << obj.value << "," << std::endl;
    os << "\"ref\": " << obj.ref << std::endl; 
    return os << "}";
}

struct Heading {
    /*1*/ double value;
    /*2*/ ReferenceNorth ref;

    Heading(picoproto::Message* message) :
        value (message->GetDouble(1)),
        ref ((ReferenceNorth)message->GetUInt64(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Heading& obj)
{
    os << "{" << std::endl;
    os << "\"value\": " << obj.value << "," << std::endl;
    os << "\"ref\": " << obj.ref << std::endl; 
    return os << "}";
}

enum ReferenceAltitude {
    REFERENCE_ALTITUDE_SEA_LEVEL = 0,
};

struct Altitude {
    /*1*/ double value;
    /*2*/ ReferenceAltitude ref;

    Altitude(picoproto::Message* message) :
        value (message->GetDouble(1)),
        ref ((ReferenceAltitude)message->GetUInt64(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Altitude& obj)
{
    os << "{" << std::endl;
    os << "\"value\": " << obj.value << "," << std::endl;
    os << "\"ref\": " << obj.ref << std::endl; 
    return os << "}";
}

enum ProcessingMethod {
    PROCESSING_METHOD_UNKNOWN = 0,
    PROCESSING_METHOD_GPS = 1,
    PROCESSING_METHOD_CELLID = 2,
    PROCESSING_METHOD_WLAN = 3,
    PROCESSING_METHOD_MANUAL = 4,
    PROCESSING_METHOD_FUSED = 5,
};

struct GPSData {
    /*1*/ std::optional<double> latitude;
    /*2*/ std::optional<double> longitude;
    /*3*/ std::optional<uint64_t> timestamp;
    /*4*/ std::optional<double> dop;
    /*5*/ std::optional<Track> track;
    /*6*/ std::optional<Heading> heading;
    /*7*/ std::optional<Altitude> altitude;
    /*8*/ std::optional<double> speed;
    /*9*/ std::optional<ProcessingMethod> processing_method;

    GPSData(picoproto::Message* message) :
        latitude (ToOptional(message->GetDoubleArray(1))),
        longitude (ToOptional(message->GetDoubleArray(2))),
        timestamp (ToOptional(message->GetUInt64Array(3))),
        dop (ToOptional(message->GetDoubleArray(4))),
        track (ToOptional<Track>(message->GetMessageArray(5))),
        heading (ToOptional<Heading>(message->GetMessageArray(6))),
        altitude (ToOptional<Altitude>(message->GetMessageArray(7))),
        speed (ToOptional(message->GetDoubleArray(8))),
        processing_method (ToOptional<ProcessingMethod>(message->GetUInt64Array(9)))
    {}
};

std::ostream& operator<<(std::ostream& os, const GPSData& obj)
{
    os << "{" << std::endl;
    os << "\"latitude\": " << obj.latitude << "," << std::endl; 
    os << "\"longitude\": " << obj.longitude << "," << std::endl;
    os << "\"timestamp\": " << obj.timestamp << "," << std::endl;
    os << "\"dop\": " << obj.dop << "," << std::endl;
    os << "\"track\": " << obj.track << "," << std::endl;
    os << "\"heading\": " << obj.heading << "," << std::endl;
    os << "\"altitude\": " << obj.altitude << "," << std::endl;
    os << "\"speed\": " << obj.speed << "," << std::endl;
    os << "\"processing_method\": " << obj.processing_method << std::endl; 
    return os << "}";
}