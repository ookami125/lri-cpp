#pragma once

#include "point2f.h"

enum HDRMode {
    HDR_MODE_NONE = 0,
    HDR_MODE_DEFAULT = 1,
    HDR_MODE_NATURAL = 2,
    HDR_MODE_SURREAL = 3,
};

enum ViewPresets {
    VIEW_PRESET_NONE = 0,
    VIEW_PRESET_NATURAL = 1,
    VIEW_PRESET_FAITHFUL = 2,
    VIEW_PRESET_LANDSCAPE = 3,
    VIEW_PRESET_PORTRAIT = 4,
};

enum SceneMode {
    SCENE_MODE_PORTRAIT = 0,
    SCENE_MODE_LANDSCAPE = 1,
    SCENE_MODE_SPORT = 2,
    SCENE_MODE_MACRO = 3,
    SCENE_MODE_NIGHT = 4,
    SCENE_MODE_NONE = 5,
};

enum AWBMode {
    AWB_MODE_AUTO = 0,
    AWB_MODE_DAYLIGHT = 1,
    AWB_MODE_SHADE = 2,
    AWB_MODE_CLOUDY = 3,
    AWB_MODE_TUNGSTEN = 4,
    AWB_MODE_FLUORESCENT = 5,
    AWB_MODE_FLASH = 6,
    AWB_MODE_CUSTOM = 7,
    AWB_MODE_KELVIN = 8,
};

enum Orientation {
    ORIENTATION_NORMAL = 0,
    ORIENTATION_ROT90_CW = 1,
    ORIENTATION_ROT90_CCW = 2,
    ORIENTATION_ROT90_CW_VFLIP = 3,
    ORIENTATION_ROT90_CCW_VFLIP = 4,
    ORIENTATION_VFLIP = 5,
    ORIENTATION_HFLIP = 6,
    ORIENTATION_ROT180 = 7,
};

enum AspectRatio {
    ASPECT_RATIO_4_3 = 0,
    ASPECT_RATIO_16_9 = 1,
};

struct Crop {
    /*1*/ Point2F start;
    /*2*/ Point2F size;

    Crop(picoproto::Message* message) :
        start (message->GetMessage(1)),
        size (message->GetMessage(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Crop& obj)
{
    os << "{" << std::endl;
    os << "\"start\": " << obj.start << "," << std::endl; 
    os << "\"size\": " << obj.size << std::endl; 
    return os << "}";
}

struct ChannelGain {
    /*1*/ float r;
    /*2*/ float g_r;
    /*3*/ float g_b;
    /*4*/ float b;

    ChannelGain(picoproto::Message* message) :
        r (message->GetFloat(1)),
        g_r (message->GetFloat(2)),
        g_b (message->GetFloat(3)),
        b (message->GetFloat(4))
    {}
};

std::ostream& operator<<(std::ostream& os, const ChannelGain& obj)
{
    os << "{" << std::endl;
    os << "\"r\": " << obj.r << "," << std::endl; 
    os << "\"g_r\": " << obj.g_r << "," << std::endl;
    os << "\"g_b\": " << obj.g_b << "," << std::endl;
    os << "\"b\": " << obj.b << std::endl; 
    return os << "}";
}

struct ViewPreferences {
    /*1*/ std::optional<float> f_number;
    /*2*/ std::optional<float> ev_offset;
    /*3*/ std::optional<bool> disable_cropping;
    /*4*/ std::optional<HDRMode> hdr_mode;
    /*5*/ std::optional<ViewPresets> view_preset;
    /*6*/ std::optional<SceneMode> scene_mode;
    /*7*/ std::optional<AWBMode> awb_mode;
    /*8*/ //reserved
    /*9*/ std::optional<Orientation> orientation;
    /*10*/ std::optional<float> image_gain;
    /*11*/ std::optional<uint64_t> image_integration_time_ns;
    /*12*/ std::optional<uint32_t> user_rating;
    /*13*/ std::optional<AspectRatio> aspect_ratio;
    /*14*/ std::optional<Crop> crop;
    /*15*/ std::optional<ChannelGain> awb_gains;
    /*16*/ std::optional<bool> is_on_tripod;
    /*17*/ std::optional<float> qc_lux_index;
    /*18*/ std::optional<float> display_gain;
    /*19*/ std::optional<uint64_t> display_integration_time_ns;

    ViewPreferences(picoproto::Message* message) :
        f_number (ToOptional(message->GetFloatArray(1))),
        ev_offset (ToOptional(message->GetFloatArray(2))),
        disable_cropping (ToOptional(message->GetBoolArray(3))),
        hdr_mode (ToOptional<HDRMode>(message->GetInt64Array(4))),
        view_preset (ToOptional<ViewPresets>(message->GetInt64Array(5))),
        scene_mode (ToOptional<SceneMode>(message->GetInt64Array(6))),
        awb_mode (ToOptional<AWBMode>(message->GetInt64Array(7))),
        orientation (ToOptional<Orientation>(message->GetInt64Array(9))),
        image_gain (ToOptional(message->GetFloatArray(10))),
        image_integration_time_ns (ToOptional(message->GetUInt64Array(11))),
        user_rating (ToOptional(message->GetUInt32Array(12))),
        aspect_ratio (ToOptional<AspectRatio>(message->GetInt64Array(13))),
        crop (ToOptional<Crop>(message->GetMessageArray(14))),
        awb_gains (ToOptional<ChannelGain>(message->GetMessageArray(15))),
        is_on_tripod (ToOptional(message->GetBoolArray(16))),
        qc_lux_index (ToOptional(message->GetFloatArray(17))),
        display_gain (ToOptional(message->GetFloatArray(18))),
        display_integration_time_ns (ToOptional(message->GetUInt64Array(19)))
    {}
};

std::ostream& operator<<(std::ostream& os, const ViewPreferences& obj)
{
    os << "{" << std::endl;
    os << "\"f_number\": " << obj.f_number << "," << std::endl; 
    os << "\"ev_offset\": " << obj.ev_offset << "," << std::endl;
    os << "\"disable_cropping\": " << obj.disable_cropping << "," << std::endl;
    os << "\"hdr_mode\": " << obj.hdr_mode << "," << std::endl;
    os << "\"view_preset\": " << obj.view_preset << "," << std::endl;
    os << "\"scene_mode\": " << obj.scene_mode << "," << std::endl;
    os << "\"awb_mode\": " << obj.awb_mode << "," << std::endl;
    os << "\"orientation\": " << obj.orientation << "," << std::endl;
    os << "\"image_gain\": " << obj.image_gain << "," << std::endl;
    os << "\"image_integration_time_ns\": " << obj.image_integration_time_ns << "," << std::endl;
    os << "\"user_rating\": " << obj.user_rating << "," << std::endl;
    os << "\"aspect_ratio\": " << obj.aspect_ratio << "," << std::endl;
    os << "\"crop\": " << obj.crop << "," << std::endl;
    os << "\"awb_gains\": " << obj.awb_gains << "," << std::endl;
    os << "\"is_on_tripod\": " << obj.is_on_tripod << "," << std::endl;
    os << "\"qc_lux_index\": " << obj.qc_lux_index << "," << std::endl;
    os << "\"display_gain\": " << obj.display_gain << "," << std::endl;
    os << "\"display_integration_time_ns\": " << obj.display_integration_time_ns << std::endl; 
    return os << "}";
}