#pragma once

#include <optional>

#include "utils.h"

#include "camera_id.h"
#include "point2i.h"
#include "point2f.h"

enum AFMode {
    AUTO = 0,
};

struct AFInfo {
    /*1*/ AFMode mode;
    /*2*/ std::optional<Point2F> roi_center;
    /*3*/ std::optional<float> disparity_focus_distance;
    /*4*/ std::optional<float> contrast_focus_distance;
    /*5*/ std::optional<bool> lens_timeout;
    /*6*/ std::optional<bool> mirror_timeout;
    /*7*/ std::optional<int32_t> mirror_position;

    AFInfo(picoproto::Message* message) :
        mode ((AFMode)(message->GetUInt64(1))),
        roi_center (ToOptional<Point2F>(message->GetMessageArray(2))),
        disparity_focus_distance (ToOptional(message->GetFloatArray(3))),
        contrast_focus_distance (ToOptional(message->GetFloatArray(4))),
        lens_timeout (ToOptional(message->GetBoolArray(5))),
        mirror_timeout (ToOptional(message->GetBoolArray(6))),
        mirror_position (ToOptional(message->GetInt32Array(7)))
    {}
};

std::ostream& operator<<(std::ostream& os, const AFInfo& obj)
{
    os << "{" << std::endl;
    os << "\"mode\": " << obj.mode << "," << std::endl; 
    os << "\"roi_center\": " << obj.roi_center << "," << std::endl;
    os << "\"disparity_focus_distance\": " << obj.disparity_focus_distance << "," << std::endl;
    os << "\"contrast_focus_distance\": " << obj.contrast_focus_distance << "," << std::endl;
    os << "\"lens_timeout\": " << obj.lens_timeout << "," << std::endl;
    os << "\"mirror_timeout\": " << obj.mirror_timeout << "," << std::endl;
    os << "\"mirror_position\": " << obj.mirror_position << std::endl; 
    return os << "}";
}

enum FormatType {
    RAW_BAYER_JPEG = 0,
    RAW_RESERVED_0 = 1,
    RAW_RESERVED_1 = 2,
    RAW_RESERVED_2 = 3,
    RAW_RESERVED_3 = 4,
    RAW_RESERVED_4 = 5,
    RAW_RESERVED_5 = 6,
    RAW_PACKED_10BPP = 7,
    RAW_PACKED_12BPP = 8,
    RAW_PACKED_14BPP = 9,
};

struct Surface {
    /*1*/ Point2I start;
    /*2*/ Point2I size;
    /*3*/ FormatType format;
    /*4*/ uint32_t row_stride;
    /*5*/ uint64_t data_offset;
    /*6*/ std::optional<Point2F> data_scale;

    Surface(picoproto::Message* message) :
        start ((Point2I)message->GetMessage(1)),
        size ((Point2I)message->GetMessage(2)),
        format ((FormatType)message->GetUInt64(3)),
        row_stride (message->GetUInt32(4)),
        data_offset (message->GetUInt64(5)),
        data_scale ((Point2F)message->GetMessage(6))
    {}
};


std::ostream& operator<<(std::ostream& os, const Surface& obj)
{
    os << "{" << std::endl;
    os << "\"start\": " << obj.start << "," << std::endl;
    os << "\"size\": " << obj.size << "," << std::endl;
    os << "\"format\": " << obj.format << "," << std::endl;
    os << "\"row_stride\": " << obj.row_stride << "," << std::endl;
    os << "\"data_offset\": " << obj.data_offset << "," << std::endl;
    os << "\"data_scale\": " << obj.data_scale << std::endl;
    os << "}";
    return os;
}

