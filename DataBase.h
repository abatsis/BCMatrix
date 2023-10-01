#include <string>
#include <memory>
#include <vector>
#include "boundary.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "randomWalk.h"

enum class Type
{
    POLYNOMIAL = 0,
    TRANSLATIONS = 1,
    PHYSICAL_PROJECTION = 2,
    SORTING_PROJECTION = 3,
    CONDITION = 4,
    WEIGHTS = 5
};

static auto readIntegers(std::string const &line) -> std::vector<int>
{
    std::string numberString;
    std::stringstream lineStream(line);
    std::vector<int> numbers;

    while (getline(lineStream, numberString, ' '))
    {
        numbers.emplace_back(stoi(numberString));
    }
    return numbers;
}

static auto readDoubles(std::string const &line) -> std::vector<double>
{
    std::string numberString;
    std::stringstream lineStream(line);
    std::vector<double> numbers;

    while (getline(lineStream, numberString, ' '))
    {
        numbers.emplace_back(stod(numberString));
    }
    return numbers;
}

static auto readProjection(std::string const &line) -> Projection
{
    std::string numberString;
    std::stringstream lineStream(line);
    getline(lineStream, numberString, ' ');
    auto parameter = stod(numberString);
    return Projection(parameter);
}

static auto readCondition(std::string const &line) -> Condition
{
    std::string numberString;
    std::stringstream lineStream(line);
    getline(lineStream, numberString, ' ');
    auto parameter = stod(numberString);
    auto projection = Projection(parameter);
    getline(lineStream, numberString, ' ');
    auto bound = stod(numberString);
    return Condition(projection, bound);
}

static auto stringContains(std::string &string, std::string substring) -> bool
{
    return string.find(substring) != std::string::npos;
}

class DataBase
{
    std::vector<double> polynomial_;
    std::vector<int> translations_;
    double physicalProjection_;
    std::unique_ptr<Projection> shortingProjection_;
    std::vector<Condition> conditions_;
    std::vector<double> weights_;
    std::unique_ptr<Boundary> boundary_;
    std::unique_ptr<RandomWalk> randomwalk_;

    Type type_;

    auto buildBoundaryAndRandomWalk() -> void
    {
        Transformation transform(polynomial_);
        randomwalk_ = std::make_unique<RandomWalk>(transform, translations_, weights_);
        boundary_ = std::make_unique<Boundary>(conditions_);
    }

    auto readType(std::string &line) -> bool
    {
        auto typeLine = false;
        if (stringContains(line, "POLYNOMIAL"))
        {
            type_ = Type::POLYNOMIAL;
            return true;
        }
        else if (stringContains(line, "TRANSLATIONS"))
        {
            type_ = Type::TRANSLATIONS;
            return true;
        }
        else if (stringContains(line, "PHYSICAL PROJECTION"))
        {
            type_ = Type::PHYSICAL_PROJECTION;
            return true;
        }
        else if (stringContains(line, "SORTING PROJECTION"))
        {
            type_ = Type::SORTING_PROJECTION;
            return true;
        }
        else if (stringContains(line, "CONDITION"))
        {
            type_ = Type::CONDITION;
            return true;
        }
        else if (stringContains(line, "WEIGHTS"))
        {
            type_ = Type::WEIGHTS;
            return true;
        }
        return typeLine;
    }

public:
    auto readFile(std::string const &filename) -> void
    {
        std::ifstream file(filename);
        if (!file)
            std::cout << "could not open file" << filename << std::endl;

        std::string line;
        while (getline(file, line, '\n'))
        {
            if (empty(line))
                continue;
            auto typeLine = readType(line);

            if (typeLine)
                continue;

            switch (type_)
            {
            case Type::POLYNOMIAL:
                polynomial_ = readDoubles(line);
                break;
            case Type::TRANSLATIONS:
                translations_ = readIntegers(line);
                break;
            case Type::PHYSICAL_PROJECTION:
                physicalProjection_ = readDoubles(line)[0];
                break;
            case Type::SORTING_PROJECTION:
                shortingProjection_ = std::make_unique<Projection>(readProjection(line));
                break;
            case Type::WEIGHTS:
                weights_ = readDoubles(line);
                break;
            case Type::CONDITION:
                conditions_.emplace_back(readCondition(line));
                break;
            }
        }
        buildBoundaryAndRandomWalk();
    }

    auto getRandomWalk() -> RandomWalk
    {
        return *randomwalk_;
    }
    auto getBoundary() -> Boundary
    {
        return *boundary_;
    }
    auto getShortingProjection() -> Projection
    {
        return *shortingProjection_;
    }
};
