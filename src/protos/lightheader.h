#pragma once

#include "../picoproto.h"

#include <vector>
#include <optional>
#include <stdint.h>
#include <string>
#include <iostream>
#include "utils.h"

#include "camera_module.h"
#include "camera_id.h"
#include "time_stamp.h"
#include "geometric_calibration.h"
#include "color_calibration.h"
#include "sensor_characterization.h"
#include "vignetting_characterization.h"
#include "hot_pixel_map.h"
#include "device_temp.h"
#include "dead_pixel_map.h"
#include "tof_calibration.h"
#include "flash_calibration.h"
#include "sensor_type.h"
#include "hw_info.h"
#include "view_preferences.h"
#include "proximity_sensors.h"
#include "imu_data.h"
#include "gps_data.h"
#include "face_data.h"

struct FactoryDeviceCalibration {
    /*1*/ std::optional<FlashCalibration> flash;
    /*2*/ std::optional<ToFCalibration> tof;
    /*3*/ std::optional<TimeStamp> time_stamp;

    FactoryDeviceCalibration(picoproto::Message* message) :
        flash (ToOptional<FlashCalibration>(message->GetMessageArray(1))),
        tof (ToOptional<ToFCalibration>(message->GetMessageArray(2))),
        time_stamp (ToOptional<TimeStamp>(message->GetMessageArray(3)))
    {}
};

std::ostream& operator<<(std::ostream& os, const FactoryDeviceCalibration& obj)
{
    os << "{" << std::endl;
    os << "\"flash\": " << obj.flash << "," << std::endl;
    os << "\"tof\": " << obj.tof << "," << std::endl;
    os << "\"time_stamp\": " << obj.time_stamp << std::endl; 
    return os << "}";
}

struct FactoryModuleCalibration {
    /*1*/ CameraID camera_id;
    /*2*/ std::vector<ColorCalibration> color;
    /*3*/ std::optional<GeometricCalibration> geometry;
    /*4*/ std::optional<VignettingCharacterization> vignetting;
    /*5*/ std::optional<HotPixelMap> hot_pixel_map;
    /*6*/ std::optional<DeadPixelMap> dead_pixel_map;
    /*7*/ std::optional<TimeStamp> time_stamp;

    FactoryModuleCalibration(picoproto::Message* message) :
        camera_id ((CameraID)message->GetUInt64(1)),
        color (To<ColorCalibration>(message->GetMessageArray(2))),
        geometry (ToOptional(message->GetMessageArray(3))),
        vignetting (ToOptional(message->GetMessageArray(4))),
        hot_pixel_map (ToOptional(message->GetMessageArray(5))),
        dead_pixel_map (ToOptional(message->GetMessageArray(6))),
        time_stamp (ToOptional(message->GetMessageArray(7)))
    {}
};

std::ostream& operator<<(std::ostream& os, const FactoryModuleCalibration& obj)
{
    os << "{" << std::endl;
    os << "\"camera_id\": " << obj.camera_id << "," << std::endl; 
    os << "\"color\": " << obj.color << "," << std::endl;
    os << "\"geometry\": " << obj.geometry << "," << std::endl;
    os << "\"vignetting\": " << obj.vignetting << "," << std::endl;
    os << "\"hot_pixel_map\": " << obj.hot_pixel_map << "," << std::endl;
    os << "\"dead_pixel_map\": " << obj.dead_pixel_map << "," << std::endl;
    os << "\"time_stamp\": " << obj.time_stamp << std::endl; 
    return os << "}";
}

struct ColorCalibrationGold {
    /*1*/ CameraID camera_id;
    /*2*/ ColorCalibration data;
    /*3*/ std::optional<TimeStamp> time_stamp;

    ColorCalibrationGold(picoproto::Message* message) :
        camera_id ((CameraID)message->GetUInt64(1)),
        data (message->GetMessage(2)),
        time_stamp (ToOptional<TimeStamp>(message->GetMessageArray(3)))
    {}
};

std::ostream& operator<<(std::ostream& os, const ColorCalibrationGold& obj)
{
    os << "{" << std::endl;
    os << "\"camera_id\": " << obj.camera_id << "," << std::endl; 
    os << "\"data\": " << obj.data << "," << std::endl;
    os << "\"time_stamp\": " << obj.time_stamp << std::endl; 
    return os << "}";
}

struct SensorData {
    /*1*/ SensorType type;
    /*2*/ SensorCharacterization data;
    /*3*/ std::optional<TimeStamp> time_stamp;

