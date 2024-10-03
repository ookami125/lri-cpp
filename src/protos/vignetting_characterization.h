#pragma once

#include "../picoproto.h"

#include <optional>
#include <vector>

#include "matrix4x4f.h"

struct CrosstalkModel {
    /*1*/ uint32_t width;
    /*2*/ uint32_t height;
    /*3*/ std::vector<Matrix4x4F> data;
    /*4*/ std::vector<std::pair<uint8_t*, size_t>> data_packed; // [packed = true];

    CrosstalkModel(picoproto::Message* message) :
        width (message->GetUInt64(1)),
        height (message->GetUInt64(2)),
        data (To<Matrix4x4F>(message->GetMessageArray(3))),
        data_packed (message->GetByteArray(4))
    {}
};

std::ostream& operator<<(std::ostream& os, const CrosstalkModel& obj)
{
    os << "{" << std::endl;
    os << "\"width\": " << obj.width << "," << std::endl; 
    os << "\"height\": " << obj.height << "," << std::endl; 
    os << "\"data\": " << obj.data << "," << std::endl; 
    os << "\"data_packed\": \"[packed data]\"" << std::endl; 
    return os << "}";
}

struct VignettingModel {
    /*1*/ uint32_t width;
    /*2*/ uint32_t height;
    /*3*/ std::vector<std::pair<uint8_t*, size_t>> data; // [packed = true];

    VignettingModel(picoproto::Message* message) :
        width (message->GetUInt64(1)),
        height (message->GetUInt64(2)),
        data (message->GetByteArray(3))
    {}
};

std::ostream& operator<<(std::ostream& os, const VignettingModel& obj)
{
    os << "{" << std::endl;
    os << "\"width\": " << obj.width << "," << std::endl; 
    os << "\"height\": " << obj.height << "," << std::endl; 
    os << "\"data\": \"[packed data]\"" << std::endl; 
    return os << "}";
}

struct MirrorVignettingModel {
    /*1*/ int32_t hall_code;
    /*2*/ VignettingModel vignetting;

    MirrorVignettingModel(picoproto::Message* message) :
        hall_code (message->GetInt64(1)),
        vignetting (message->GetMessage(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const MirrorVignettingModel& obj)
{
    os << "{" << std::endl;
    os << "\"hall_code\": " << obj.hall_code << "," << std::endl; 
    os << "\"vignetting\": " << obj.vignetting << std::endl; 
    return os << "}";
}

struct VignettingCharacterization {
    /*1*/ CrosstalkModel crosstalk;
    /*2*/ std::vector<MirrorVignettingModel> vignetting;
    /*3*/ std::optional<float> relative_brightness;
    /*4*/ std::optional<int32_t> lens_hall_code;

    VignettingCharacterization(picoproto::Message* message) :
        crosstalk (message->GetMessage(1)),
        vignetting (To<MirrorVignettingModel>(message->GetMessageArray(2))),
        relative_brightness (ToOptional(message->GetFloatArray(3))),
        lens_hall_code (ToOptional(message->GetInt64Array(4)))
    {}
};

std::ostream& operator<<(std::ostream& os, const VignettingCharacterization& obj)
{
    os << "{" << std::endl;
    os << "\"crosstalk\": " << obj.crosstalk << "," << std::endl; 
    os << "\"vignetting\": " << obj.vignetting << "," << std::endl;
    os << "\"relative_brightness\": " << obj.relative_brightness << "," << std::endl;
    os << "\"lens_hall_code\": " << obj.lens_hall_code << std::endl; 
    return os << "}";
}