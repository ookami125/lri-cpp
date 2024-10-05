#pragma once

#include "../picoproto.h"

#include "camera_id.h"
#include "rectanglei.h"

#include "utils.h"

struct ROI {
    /*1*/ std::optional<uint32_t> id;
    /*2*/ std::optional<RectangleI> roi;
    /*3*/ std::optional<float> confidence;

    ROI(picoproto::Message* message) :
        id (ToOptional(message->GetUInt32Array(1))),
        roi (ToOptional<RectangleI>(message->GetMessageArray(1))),
        confidence (ToOptional(message->GetFloatArray(1)))
    {}
};

std::ostream& operator<<(std::ostream& os, const ROI& obj)
{
    os << "{" << std::endl;
    os << "\"id\": " << obj.id << "," << std::endl;
    os << "\"roi\": " << obj.roi << "," << std::endl;
    os << "\"confidence\": " << obj.confidence << std::endl;
    os << "}";
    return os;
}

struct FaceData {
    /*1*/ CameraID id;
    /*2*/ uint32_t frame_index;
    /*3*/ std::vector<ROI> rois;

    FaceData(picoproto::Message* message) :
        id ((CameraID) message->GetUInt64(1)),
        frame_index ((uint32_t)message->GetInt64(2)),
        rois (To<ROI>(message->GetMessageArray(3)))
    {}
};

std::ostream& operator<<(std::ostream& os, const FaceData& obj)
{
    os << "{" << std::endl;
    os << "\"id\": " << obj.id << "," << std::endl;
    os << "\"frame_index\": " << obj.frame_index << "," << std::endl;
    os << "\"rois\": " << obj.rois << std::endl;
    os << "}";
    return os;
}