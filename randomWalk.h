#pragma once
#include "transformation.h"
#include <algorithm>

class RandomWalk
{
    std::vector<int> translations_;
    Transformation transform_;
    std::vector<double> weights_;
    int size_ = 0;

public:
    RandomWalk(Transformation const &transformation, std::vector<int> const &translations, std::vector<double> const &weights) : transform_(transformation), translations_(translations), weights_(weights)
    {
        size_ = std::min(translations.size(), weights.size());
    };

    auto generateFrom(OrderedPoint const &parent_point) -> std::vector<OrderedPoint>
    {
        std::vector<OrderedPoint> points;
        for (auto &translation : translations_)
        {
            auto child_point = transform_(parent_point, translation);
            points.emplace_back(child_point);
        }
        return points;
    }

    auto getWeights() -> std::vector<double>
    {
        return weights_;
    }

    auto getTranslations() -> std::vector<int>
    {
        return translations_;
    }

    auto getSize() -> int
    {
        return size_;
    }

    auto getDimension() -> int
    {
        return transform_.getDimension();
    }

    auto getTransormationPolynomial() -> std::vector<double> *
    {
        return transform_.polynomial();
    }
};

std::ostream &operator<<(std::ostream &stream, RandomWalk &randomWalk)
{
    stream << "Random Walk Start" << std::endl
           << "transformation: ";

    for (auto &number : *randomWalk.getTransormationPolynomial())
    {
        stream << number << ", ";
    }

    stream << std::endl
           << "translations: ";

    for (auto &number : randomWalk.getTranslations())
    {
        stream << number << ", ";
    }

    stream << std::endl
           << "weights: ";

    for (auto &number : randomWalk.getWeights())
    {
        stream << number << ", ";
    }

    stream << std::endl
           << "Random Walk End" << std::endl;
    return stream;
};