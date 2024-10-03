#pragma once

#include <optional>

#include "mirror_system.h"
#include "matrix3x3f.h"
#include "point3f.h"
#include "distortion.h"
#include "range2f.h"
#include "device_temp.h"
#include "point2f.h"

enum GeometricCalibration_MirrorType {
    NONE = 0,
    GLUED = 1,
    MOVABLE = 2,
};

struct Intrinsics {
    /*1*/ Matrix3x3F k_mat;
    /*2*/ std::optional<float> rms_error;

    Intrinsics(picoproto::Message* message) : 
        k_mat (message->GetMessage(1)),
        rms_error (ToOptional(message->GetFloatArray(2)))
    {}
};

std::ostream& operator<<(std::ostream& os, const Intrinsics& obj)
{
    os << "{" << std::endl;
    os << "\"k_mat\": " << obj.k_mat << "," << std::endl;
    os << "\"rms_error\": " << obj.rms_error << std::endl;
    os << "}";
    return os;
}

struct CanonicalFormat {
    /*1*/ Matrix3x3F rotation;
    /*2*/ Point3F translation;
    /*3*/ std::optional<float> stereo_error;
    /*4*/ std::optional<float> reprojection_error;

    CanonicalFormat(picoproto::Message* message) :
        rotation (message->GetMessage(1)),
        translation (message->GetMessage(2)),
        stereo_error (ToOptional(message->GetFloatArray(3))),
        reprojection_error (ToOptional(message->GetFloatArray(4)))
    {}
};

std::ostream& operator<<(std::ostream& os, const CanonicalFormat& obj)
{
    os << "{" << std::endl;
    os << "\"rotation\": " << obj.rotation << "," << std::endl;
    os << "\"translation\": " << obj.translation << "," << std::endl;
    os << "\"stereo_error\": " << obj.stereo_error << "," << std::endl;
    os << "\"reprojection_error\": " << obj.reprojection_error << std::endl;
    os << "}";
    return os;
}

struct MovableMirrorFormat {
    /*1*/ std::optional<MirrorSystem> mirror_system;
    /*2*/ std::optional<MirrorActuatorMapping> mirror_actuator_mapping;

    MovableMirrorFormat(picoproto::Message* message) :
        mirror_system (ToOptional<MirrorSystem>(message->GetMessageArray(1))),
        mirror_actuator_mapping (ToOptional<MirrorActuatorMapping>(message->GetMessageArray(2)))
    {}
};

std::ostream& operator<<(std::ostream& os, const MovableMirrorFormat& obj)
{
    os << "{" << std::endl;
    os << "\"mirror_system\": " << obj.mirror_system << "," << std::endl;
    os << "\"mirror_actuator_mapping\": " << obj.mirror_actuator_mapping << std::endl;
    os << "}";
    return os;
}

struct Extrinsics {
    /*1*/ std::optional<CanonicalFormat> canonical;
    /*2*/ std::optional<MovableMirrorFormat> moveable_mirror;

    Extrinsics(picoproto::Message* message) :
        canonical (ToOptional<CanonicalFormat>(message->GetMessageArray(1))),
        moveable_mirror (ToOptional<MovableMirrorFormat>(message->GetMessageArray(2)))
    {}
};

std::ostream& operator<<(std::ostream& os, const Extrinsics& obj)
{
    os << "{" << std::endl;
    os << "\"canonical\": " << obj.canonical << "," << std::endl;
    os << "\"moveable_mirror\": " << obj.moveable_mirror << std::endl;
    os << "}";
    return os;
}

struct CalibrationFocusBundle {
    /*1*/ float focus_distance;
    /*2*/ std::optional<Intrinsics> intrinsics;
    /*3*/ std::optional<Extrinsics> extrinsics;
    /*4*/ std::optional<int32_t> sensor_temp;
    /*5*/ std::optional<DeviceTemp> device_temp;
    /*6*/ std::optional<float> focus_hall_code;

