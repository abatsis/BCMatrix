#pragma once
#include "orderedPoint.h"

class Transformation
{
    std::vector<float> coefficients_;

public:
    Transformation(std::vector<float> coefficients) : coefficients_(coefficients){};

    OrderedPoint operator()(OrderedPoint const &orderedPoint, int const &translation)
    {
        auto size = orderedPoint.coords.size();
        std::vector<float> newCoords(size, 0);
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
};
