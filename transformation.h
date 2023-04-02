#pragma
#include <vector>

class Transformation
{
    std::vector<float> coefficients_;

public:
    Transformation(std::vector<float> coefficients) : coefficients_(coefficients){};

    std::vector<float> operator()(std::vector<float> const &integers, int const &translation)
    {
        for (int i = 0; i < integers.size() - 1; i++)
        {
            integers[i + 1] = integers[i];
        }

        auto lastTerm = integers.back();

        for (int i = 0; i < integers.size(); i++)
        {
            integers[i] = -coefficients_[i] * lastTerm;
        }

        integers[0] += translation;

        return integers;
    }
};