struct CameraModule {
    /*1*/ std::optional<AFInfo> af_info;
    /*2*/ CameraID id;
    /*3*/ std::optional<bool> is_enabled; //[default = true];
    /*4*/ std::optional<int32_t> mirror_position; //[default = 0];
    /*5*/ int32_t lens_position;
    /*6*/ //reserved;
    /*7*/ float sensor_analog_gain;
    /*8*/ uint64_t sensor_exposure;
    /*9*/ Surface sensor_data_surface;
    /*10*/ std::optional<int32_t> sensor_temparature;
    /*11*/ std::optional<bool> sensor_is_horizontal_flip; //[default = false];
    /*12*/ std::optional<bool> sensor_is_vertical_flip; //[default = false];
    /*13*/ std::optional<Point2I> sensor_bayer_red_override;
    /*14*/ std::optional<float> sensor_digital_gain;
    /*15*/ std::optional<uint32_t> frame_index;
    /*16*/ std::optional<bool> sensor_dpc_on; //[default = true];
    /*17*/ std::optional<int32_t> sensor_exp_start_offset;
    /*18*/ std::optional<float> sensor_scan_speed;
    
    CameraModule(picoproto::Message* message) :
        af_info (ToOptional<AFInfo>(message->GetMessageArray(1))),
        id ((CameraID)(message->GetUInt64(2))),
        is_enabled (ToOptional(message->GetBoolArray(3))),
        mirror_position (ToOptional(message->GetInt32Array(4))),
        lens_position (message->GetInt32(5)),
        sensor_analog_gain (message->GetFloat(7)),
        sensor_exposure (message->GetUInt64(8)),
        sensor_data_surface ((Surface)(message->GetMessage(9))),
        sensor_temparature (ToOptional(message->GetInt32Array(10))),
        sensor_is_horizontal_flip (ToOptional(message->GetBoolArray(11))),
        sensor_is_vertical_flip (ToOptional(message->GetBoolArray(12))),
        sensor_bayer_red_override (ToOptional<Point2I>(message->GetMessageArray(13))),
        sensor_digital_gain (ToOptional(message->GetBoolArray(14))),
        frame_index (ToOptional(message->GetUInt32Array(15))),
        sensor_dpc_on (ToOptional(message->GetBoolArray(16))),
        sensor_exp_start_offset (ToOptional(message->GetInt32Array(17))),
        sensor_scan_speed (ToOptional(message->GetFloatArray(18)))
    {}
};

std::ostream& operator<<(std::ostream& os, const CameraModule& obj)
{
    os << "{" << std::endl;
    os << "\"af_info\": " << obj.af_info << "," << std::endl;
    os << "\"id\": " << obj.id << "," << std::endl;
    os << "\"is_enabled\": " << obj.is_enabled << "," << std::endl;
    os << "\"mirror_position\": " << obj.mirror_position << "," << std::endl;
    os << "\"lens_position\": " << obj.lens_position << "," << std::endl;
    os << "\"sensor_analog_gain\": " << obj.sensor_analog_gain << "," << std::endl;
    os << "\"sensor_exposure\": " << obj.sensor_exposure << "," << std::endl;
    os << "\"sensor_data_surface\": " << obj.sensor_data_surface << "," << std::endl;
    os << "\"sensor_temparature\": " << obj.sensor_temparature << "," << std::endl;
    os << "\"sensor_is_horizontal_flip\": " << obj.sensor_is_horizontal_flip << "," << std::endl;
    os << "\"sensor_is_vertical_flip\": " << obj.sensor_is_vertical_flip << "," << std::endl;
    os << "\"sensor_bayer_red_override\": " << obj.sensor_bayer_red_override << "," << std::endl;
    os << "\"sensor_digital_gain\": " << obj.sensor_digital_gain << "," << std::endl;
    os << "\"frame_index\": " << obj.frame_index << "," << std::endl;
    os << "\"sensor_dpc_on\": " << obj.sensor_dpc_on << "," << std::endl;
    os << "\"sensor_exp_start_offset\": " << obj.sensor_exp_start_offset << "," << std::endl;
    os << "\"sensor_scan_speed\": " << obj.sensor_scan_speed << std::endl;
    os << "}";
    return os;
}