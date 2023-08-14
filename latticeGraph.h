#pragma once
#include "orderedPoint.h"
#include "randomWalk.h"
#include "boundary.h"

struct weightedEdge{
    OrderedPoint* start;
    OrderedPoint* end;
    float weight;
}

using Generation = std::unordered_set<OrderedPoint>; 

class LatticeGraph{

std::vector<Generation> nodeGenerations_;
Boundary boundary_;
RandomWalk randomWalk_;
int dimension_;

public:
LatticeGraph(RandomWalk const& randomWalk, Boundary const& boundary): randomWalk_(randomWalk), boundary_(boundary){
    dimension_ = randomWalk.getSize();
    auto startingPoint = OrderedPoint{std::vector<float>(dimension_, 0), 0};


};

}