#pragma once
#include "../picoproto.h"

#include "matrix3x3f.h"
#include "point3f.h"
#include "range2f.h"

struct MirrorSystem {
    /*1*/ Point3F real_camera_location;
    /*2*/ Matrix3x3F real_camera_orientation;
    /*3*/ Point3F rotation_axis;
    /*4*/ Point3F point_on_rotation_axis;
    /*5*/ float distance_mirror_plane_to_point_on_rotation_axis;
    /*6*/ Point3F mirror_normal_at_zero_degrees;
    /*7*/ bool flip_img_around_x;
    /*8*/ Range2F mirror_angle_range;
    /*9*/ std::optional<float> reprojection_error;

    MirrorSystem(picoproto::Message* message) :
        real_camera_location (message->GetMessage(1)),
        real_camera_orientation (message->GetMessage(2)),
        rotation_axis (message->GetMessage(3)),
        point_on_rotation_axis (message->GetMessage(4)),
        distance_mirror_plane_to_point_on_rotation_axis (message->GetFloat(5)),
        mirror_normal_at_zero_degrees (message->GetMessage(6)),
        flip_img_around_x (message->GetBool(7)),
        mirror_angle_range (message->GetMessage(8)),
        reprojection_error (ToOptional(message->GetFloatArray(9)))
    {}
};

std::ostream& operator<<(std::ostream& os, const MirrorSystem& obj)
{
    os << "{" << std::endl;
    os << "\"real_camera_location\": " << obj.real_camera_location << "," << std::endl;
    os << "\"real_camera_orientation\": " << obj.real_camera_orientation << "," << std::endl;
    os << "\"rotation_axis\": " << obj.rotation_axis << "," << std::endl;
    os << "\"point_on_rotation_axis\": " << obj.point_on_rotation_axis << "," << std::endl;
    os << "\"distance_mirror_plane_to_point_on_rotation_axis\": " << obj.distance_mirror_plane_to_point_on_rotation_axis << "," << std::endl;
    os << "\"mirror_normal_at_zero_degrees\": " << obj.mirror_normal_at_zero_degrees << "," << std::endl;
    os << "\"flip_img_around_x\": " << obj.flip_img_around_x << "," << std::endl;
    os << "\"mirror_angle_range\": " << obj.mirror_angle_range << "," << std::endl;
    os << "\"reprojection_error\": " << obj.reprojection_error << std::endl;
    os << "}";
    return os;
}

enum TransformationType {
    MEAN_STD_NORMALIZE = 0,
    TAN_HALF_THETA = 1,
};

struct ActuatorAnglePair {
    /*1*/ int32_t hall_code;
    /*2*/ float angle;

    ActuatorAnglePair(picoproto::Message* message) :
        hall_code ((int32_t)message->GetInt64(1)),
        angle (message->GetFloat(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const ActuatorAnglePair& obj)
{
    os << "{" << std::endl;
    os << "\"hall_code\": " << obj.hall_code << "," << std::endl;
    os << "\"angle\": " << obj.angle << std::endl;
    os << "}";
    return os;
}

struct QuadraticModel {
    /*1*/ bool use_rplus_for_left_segment;
    /*2*/ bool use_rplus_for_right_segment;
    /*3*/ float inflection_value;
    /*4*/ std::vector<std::pair<uint8_t*, size_t>> model_coeffs;// [packed = true];

    QuadraticModel(picoproto::Message* message) :
        use_rplus_for_left_segment (message->GetBool(1)),
        use_rplus_for_right_segment (message->GetBool(2)),
        inflection_value (message->GetFloat(3)),
        model_coeffs (message->GetByteArray(4))
    {}
};

std::ostream& operator<<(std::ostream& os, const QuadraticModel& obj)
{
    os << "{" << std::endl;
    os << "\"use_rplus_for_left_segment\": " << obj.use_rplus_for_left_segment << "," << std::endl;
    os << "\"use_rplus_for_right_segment\": " << obj.use_rplus_for_right_segment << "," << std::endl;
    os << "\"inflection_value\": " << obj.inflection_value << "," << std::endl;
    os << "\"model_coeffs\": \"[packed data]\"" << std::endl;
    os << "}";
    return os;
}

struct MirrorActuatorMapping {
    /*1*/ TransformationType transformation_type;
    /*2*/ float actuator_length_offset;
    /*3*/ float actuator_length_scale;
    /*4*/ float mirror_angle_offset;
    /*5*/ float mirror_angle_scale;
    /*6*/ std::vector<ActuatorAnglePair> actuator_angle_pair_vec;
    /*7*/ QuadraticModel quadratic_model;
    /*8*/ std::optional<float> angle_to_hall_code_error;
    /*9*/ std::optional<float> hall_code_to_angle_error;
    /*10*/ std::optional<Range2F> hall_code_range;

    MirrorActuatorMapping(picoproto::Message* message) :
        transformation_type ((TransformationType)message->GetUInt64(1)),
        actuator_length_offset (message->GetFloat(2)),
        actuator_length_scale (message->GetFloat(3)),
        mirror_angle_offset (message->GetFloat(4)),
        mirror_angle_scale (message->GetFloat(5)),
        actuator_angle_pair_vec (To<ActuatorAnglePair>(message->GetMessageArray(6))),
        quadratic_model (message->GetMessage(7)),
        angle_to_hall_code_error (ToOptional(message->GetFloatArray(8))),
        hall_code_to_angle_error (ToOptional(message->GetFloatArray(9))),
        hall_code_range (ToOptional<Range2F>(message->GetMessageArray(10)))
    {}
};

std::ostream& operator<<(std::ostream& os, const MirrorActuatorMapping& obj)
{
    os << "{" << std::endl;
    os << "\"transformation_type\": " << obj.transformation_type << "," << std::endl;
    os << "\"actuator_length_offset\": " << obj.actuator_length_offset << "," << std::endl;
    os << "\"actuator_length_scale\": " << obj.actuator_length_scale << "," << std::endl;
    os << "\"mirror_angle_offset\": " << obj.mirror_angle_offset << "," << std::endl;
    os << "\"mirror_angle_scale\": " << obj.mirror_angle_scale << "," << std::endl;
    os << "\"actuator_angle_pair_vec\": " << obj.actuator_angle_pair_vec << "," << std::endl;
    os << "\"quadratic_model\": " << obj.quadratic_model << "," << std::endl;
    os << "\"angle_to_hall_code_error\": " << obj.angle_to_hall_code_error << "," << std::endl;
    os << "\"hall_code_to_angle_error\": " << obj.hall_code_to_angle_error << "," << std::endl;
    os << "\"hall_code_range\": " << obj.hall_code_range << std::endl;
    os << "}";
    return os;
}