    CalibrationFocusBundle(picoproto::Message* message) :
        focus_distance (message->GetFloat(1)),
        intrinsics (ToOptional<Intrinsics>(message->GetMessageArray(2))),
        extrinsics (ToOptional<Extrinsics>(message->GetMessageArray(3))),
        sensor_temp (ToOptional<int32_t>(message->GetInt64Array(4))),
        device_temp (ToOptional<DeviceTemp>(message->GetMessageArray(5))),
        focus_hall_code (ToOptional(message->GetFloatArray(6)))
    {}
};

std::ostream& operator<<(std::ostream& os, const CalibrationFocusBundle& obj)
{
    os << "{" << std::endl;
    os << "\"focus_distance\": " << obj.focus_distance << "," << std::endl;
    os << "\"intrinsics\": " << obj.intrinsics << "," << std::endl;
    os << "\"extrinsics\": " << obj.extrinsics << "," << std::endl;
    os << "\"sensor_temp\": " << obj.sensor_temp << "," << std::endl;
    os << "\"device_temp\": " << obj.device_temp << "," << std::endl;
    os << "\"focus_hall_code\": " << obj.focus_hall_code << std::endl;
    os << "}";
    return os;
}

struct AngleOpticalCenterMapping {
    /*1*/ Point2F center_start;
    /*2*/ Point2F center_end;
    /*3*/ float angle_offset;
    /*4*/ float t_scale;
    /*5*/ float t_offset;

    AngleOpticalCenterMapping(picoproto::Message* message) :
        center_start (message->GetMessage(1)),
        center_end (message->GetMessage(2)),
        angle_offset (message->GetFloat(3)),
        t_scale (message->GetFloat(4)),
        t_offset (message->GetFloat(5))
    {}
};

std::ostream& operator<<(std::ostream& os, const AngleOpticalCenterMapping& obj)
{
    os << "{" << std::endl;
    os << "\"center_start\": " << obj.center_start << "," << std::endl;
    os << "\"center_end\": " << obj.center_end << "," << std::endl;
    os << "\"angle_offset\": " << obj.angle_offset << "," << std::endl;
    os << "\"t_scale\": " << obj.t_scale << "," << std::endl;
    os << "\"t_offset\": " << obj.t_offset << std::endl;
    os << "}";
    return os;
}

struct GeometricCalibration {
    /*1*/ std::optional<GeometricCalibration_MirrorType> mirror_type;
    /*2*/ std::vector<CalibrationFocusBundle> per_focus_calibration;
    /*3*/ std::optional<Distortion> distortion;
    /*4*/ std::optional<Range2F> lens_hall_code_range;
    /*5*/ std::optional<Range2F> focus_distance_range;
    /*6*/ std::optional<AngleOpticalCenterMapping> angle_optical_center_mapping;

    GeometricCalibration(picoproto::Message* message) :
        mirror_type (ToOptional<GeometricCalibration_MirrorType>(message->GetUInt64Array(1))),
        per_focus_calibration (To<CalibrationFocusBundle>(message->GetMessageArray(2))),
        distortion (ToOptional<Distortion>(message->GetMessageArray(3))),
        lens_hall_code_range (ToOptional<Range2F>(message->GetMessageArray(4))),
        focus_distance_range (ToOptional<Range2F>(message->GetMessageArray(5))),
        angle_optical_center_mapping (ToOptional<AngleOpticalCenterMapping>(message->GetMessageArray(6)))
    {} 
};

std::ostream& operator<<(std::ostream& os, const GeometricCalibration& obj)
{
    os << "{" << std::endl;
    os << "\"mirror_type\": " << obj.mirror_type << "," << std::endl;
    os << "\"per_focus_calibration\": " << obj.per_focus_calibration << "," << std::endl;
    os << "\"distortion\": " << obj.distortion << "," << std::endl;
    os << "\"lens_hall_code_range\": " << obj.lens_hall_code_range << "," << std::endl;
    os << "\"focus_distance_range\": " << obj.focus_distance_range << "," << std::endl;
    os << "\"angle_optical_center_mapping\": " << obj.angle_optical_center_mapping << std::endl;
    os << "}";
    return os;
}