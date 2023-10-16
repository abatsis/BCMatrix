#pragma once
#include "orderedPoint.h"
#include "projection.h"
#include <cstdlib>

class Condition
{
    Projection projection_;
    double bound_;
    double epsilon_ = 0.01;

public:
    Condition(Projection const &projection, double const &bound) : projection_(projection), bound_(bound){};

    auto isSatisfiedBy(OrderedPoint const &point) const -> bool
    {
        return std::abs(projection_(point)) <= bound_ + epsilon_;
    }

    auto projection() -> double
    {
        return projection_.parameter();
    };

    auto bound() -> double
    {
        return bound_;
    };

    auto epsilon() -> double
    {
        return epsilon_;
    };

    auto setSensitivety(double const &sensitivety) -> void
    {
        epsilon_ = sensitivety;
    }
};

class Boundary
{
    std::vector<Condition> conditions_;

public:
    Boundary(std::vector<Condition> const &conditions) : conditions_(conditions){};

    auto encloses(OrderedPoint const &point)
    {
        for (auto const &condition : conditions_)
        {
            if (!condition.isSatisfiedBy(point))
            {
                return false;
            }
        }
        return true;
    }

    auto setSensitivety(double const &sensitivety) -> void
    {
        for (auto &condition : conditions_)
        {
            condition.setSensitivety(sensitivety);
        }
    };

    auto getConditions() -> std::vector<Condition> *
    {
        return &conditions_;
    }
};

std::ostream &operator<<(std::ostream &stream, Condition &condition)
{
    stream << "[projection=" << condition.projection()
           << ", bound=" << condition.bound() << ", epsilon=" << condition.epsilon() << "]" << std::endl;
    return stream;
};

std::ostream &operator<<(std::ostream &stream, Boundary &boundary)
{
    stream << "Boundary start" << std::endl;
    for (auto &condition : *boundary.getConditions())
    {
        stream << condition << std::endl;
    }
    stream << "Boundary end" << std::endl;
    return stream;
};