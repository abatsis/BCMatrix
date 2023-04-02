#pragma
#include <vector>

class Projection
{
    float parameter_;

public:
    Projection(float parameter) : parameter_(parameter){};

    float operator()(std::vector<float> const &integers)
    {
        float powerOfParameter = 1;
        float result = 0;
        for (auto const &integer : integers)
        {
            result = result + integer * powerOfParameter;
            powerOfParameter = powerOfParameter * parameter_;
        }
        return result;
    }
};
