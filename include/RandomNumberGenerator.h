/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of random number generator class   ///
///     and its source code - it is supposed to       ///
///     return random integers and floats             ///
/////////////////////////////////////////////////////////
#pragma once

#include <random>

template <typename Numeric, typename RealOnlyNumeric = float>
class RandomNumberGenerator
{
private:
    using distributionType = typename std::conditional<
        std::is_integral<Numeric>::value,
        std::uniform_int_distribution<Numeric>,
        std::uniform_real_distribution<Numeric>>::type;

    std::mt19937_64 generator_;
    distributionType distribution_;
    std::normal_distribution<RealOnlyNumeric> normalDistribution_;

public:
    RandomNumberGenerator(Numeric min = 0.0f, Numeric max = 1.0f, RealOnlyNumeric mean = 5.0, RealOnlyNumeric stddev = 2.0)
    {
        std::random_device rd;
        this->generator_ = std::mt19937_64(rd());
        this->distribution_ = distributionType(min, max);
        this->normalDistribution_ = std::normal_distribution<RealOnlyNumeric>(mean, stddev);
    }
    Numeric get()
    {
        return distribution_(generator_);
    }
    Numeric get(Numeric min, Numeric max)
    {
        typename distributionType::param_type d2(min, max);
        distribution_.param(d2);
        return distribution_(generator_);
    }

    RealOnlyNumeric getNormal()
    {
        return normalDistribution_(generator_);
    }

    RealOnlyNumeric getNormal(RealOnlyNumeric mean, RealOnlyNumeric stddev)
    {
        this->normalDistribution_ = std::normal_distribution<RealOnlyNumeric>(mean, stddev);
        return normalDistribution_(generator_);
    }

    float getNormalMin()
    {
        return normalDistribution_.min();
    }
    float getNormalMax()
    {
        return normalDistribution_.max();
    }
};