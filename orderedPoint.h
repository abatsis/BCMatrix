#pragma once
#include <vector>
#include <ostream>

struct OrderedPoint
{
    std::vector<double> coords;
    int position = 0;
};

std::ostream &operator<<(std::ostream &stream, OrderedPoint &OrderedPoint)
{
    for (auto &number : OrderedPoint.coords)
    {
        stream << number << ", ";
    }
    stream << std::endl;
    return stream;
};