#pragma once
#include "orderedPoint.h"
#include "randomWalk.h"
#include "boundary.h"
#include <unordered_map>

struct weightedEdge
{
    OrderedPoint *start;
    OrderedPoint *end;
    double weight;
};

template <typename T>
struct VectorHash
{
    size_t operator()(const std::vector<T> &vec) const
    {
        size_t seed = vec.size();
        for (const T &value : vec)
        {
            // Combine the hash value of each element using XOR
            seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

using Nodes = std::unordered_map<size_t, OrderedPoint>;

class LatticeGraph
{

    Nodes nodes_;
    std::vector<weightedEdge> edges_;
    int dimension_;
    VectorHash<double> getHashKey;

    auto placeStartingNode(std::vector<OrderedPoint *> &lastGeneration)
    {
        auto startingPoint = OrderedPoint{std::vector<double>(dimension_, 0), 0};
        auto iterator = nodes_.emplace(getHashKey(startingPoint.coords), startingPoint);
        lastGeneration.emplace_back(&iterator.first->second);
    }

    auto updateDataStructures(OrderedPoint &childPoint,
                              OrderedPoint *parentPoint,
                              double weight,
                              std::vector<OrderedPoint *> &newGeneration,
                              std::vector<OrderedPoint *> &sortingVector) -> void
    {
        auto key = getHashKey(childPoint.coords);
        auto iterator = nodes_.emplace(key, childPoint);
        auto pointIsNew = iterator.second;
        auto childPointPointer = &iterator.first->second;
        if (pointIsNew)
        {
            newGeneration.emplace_back(childPointPointer);
            sortingVector.emplace_back(childPointPointer);
        }
        auto edge = weightedEdge{parentPoint, childPointPointer, weight};
        edges_.emplace_back(edge);
    }

    auto sortNodes(std::vector<OrderedPoint *> &sortingVector, Projection &sortingProjection)
    {
        auto comparator = [sortingProjection](OrderedPoint *point1, OrderedPoint *point2)
        { return sortingProjection(*point1) < sortingProjection(*point2); };
        std::sort(sortingVector.begin(), sortingVector.end(), comparator);
        for (int i = 0; i < sortingVector.size(); i++)
        {
            sortingVector[i]->position = i;
        }
    }

public:
    LatticeGraph(RandomWalk &randomWalk, Boundary &boundary, Projection &sortingProjection)
    {
        dimension_ = randomWalk.getDimension();
        auto weights = randomWalk.getWeights();

        std::vector<OrderedPoint *> lastGeneration;
        std::vector<OrderedPoint *> sortingVector;

        placeStartingNode(lastGeneration);
        while (!lastGeneration.empty())
        {
            std::vector<OrderedPoint *> newGeneration;
            for (auto parentPoint : lastGeneration)
            {
                auto children = randomWalk.generateFrom(*parentPoint);
                for (int i = 0; i < randomWalk.getSize(); i++)
                {
                    auto childPoint = children[i];
                    if (boundary.encloses(childPoint))
                    {
                        auto weight = weights[i];
                        updateDataStructures(childPoint, parentPoint, weight, newGeneration, sortingVector);
                    }
                }
            }
            lastGeneration = newGeneration;
        }
        sortNodes(sortingVector, sortingProjection);
    };

    auto numberOfNodes() -> int
    {
        return nodes_.size();
    };

    auto getEdges() -> std::vector<weightedEdge> *
    {
        return &edges_;
    }
};