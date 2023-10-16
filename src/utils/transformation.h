#pragma once
#include "orderedPoint.h"

class Transformation
{
    std::vector<int> coefficients_;

public:
    Transformation(std::vector<int> coefficients) : coefficients_(coefficients){};

    OrderedPoint operator()(OrderedPoint const &orderedPoint, int const &translation)
    {
        auto size = orderedPoint.coords.size();
        std::vector<int> newCoords(size, 0);
        auto lastTerm = orderedPoint.coords.back();

        for (int i = 0; i < size - 1; i++)
        {
            newCoords[i + 1] = orderedPoint.coords[i];
        }

        for (int i = 0; i < size; i++)
        {
            newCoords[i] += -coefficients_[i] * lastTerm;
        }

        newCoords[0] += translation;

        return {newCoords, 0};
    }

    auto getDimension() -> int
    {
        return coefficients_.size();
    }

    auto polynomial() -> std::vector<int> *
    {
        return &coefficients_;
    };
};

std::ostream &operator<<(std::ostream &stream, Transformation &transformation)
{
    for (auto &number : *transformation.polynomial())
    {
        stream << number << ", ";
    }
    stream << std::endl;
    return stream;
};