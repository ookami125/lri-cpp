#pragma once

#include "../picoproto.h"

#include "sensor_type.h"
#include "camera_id.h"

#include <vector>
#include <optional>
#include <string>

#include "utils.h"

enum LensType {
    LENS_UNKNOWN = 0,
    LENS_SHOWIN = 1,
    LENS_LARGAN = 2,
    LENS_SUNNY = 3,
    LENS_KANTATSU = 4,
};

enum MirrorActuatorType {
    MIRROR_ACTUATOR_UNKNOWN = 0,
    MIRROR_ACTUATOR_PZT = 1,
};

enum CameraModuleHwInfo_MirrorType {
    MIRROR_UNKNOWN = 0,
    MIRROR_DIELECTRIC_SNX = 1,
    MIRROR_SILVER_ZUISHO = 2,
};

enum LensActuatorType {
    LENS_ACTUATOR_UNKNOWN = 0,
    LENS_ACTUATOR_SHICOH = 1,
    LENS_ACTUATOR_PZT = 2,
};

struct CameraModuleHwInfo {
    /**/ CameraID id;
    /**/ SensorType sensor;
    /**/ std::optional<LensType> lens; // [default = LENS_UNKNOWN];
    /**/ std::optional<MirrorActuatorType> mirror_actuator; // [default = MIRROR_ACTUATOR_UNKNOWN];
    /**/ std::optional<CameraModuleHwInfo_MirrorType> mirror; // [default = MIRROR_UNKNOWN];

    CameraModuleHwInfo(picoproto::Message* message) :
        id ((CameraID)message->GetUInt64(1)),
        sensor ((SensorType)message->GetInt64(2)),
        lens (ToOptional<LensType>(message->GetUInt64Array(3))),
        mirror_actuator (ToOptional<MirrorActuatorType>(message->GetUInt64Array(4))),
        mirror (ToOptional<CameraModuleHwInfo_MirrorType>(message->GetUInt64Array(5)))
    {}
};

std::ostream& operator<<(std::ostream& os, const CameraModuleHwInfo& obj)
{
    os << "{" << std::endl;
    os << "\"id\": " << obj.id << "," << std::endl; 
    os << "\"sensor\": " << obj.sensor << "," << std::endl;
    os << "\"lens\": " << obj.lens << "," << std::endl;
    os << "\"mirror_actuator\": " << obj.mirror_actuator << "," << std::endl;
    os << "\"mirror\": " << obj.mirror << std::endl; 
    return os << "}";
}

enum FlashType {
    FLASH_UNKNOWN = 0,
    FLASH_OSRAM_CBLPM1 = 1,
};

enum ToFType {
    TOF_UNKNOWN = 0,
    TOF_STMICRO_VL53L0 = 1,
};

enum ModelType {
    MODEL_P1 = 0,
    MODEL_P1_1 = 1,
    MODEL_P2 = 2,
};

struct HwInfo {
    /*1*/ std::vector<CameraModuleHwInfo> camera;
    /*2*/ std::optional<FlashType> flash; // [default = FLASH_UNKNOWN];    
    /*3*/ std::optional<ToFType> tof; // [default = TOF_UNKNOWN];
    /*4*/ std::optional<ModelType> model;
    /*5*/ std::optional<std::string> manufacturer;

    HwInfo(picoproto::Message* message) :
        camera (To<CameraModuleHwInfo>(message->GetMessageArray(1))),
        flash (ToOptional<FlashType>(message->GetInt64Array(2))),
        tof (ToOptional<ToFType>(message->GetInt64Array(3))),
        model (ToOptional<ModelType>(message->GetInt64Array(4))),
        manufacturer (ToOptional(message->GetStringArray(5)))
    {}
};

std::ostream& operator<<(std::ostream& os, const HwInfo& obj)
{
    os << "{" << std::endl;
    os << "\"camera\": " << obj.camera << "," << std::endl; 
    os << "\"flash\": " << obj.flash << "," << std::endl;
    os << "\"tof\": " << obj.tof << "," << std::endl;
    os << "\"model\": " << obj.model << "," << std::endl;
    os << "\"manufacturer\": \"" << obj.manufacturer << "\"" << std::endl; 
    return os << "}";
}