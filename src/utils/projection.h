#pragma once
#include <vector>

class Projection
{
    double parameter_;

public:
    Projection(double const &parameter) : parameter_(parameter){};

    auto operator()(OrderedPoint const &point) const -> double
    {
        auto coords = point.coords;
        double powerOfParameter = 1;
        double result = 0;
        for (auto const &coord : coords)
        {
            result = result + coord * powerOfParameter;
            powerOfParameter = powerOfParameter * parameter_;
        }
        return result;
    }

    auto parameter() -> double
    {
        return parameter_;
    };
};

std::ostream &operator<<(std::ostream &stream, Projection &projection)
{
    stream << projection.parameter() << std::endl;
    return stream;
}