    SensorData(picoproto::Message* message) :
        type ((SensorType)message->GetInt64(1)),
        data (message->GetMessage(2)),
        time_stamp (ToOptional<TimeStamp>(message->GetMessageArray(3)))
    {}
};

std::ostream& operator<<(std::ostream& os, const SensorData& obj)
{
    os << "{" << std::endl;
    os << "\"type\": " << obj.type << "," << std::endl; 
    os << "\"data\": " << obj.data << "," << std::endl;
    os << "\"time_stamp\": " << obj.time_stamp << std::endl; 
    return os << "}";
}

enum Mode {
    NOFLASH = 0,
    FLASH = 1,
    TORCH = 2,
};

struct FlashData {
    /*1*/ Mode mode;
    /*2*/ std::optional<uint32_t> ledcool_current;
    /*3*/ std::optional<uint32_t> ledwarm_current;
    /*4*/ std::optional<uint32_t> flash_duration;
    /*5*/ std::optional<int32_t> offset_duration;

    FlashData(picoproto::Message* message) :
        mode ((Mode)message->GetUInt64(1)),
        ledcool_current (ToOptional(message->GetUInt32Array(2))),
        ledwarm_current (ToOptional(message->GetUInt32Array(3))),
        flash_duration (ToOptional(message->GetUInt32Array(4))),
        offset_duration (ToOptional(message->GetInt32Array(5)))
    {}
};

std::ostream& operator<<(std::ostream& os, const FlashData& obj)
{
    os << "{" << std::endl;
    os << "\"mode\": " << obj.mode << "," << std::endl; 
    os << "\"ledcool_current\": " << obj.ledcool_current << "," << std::endl;
    os << "\"ledwarm_current\": " << obj.ledwarm_current << "," << std::endl;
    os << "\"flash_duration\": " << obj.flash_duration << "," << std::endl;
    os << "\"offset_duration\": " << obj.offset_duration << std::endl; 
    return os << "}";
}

struct AFDebugInfo {
    /*1*/ std::optional<bool> focus_achieved;
    /*2*/ std::optional<int32_t> image_focal_length;

    AFDebugInfo(picoproto::Message* message) :
        focus_achieved (ToOptional(message->GetBoolArray(1))),
        image_focal_length (ToOptional(message->GetInt32Array(2)))
    {}
};

std::ostream& operator<<(std::ostream& os, const AFDebugInfo& obj)
{
    os << "{" << std::endl;
    os << "\"focus_achieved\": " << obj.focus_achieved << "," << std::endl; 
    os << "\"image_focal_length\": " << obj.image_focal_length << std::endl; 
    return os << "}";
}

struct Compatibility {
    /*1*/ uint32_t version;
    /*2*/ std::string features;

