#pragma once
#include "orderedPoint.h"
#include "projection.h"
#include <cstdlib>

class Condition
{
    Projection projection_;
    float bound_;

public:
    Condition(Projection const& projection, float const& bound) : projection_(projection), bound_(bound){};

    auto isSatisfiedBy(OrderedPoint const& point) const -> bool{
        return std::abs(projection_(point)) <= bound_;
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