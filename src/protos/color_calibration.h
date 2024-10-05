#pragma once

#include "matrix3x3f.h"
#include "point3f.h"
#include "point2f.h"

enum IlluminantType {
    A = 0,
    D50 = 1,
    D65 = 2,
    D75 = 3,
    F2 = 4,
    F7 = 5,
    F11 = 6,
    TL84 = 7,
    UNKNOWN = 99,
};

struct SpectralSensitivity {
    /*1*/ uint32_t start;
    /*2*/ uint32_t end;
    /*3*/ std::vector<std::pair<uint8_t*, size_t>> data; //[packed = true]

    SpectralSensitivity(picoproto::Message* message) :
        start (message->GetUInt32(1)),
        end (message->GetUInt32(2)),
        data (message->GetByteArray(3))
    {}
};

std::ostream& operator<<(std::ostream& os, const SpectralSensitivity& obj)
{
    os << "{" << std::endl;
    os << "\"start\": " << obj.start << "," << std::endl;
    os << "\"end\": " << obj.end << "," << std::endl;
    os << "\"data\": \"[packed data]\"," << std::endl;
    os << "}";
    return os;
}

enum ChannelFormat {
    MONO = 0,
    RGB = 1,
    BAYER_RGGB = 2,
};

struct SpectralData {
    /*1*/ ChannelFormat format;
    /*2*/ std::vector<SpectralSensitivity> channel_data;

    SpectralData(picoproto::Message* message) :
        format ((ChannelFormat)message->GetUInt64(1)),
        channel_data (To<SpectralSensitivity>(message->GetMessageArray(2)))
    {}
};

std::ostream& operator<<(std::ostream& os, const SpectralData& obj)
{
    os << "{" << std::endl;
    os << "\"type\": " << obj.format << "," << std::endl;
    os << "\"forward_matrix\": " << obj.channel_data << "," << std::endl;
    os << "}";
    return os;
}

struct ColorCalibration {
    /*1*/ IlluminantType type;
    /*2*/ Matrix3x3F forward_matrix;
    /*3*/ Matrix3x3F color_matrix;
    /*4*/ float rg_ratio;
    /*5*/ float bg_ratio;
    /*6*/ std::vector<Point3F> macbeth_data;
    /*7*/ std::vector<Point2F> illuminant_spd;
    /*8*/ std::optional<SpectralData> spectral_data;

    ColorCalibration(picoproto::Message* message) :
        type ((IlluminantType)message->GetUInt64(1)),
        forward_matrix ((Matrix3x3F)message->GetMessage(2)),
        color_matrix ((Matrix3x3F)message->GetMessage(3)),
        rg_ratio (message->GetFloat(4)),
        bg_ratio (message->GetFloat(5)),
        macbeth_data (To<Point3F>(message->GetMessageArray(6))),
        illuminant_spd (To<Point2F>(message->GetMessageArray(7))),
        spectral_data (ToOptional<SpectralData>(message->GetMessageArray(8)))
    {}
};

std::ostream& operator<<(std::ostream& os, const ColorCalibration& obj)
{
    os << "{" << std::endl;
    os << "\"type\": " << obj.type << "," << std::endl;
    os << "\"forward_matrix\": " << obj.forward_matrix << "," << std::endl;
    os << "\"color_matrix\": " << obj.color_matrix << "," << std::endl;
    os << "\"rg_ratio\": " << obj.rg_ratio << "," << std::endl;
    os << "\"bg_ratio\": " << obj.bg_ratio << "," << std::endl;
    os << "\"macbeth_data\": " << obj.macbeth_data << "," << std::endl;
    os << "\"illuminant_spd\": " << obj.illuminant_spd << "," << std::endl;
    os << "\"spectral_data\": " << obj.spectral_data << "," << std::endl;
    os << "}";
    return os;
}