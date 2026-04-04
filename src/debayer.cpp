#include "debayer.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include "image.h"
#include "utils.h"

uint16_t average(uint16_t lhs, uint16_t rhs) {
    return (uint16_t)((lhs >> 1) + (rhs >> 1u) + (lhs & rhs & 1));
}

Image debayerImage(Image* srcImage, uint8_t bayerPatternOffset, DemosaicMode mode) {

    const int R=0, G=1, B=2;
    char map[] = { R,G,G,B };
    if(bayerPatternOffset & 0x1) {
        swap(map[0], map[1]);
        swap(map[2], map[3]);
    }
    if(bayerPatternOffset & 0x2) {
        swap(map[0], map[2]);
        swap(map[1], map[3]);
    }

    // Pull out RGB values into their seperate channels
    Image image = Image(srcImage->width, srcImage->height, 3);
    for(int32_t y=0; y<(int32_t)srcImage->height; ++y) {
        for(int32_t x=0; x<(int32_t)srcImage->width; ++x) {
            char id = map[((y%2)<<1) | x%2];
            image.setColor(x, y, id, srcImage->getColor(x, y, 0));
        }
    }

    switch(mode) {
        default:
        case DemosaicMode::Photosite:
            break;

        case DemosaicMode::Interpolate: {
            //Interpolate RGB values to other pixels
            Image debayeredImage = image;
            for(int32_t y=0; y<(int64_t)image.height; y++) {
                for(int32_t x=0; x<(int64_t)image.width; x++) {
                    char id = map[((y%2)<<1) | x % 2];
                    switch(id) {
                        case R: {
                            uint16_t g = average(
                                average(image.getColorOrMirror(x-1, y, 1), image.getColorOrMirror(x, y-1, 1)),
                                average(image.getColorOrMirror(x+1, y, 1), image.getColorOrMirror(x, y+1, 1)));
                            debayeredImage.setColor(x, y, 1, g);
                            uint16_t b = average(
                                average(image.getColorOrMirror(x-1, y-1, 2), image.getColorOrMirror(x-1, y+1, 2)),
                                average(image.getColorOrMirror(x+1, y-1, 2), image.getColorOrMirror(x+1, y+1, 2)));
                            debayeredImage.setColor(x, y, 2, b);
                        } break;
                        case G: {
                            uint16_t r = average(
                                image.getColorOrMirror(x-1, y, 0) + image.getColorOrMirror(x, y-1, 0),
                                image.getColorOrMirror(x+1, y, 0) + image.getColorOrMirror(x, y+1, 0));
                            debayeredImage.setColor(x, y, 0, r);
                            uint16_t b = average(
                                image.getColorOrMirror(x-1, y, 2) + image.getColorOrMirror(x, y-1, 2),
                                image.getColorOrMirror(x+1, y, 2) + image.getColorOrMirror(x, y+1, 2));
                            debayeredImage.setColor(x, y, 2, b);
                        } break;
                        case B: {
                            uint16_t r = average(
                                average(image.getColorOrMirror(x-1, y-1, 0), image.getColorOrMirror(x-1, y+1, 0)),
                                average(image.getColorOrMirror(x+1, y-1, 0), image.getColorOrMirror(x+1, y+1, 0)));
                            debayeredImage.setColor(x, y, 0, r);
                            uint16_t g = average(
                                average(image.getColorOrMirror(x-1, y, 1), image.getColorOrMirror(x, y-1, 1)),
                                average(image.getColorOrMirror(x+1, y, 1), image.getColorOrMirror(x, y+1, 1)));
                            debayeredImage.setColor(x, y, 1, g);
                        } break;
                    }
                }
            }
            image = std::move(debayeredImage);
        } break;
        case DemosaicMode::LMMSE: {
            Image lmmseImage = image;

            auto clampSensor = [](float value) -> uint16_t {
                value = std::clamp(value, 0.0f, 1023.0f);
                return (uint16_t)std::lround(value);
            };

            auto weightFromGradient = [](float gradient) -> float {
                return 1.0f / (gradient + 1.0f);
            };

            auto sample = [&](int32_t sx, int32_t sy, int channel) -> float {
                return (float)lmmseImage.getColorOrMirror(sx, sy, channel);
            };

            // 1) Estimate missing green using directional LMMSE weighting.
            for(int32_t y=0; y<(int32_t)lmmseImage.height; ++y) {
                for(int32_t x=0; x<(int32_t)lmmseImage.width; ++x) {
                    char id = map[((y & 1) << 1) | (x & 1)];
                    if(id == G) {
                        continue;
                    }

                    float center = sample(x, y, id);

                    float leftG = sample(x - 1, y, G);
                    float rightG = sample(x + 1, y, G);
                    float upG = sample(x, y - 1, G);
                    float downG = sample(x, y + 1, G);

                    float farLeft = sample(x - 2, y, id);
                    float farRight = sample(x + 2, y, id);
                    float farUp = sample(x, y - 2, id);
                    float farDown = sample(x, y + 2, id);

                    float gh = 0.5f * (leftG + rightG) + 0.25f * (2.0f * center - farLeft - farRight);
                    float gv = 0.5f * (upG + downG) + 0.25f * (2.0f * center - farUp - farDown);

                    float gradH = std::fabs(rightG - leftG) + std::fabs(farRight - center) + std::fabs(center - farLeft);
                    float gradV = std::fabs(downG - upG) + std::fabs(farDown - center) + std::fabs(center - farUp);

                    float wH = weightFromGradient(gradH);
                    float wV = weightFromGradient(gradV);
                    float green = (wH * gh + wV * gv) / (wH + wV);
                    lmmseImage.setColor(x, y, G, clampSensor(green));
                }
            }

            auto directionalEstimate = [&](int32_t px, int32_t py, int channel, int dx, int dy) -> std::pair<float, float> {
                float baseG = sample(px, py, G);
                float posC = sample(px + dx, py + dy, channel);
                float negC = sample(px - dx, py - dy, channel);
                float posG = sample(px + dx, py + dy, G);
                float negG = sample(px - dx, py - dy, G);

                float diff = ((posC - posG) + (negC - negG)) * 0.5f;
                float estimate = baseG + diff;

                float localGradient = std::fabs(posG - negG);
                localGradient += 0.5f * std::fabs(sample(px + 2 * dx, py + 2 * dy, G) - sample(px - 2 * dx, py - 2 * dy, G));
                localGradient += 0.25f * std::fabs((posC - posG) - (negC - negG));

                float weight = weightFromGradient(localGradient);
                return {estimate, weight};
            };

            auto blendEstimates = [](const std::vector<std::pair<float, float>>& estimates) -> float {
                float numerator = 0.0f;
                float denominator = 0.0f;
                for(const auto& est : estimates) {
                    numerator += est.first * est.second;
                    denominator += est.second;
                }
                if(denominator <= 0.0f) {
                    return estimates.empty() ? 0.0f : estimates.front().first;
                }
                return numerator / denominator;
            };

            // 2) Reconstruct red & blue on green pixels using axis-aligned neighbours.
            for(int32_t y=0; y<(int32_t)lmmseImage.height; ++y) {
                for(int32_t x=0; x<(int32_t)lmmseImage.width; ++x) {
                    char id = map[((y & 1) << 1) | (x & 1)];
                    if(id != G) {
                        continue;
                    }

                    int xParity = x & 1;
                    int yParity = y & 1;
                    char horizColor = map[(yParity << 1) | (xParity ^ 1)];

                    if(horizColor == R) {
                        auto est = directionalEstimate(x, y, R, 1, 0);
                        lmmseImage.setColor(x, y, R, clampSensor(est.first));
                    } else {
                        auto est = directionalEstimate(x, y, R, 0, 1);
                        lmmseImage.setColor(x, y, R, clampSensor(est.first));
                    }

                    if(horizColor == B) {
                        auto est = directionalEstimate(x, y, B, 1, 0);
                        lmmseImage.setColor(x, y, B, clampSensor(est.first));
                    } else {
                        auto est = directionalEstimate(x, y, B, 0, 1);
                        lmmseImage.setColor(x, y, B, clampSensor(est.first));
                    }
                }
            }

            // 3) Reconstruct the last channel on red & blue pixels using diagonal neighbours.
            for(int32_t y=0; y<(int32_t)lmmseImage.height; ++y) {
                for(int32_t x=0; x<(int32_t)lmmseImage.width; ++x) {
                    char id = map[((y & 1) << 1) | (x & 1)];
                    if(id == R) {
                        std::vector<std::pair<float, float>> ests = {
                            directionalEstimate(x, y, B, 1, 1),
                            directionalEstimate(x, y, B, 1, -1),
                        };
                        float value = blendEstimates(ests);
                        lmmseImage.setColor(x, y, B, clampSensor(value));
                    } else if(id == B) {
                        std::vector<std::pair<float, float>> ests = {
                            directionalEstimate(x, y, R, 1, 1),
                            directionalEstimate(x, y, R, 1, -1),
                        };
                        float value = blendEstimates(ests);
                        lmmseImage.setColor(x, y, R, clampSensor(value));
                    }
                }
            }

            image = std::move(lmmseImage);
        } break;

    }

    return image;
}
