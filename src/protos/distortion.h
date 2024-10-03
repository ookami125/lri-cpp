#pragma once

#include "point2f.h"
#include "point2d.h"
#include "rectanglei.h"

struct Polynomial {
    /*1*/ Point2F distortion_center;
    /*2*/ Point2F normalization;
    /*3*/ std::vector<std::pair<uint8_t*, size_t>> coeffs; // [packed = true];
    /*4*/ std::optional<float> fit_cost;
    /*5*/ std::optional<RectangleI> valid_roi;

    Polynomial(picoproto::Message* message) :
        distortion_center (message->GetMessage(1)),
        normalization (message->GetMessage(2)),
        coeffs (message->GetByteArray(3)),
        fit_cost (ToOptional(message->GetFloatArray(4))),
        valid_roi (ToOptional<RectangleI>(message->GetMessageArray(5)))
    {}
};


std::ostream& operator<<(std::ostream& os, const Polynomial& obj)
{
    os << "{" << std::endl;
    os << "\"distortion_center;\": " << obj.distortion_center << "," << std::endl;
    os << "\"normalization;\": " << obj.normalization << "," << std::endl;
    os << "\"coeffs\": \"[packed data]\"" << "," << std::endl;
    os << "\"fit_cost;\": " << obj.fit_cost << "," << std::endl;
    os << "\"valid_roi;\": " << obj.valid_roi << std::endl;
    os << "}";
    return os;
}

struct CRA {
    /*1*/ Point2F distortion_center;
    /*2*/ std::pair<uint8_t*, size_t> sensor_distance;
    /*3*/ std::pair<uint8_t*, size_t> exit_pupil_distance;
    /*4*/ float pixel_size;
    /*5*/ std::vector<Point2D> cra;
    /*6*/ std::vector<Point2F> coeffs;
    /*7*/ std::optional<float> fit_cost;
    /*8*/ std::optional<RectangleI> valid_roi;
    /*9*/ std::optional<float> lens_hall_code;
    /*10*/ std::optional<float> distance_hall_ratio;

    CRA(picoproto::Message* message) :
        distortion_center (message->GetMessage(1)),
        sensor_distance (message->GetBytes(2)),
        exit_pupil_distance (message->GetBytes(3)),
        pixel_size (message->GetFloat(4)),
        cra (To<Point2D>(message->GetMessageArray(5))),
        coeffs (To<Point2F>(message->GetMessageArray(6))),
        fit_cost (ToOptional(message->GetFloatArray(7))),
        valid_roi (ToOptional<RectangleI>(message->GetMessageArray(8))),
        lens_hall_code (ToOptional(message->GetFloatArray(9))),
        distance_hall_ratio (ToOptional(message->GetFloatArray(10)))
    {}
};

std::ostream& operator<<(std::ostream& os, const CRA& obj)
{
    os << "{" << std::endl;
    os << "\"distortion_center\": " << obj.distortion_center << "," << std::endl;
    //os << "\"sensor_distance\": " << obj.sensor_distance << "," << std::endl;
    //os << "\"exit_pupil_distance\": " << obj.exit_pupil_distance << "," << std::endl;
    os << "\"pixel_size\": " << obj.pixel_size << "," << std::endl;
    os << "\"cra\": " << obj.cra << "," << std::endl;
    os << "\"coeffs\": " << obj.coeffs << "," << std::endl;
    os << "\"fit_cost\": " << obj.fit_cost << "," << std::endl;
    os << "\"valid_roi\": " << obj.valid_roi << "," << std::endl;
    os << "\"lens_hall_code\": " << obj.lens_hall_code << "," << std::endl;
    os << "\"distance_hall_ratio\": " << obj.distance_hall_ratio << std::endl;
    os << "}";
    return os;
}

struct Distortion {
    /*1*/ std::optional<Polynomial> polynomial;
    /*2*/ std::optional<CRA> cra;

    Distortion(picoproto::Message* message) : 
        polynomial (ToOptional<Polynomial>(message->GetMessageArray(1))),
        cra (ToOptional<CRA>(message->GetMessageArray(1)))
    {}
};

std::ostream& operator<<(std::ostream& os, const Distortion& obj)
{
    os << "{" << std::endl;
    os << "\"polynomial\": " << obj.polynomial << "," << std::endl;
    os << "\"cra\": " << obj.cra << std::endl;
    os << "}";
    return os;
}