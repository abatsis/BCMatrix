#pragma once
#include <vector>
#include <ostream>

struct OrderedPoint
{
    std::vector<int> coords;
    int position = 0;
};

std::ostream &operator<<(std::ostream &stream, OrderedPoint &orderedPoint)
{
    stream << "[ (";
    auto atFirstNumber = true;
    for (auto &number : orderedPoint.coords)
    {
        if (atFirstNumber)
        {
            stream << number;
            atFirstNumber = false;
        }
        else
            stream << ", " << number;
    }
    stream << "), pos= " << orderedPoint.position << " ]";
    return stream;
};