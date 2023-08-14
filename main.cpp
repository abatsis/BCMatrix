#include "transformation.h"
#include "randomWalk.h"
#include <iostream>
#include "projection.h"
#include "boundary.h"

template <typename T>
 static auto printVector(std::vector<T> const& aVector) {
    for (auto const& i : aVector){
        std::cout << i << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    std::vector<float> coords = {1,0,0,3};
    OrderedPoint point = {{1,0,0,3}, 0};

    printVector(point.coords);
    auto transform = Transformation({0,0,0,5});
    auto child_point = transform(point,0);
    printVector(child_point.coords);

    std::vector<int> translations = {-1,0,1};
    std::vector<float> weights = {0.5,1,0.5};

    auto randomWalk = RandomWalk(transform, translations, weights);
    auto child_points = randomWalk.generateFrom(point);

    for (auto i : child_points){
        printVector(i.coords);
    }

    std::vector<Condition> conditions;
    auto projection = Projection(2);
    auto condition = Condition(projection, 1000);
    conditions.emplace_back(condition);

    projection = Projection(4);
    condition = Condition(projection, 1000);
    conditions.emplace_back(condition);

    projection = Projection(1.45);
    condition = Condition(projection, 1000);
    conditions.emplace_back(condition);
    
    projection = Projection(3);
    condition = Condition(projection, 1000);
    conditions.emplace_back(condition);

    auto boundary = Boundary(conditions);

    std::cout << "is in boundary: " << boundary.encloses(point) << std::endl;


}