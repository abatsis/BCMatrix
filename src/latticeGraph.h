#pragma once
#include "orderedPoint.h"
#include "randomWalk.h"
#include "boundary.h"
#include <unordered_map>

struct weightedEdge
{
    OrderedPoint start;
    OrderedPoint end;
    double weight;
};

struct Hash
{
    size_t operator()(const OrderedPoint &orderedPoint) const
    {
        auto coords = orderedPoint.coords;
        auto hasher = std::hash<int>{};
        size_t seed = coords.size();
        for (const int &value : coords)
        {
            // Combine the hash value of each element using XOR
            seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct EqualPoints
{
    bool operator()(OrderedPoint const &point1, OrderedPoint const &point2) const
    {
        if (point1.coords.size() != point2.coords.size())
            return false;

        auto equalValues = true;
        for (int i = 0; i < point1.coords.size(); ++i)
        {
            if (point1.coords[i] != point2.coords[i])
                equalValues = false;
        }
        return equalValues;
    }
};

class LatticeGraph
{

    std::unordered_map<OrderedPoint, int, Hash, EqualPoints> nodes_;
    std::vector<weightedEdge> edges_;
    int dimension_;

    auto placeStartingNode(std::vector<OrderedPoint> &lastGeneration)
    {
        auto startingPoint = OrderedPoint{std::vector<int>(dimension_, 0), 0};
        nodes_.emplace(startingPoint, 0);
        lastGeneration.emplace_back(startingPoint);
    }

    auto updateDataStructures(OrderedPoint &childPoint,
                              OrderedPoint &parentPoint,
                              double weight,
                              std::vector<OrderedPoint> &newGeneration) -> void
    {
        auto placement = nodes_.emplace(childPoint, 0);
        auto pointIsNew = placement.second;
        if (pointIsNew)
            newGeneration.emplace_back(childPoint);
        auto edge = weightedEdge{parentPoint, childPoint, weight};
        edges_.emplace_back(edge);
    }

    auto sortNodes(Projection &sortingProjection)
    {
        std::vector<OrderedPoint> sortingVector;
        for (auto &it : nodes_)
        {
            auto node = it.first;
            sortingVector.emplace_back(node);
        }

        auto comparator = [sortingProjection](OrderedPoint point1, OrderedPoint point2)
        { return sortingProjection(point1) < sortingProjection(point2); };

        std::sort(sortingVector.begin(), sortingVector.end(), comparator);
        for (int i = 0; i < sortingVector.size(); i++)
        {
            auto node = sortingVector[i];
            nodes_[node] = i;
        }
    }

    auto updateEdges()
    {
        for (auto &edge : edges_)
        {
            auto startPoint = &edge.start;
            auto endPoint = &edge.end;
            startPoint->position = nodes_[*startPoint];
            endPoint->position = nodes_[*endPoint];
        }
    }

public:
    LatticeGraph(RandomWalk &randomWalk, Boundary &boundary, Projection &sortingProjection)
    {
        dimension_ = randomWalk.getDimension();
        auto weights = randomWalk.getWeights();

        std::vector<OrderedPoint> lastGeneration;

        placeStartingNode(lastGeneration);
        while (!lastGeneration.empty())
        {
            std::vector<OrderedPoint> newGeneration;
            for (auto parentPoint : lastGeneration)
            {
                auto children = randomWalk.generateFrom(parentPoint);
                for (int i = 0; i < randomWalk.getSize(); i++)
                {
                    auto childPoint = children[i];
                    if (boundary.encloses(childPoint))
                    {
                        auto weight = weights[i];
                        updateDataStructures(childPoint, parentPoint, weight, newGeneration);
                    }
                }
            }
            lastGeneration = newGeneration;
        }
        sortNodes(sortingProjection);
        updateEdges();
    };

    auto numberOfNodes() -> int
    {
        return nodes_.size();
    };

    auto getEdges() -> std::vector<weightedEdge> *
    {
        return &edges_;
    }

    auto clear()
    {
        nodes_.clear();
        edges_.clear();
    }
};