#pragma once
#include <vector>

class Projection
{
    float parameter_;

public:
    Projection(float const& parameter) : parameter_(parameter){};

    auto operator()(OrderedPoint const &point) const -> float
    {
        auto coords = point.coords;
        float powerOfParameter = 1;
        float result = 0;
        for (auto const &coord : coords)
        {
            result = result + coord * powerOfParameter;
            powerOfParameter = powerOfParameter * parameter_;
        }
        return result;
    }
};