    Compatibility(picoproto::Message* message) :
        version ((uint32_t)message->GetUInt64(1)),
        features (message->GetString(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Compatibility& obj)
{
    os << "{" << std::endl;
    os << "\"version\": " << obj.version << "," << std::endl; 
    os << "\"features\": \"" << obj.features << "\"" << std::endl; 
    return os << "}";
}

struct LightHeader {
    /*1*/ std::optional<uint64_t> image_unique_id_low;
    /*2*/ std::optional<uint64_t> image_unique_id_high;
    /*3*/ std::optional<TimeStamp> image_time_stamp;
    /*4*/ std::optional<int32_t> image_focal_length;
    /*5*/ std::optional<CameraID> image_reference_camera;
    /*6*/ std::optional<uint64_t> device_unique_id_low;
    /*7*/ std::optional<uint64_t> device_unique_id_high;
    /*8*/ std::optional<std::string> device_model_name;
    /*9*/ std::optional<std::string> device_fw_version;
    /*10*/ std::optional<std::string> device_asic_fw_version;
    /*11*/ std::optional<DeviceTemp> device_temperature;
    /*12*/ std::vector<CameraModule> modules;
    /*13*/ std::vector<FactoryModuleCalibration> module_calibration;
    /*14*/ std::vector<FactoryDeviceCalibration> device_calibration;
    /*15*/ std::vector<ColorCalibrationGold> gold_cc;
    /*16*/ std::vector<SensorData> sensor_data;
    /*17*/ std::vector<float> tof_range;
    /*18*/ std::vector<HwInfo> hw_info;
    /*19*/ std::vector<ViewPreferences> view_preferences;
    /*20*/ std::vector<ProximitySensors> proximity_sensors;
    /*22*/ std::vector<FlashData> flash_data;
    /*23*/ std::vector<IMUData> imu_data;
    /*24*/ std::vector<AFDebugInfo> af_info;
    /*25*/ std::vector<GPSData> gps_data;
    /*26*/ std::vector<Compatibility> compatibility;
    /*27*/ std::vector<FaceData> face_data;

    LightHeader(picoproto::Message* message) :
        image_unique_id_low (ToOptional(message->GetUInt64Array(1))),
        image_unique_id_high (ToOptional(message->GetUInt64Array(2))),
        image_time_stamp (ToOptional<TimeStamp>(message->GetMessageArray(3))),
        image_focal_length (ToOptional(message->GetInt32Array(4))),
        image_reference_camera (ToOptional<CameraID>(message->GetUInt32Array(5))),
        device_unique_id_low (ToOptional(message->GetUInt64Array(6))),
        device_unique_id_high (ToOptional(message->GetUInt64Array(7))),
        device_model_name (ToOptional(message->GetStringArray(8))),
        device_fw_version (ToOptional(message->GetStringArray(9))),
        device_asic_fw_version (ToOptional(message->GetStringArray(10))),
        device_temperature (ToOptional<DeviceTemp>(message->GetMessageArray(11))),
        modules (To<CameraModule>(message->GetMessageArray(12))),
        module_calibration (To<FactoryModuleCalibration>(message->GetMessageArray(13))),
        device_calibration (To<FactoryDeviceCalibration>(message->GetMessageArray(14))),
        gold_cc (To<ColorCalibrationGold>(message->GetMessageArray(15))),
        sensor_data (To<SensorData>(message->GetMessageArray(16))),
        tof_range (message->GetFloatArray(17)),
        hw_info (To<HwInfo>(message->GetMessageArray(18))),
        view_preferences (To<ViewPreferences>(message->GetMessageArray(19))),
        proximity_sensors (To<ProximitySensors>(message->GetMessageArray(20))),
        flash_data (To<FlashData>(message->GetMessageArray(22))),
        imu_data (To<IMUData>(message->GetMessageArray(23))),
        af_info (To<AFDebugInfo>(message->GetMessageArray(24))),
        gps_data (To<GPSData>(message->GetMessageArray(25))),
        compatibility (To<Compatibility>(message->GetMessageArray(26))),
        face_data (To<FaceData>(message->GetMessageArray(27)))
    {}
};

std::ostream& operator<<(std::ostream& os, const LightHeader& obj)
{
    os << "\"Lightheader\": {" << std::endl;
    os << "\"image_unique_id_low\": " << obj.image_unique_id_low << "," << std::endl;
    os << "\"image_unique_id_high\": " << obj.image_unique_id_high << "," << std::endl;
    os << "\"image_time_stamp\": " << obj.image_time_stamp << "," << std::endl;
    os << "\"image_focal_length\": " << obj.image_focal_length << "," << std::endl;
    os << "\"image_reference_camera\": " << obj.image_reference_camera << "," << std::endl;
    os << "\"device_unique_id_low\": " << obj.device_unique_id_low << "," << std::endl;
    os << "\"device_unique_id_high\": " << obj.device_unique_id_high << "," << std::endl;
    os << "\"device_model_name\": \"" << obj.device_model_name << "\"," << std::endl;
    os << "\"device_fw_version\": \"" << obj.device_fw_version << "\"," << std::endl;
    os << "\"device_asic_fw_version\": " << obj.device_asic_fw_version << "," << std::endl;
    os << "\"device_temperature\": " << obj.device_temperature << "," << std::endl;
    os << "\"modules\": " << obj.modules << "," << std::endl;
    os << "\"module_calibration\": " << obj.module_calibration << "," << std::endl;
    os << "\"device_calibration\": " << obj.device_calibration << "," << std::endl;
    os << "\"gold_cc\": " << obj.gold_cc << "," << std::endl;
    os << "\"sensor_data\": " << obj.sensor_data << "," << std::endl;
    os << "\"tof_range\": " << obj.tof_range << "," << std::endl;
    os << "\"hw_info\": " << obj.hw_info << "," << std::endl;
    os << "\"view_preferences\": " << obj.view_preferences << "," << std::endl;
    os << "\"proximity_sensors\": " << obj.proximity_sensors << "," << std::endl;
    os << "\"flash_data\": " << obj.flash_data << "," << std::endl;
    os << "\"imu_data\": " << obj.imu_data << "," << std::endl;
    os << "\"af_info\": " << obj.af_info << "," << std::endl;
    os << "\"gps_data\": " << obj.gps_data << "," << std::endl;
    os << "\"compatibility\": " << obj.compatibility << "," << std::endl;
    os << "\"face_data\": " << obj.face_data << std::endl;
    return os << "}";
}