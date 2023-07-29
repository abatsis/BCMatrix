#pragma once
#include "transformation.h"
#include <algorithm>

class RandomWalk
{
    std::vector<int> translations_;
    Transformation transform_;
    std::vector<float> weights_;
    int size_;

public:
    RandomWalk(Transformation const& transformation, std::vector<int> const& translations, std::vector<float> const& weights) :
    transform_(transformation), translations_(translations), weights_(weights){
        size_ = std::min(translations.size(), weights.size());
    };

    auto generateFrom(OrderedPoint const& parent_point) -> std::vector<OrderedPoint>
    {
        std::vector<OrderedPoint> points;
       for(int i = 0; i < size_; i++){
        auto translation = translations_[i];
        auto child_point = transform_(parent_point, translation);
        points.emplace_back(child_point);
       }
        return points;
    }

    auto getWeights() -> std::vector<float>{
        return weights_; 
    }

    auto getSize() -> int{
        return size_;
    }
};
