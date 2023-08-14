#pragma once
#include "orderedPoint.h"
#include "projection.h"
#include <cstdlib>

class Condition
{
    Projection projection_;
    float bound_;
    float epsilon_ = 0.01;

public:
    Condition(Projection const& projection, float const& bound) : projection_(projection), bound_(bound){};

    auto isSatisfiedBy(OrderedPoint const& point) const -> bool{
        return std::abs(projection_(point)) <= bound_ + epsilon_;
    }
};


class Boundary 
{
    std::vector<Condition> conditions_;
    public:
    Boundary(std::vector<Condition> const& conditions) : conditions_(conditions){};

    auto encloses(OrderedPoint const& point){
        for (auto const& condition : conditions_ ){
            if (!condition.isSatisfiedBy(point)){
                return false;
            }
        }
        return true;
    }
};